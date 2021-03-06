#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include <EEPROM.h>
#include "time.h"
#include "room.h"
#include "program.h"
#include "isr.h"
#include "HWsettings.h"
#include "SWsettings.h"

//global variables :-@
#ifdef SERIALDEBUG
char debug[50];
#endif
int refresh = 1;
int clean = 1;
int dots = 1;
int actPset = 0;
int actDWset = 1;
enum mode actMode = STD;
enum set toSet = HOUR;
enum progSet toProg = ENABLED;
datetype now{30, 40, 14, 20, 5, 11, 2020}; //initial date (guess why...)

onInterval interval[7][3] = { //start, stop, temp, enabled
            {{32400, 36000, 19, 1}, {39600, 43200, 19, 0}, {46800, 50400, 19, 1}}, //LUN
            {{32400, 36000, 19, 1}, {39600, 43200, 19, 0}, {46800, 50400, 19, 1}}, //MAR
            {{32400, 36000, 19, 1}, {39600, 43200, 19, 0}, {46800, 50400, 19, 1}}, //MER
            {{32400, 36000, 19, 1}, {39600, 43200, 19, 0}, {46800, 50400, 19, 1}}, //GIO
            {{32400, 36000, 19, 1}, {39600, 79200, 23, 1}, {46800, 50400, 19, 0}}, //VEN
            {{32400, 36000, 19, 1}, {39600, 43200, 19, 0}, {46800, 50400, 19, 1}}, //SAB
            {{32400, 36000, 19, 1}, {39600, 43200, 19, 0}, {46800, 50400, 19, 1}}  //DOM
}; //ON intervals array (7 days x 3 intervals each days)

unsigned long int elapsedNotSTD = 0;

//instantiate display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//instantiate room sensor
DHT_nonblocking dht_sensor(dhtData, dhtType);

//------------------------
//room measurement routine
//------------------------
void measureRoom(float *temperature, float *humidity){
  static unsigned long timestamp = millis();
  int heater;

  if(millis() - timestamp > 5000ul){
    if(dht_sensor.measure(temperature, humidity) == true){
      timestamp = millis();
      heater = shouldHeat(*temperature, &now, interval);
      digitalWrite(heat, heater);
      
#ifdef SERIALDEBUG
      //Serial.println(debug);
#endif
    }
  }
  return;
}

void saveProgram(){
  char *toSave;
  toSave = (char *)&interval;
  int sizeToSave = sizeof(interval);
  int i;
  
  for (i=0; i<sizeToSave; i++){
    EEPROM.update(i+512, toSave[i]);
  }
  
  return;
}

//---------------------
//Arduino SETUP routine
//---------------------
void setup() {
  char *timeToRetrieve;
  timeToRetrieve = (char *)&now;
  int i;
  
  //init buttons & ISRs
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button2), ISRMenuButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(button3), ISRAuxButton, FALLING);

  pinMode(heat, OUTPUT);
  digitalWrite(heat, 0);
  
  //init serial debug port
#ifdef SERIALDEBUG
  Serial.begin(9600);
#endif

  //init 16x2 display
  lcd.begin(16, 2);

  //retrieve last time saved from EEPROM
  for (i=0; i<sizeof(now); i++){
    timeToRetrieve[i] = EEPROM.read(i);
  }
}

//---------------
//MENU button ISR
//---------------
void ISRMenuButton(){
  static unsigned long last;
  
  if(micros() - last >= DEBOUNCE) {
    last = micros();
    elapsedNotSTD = millis();
    
    refresh = 1; //set display to be updated

    mainMenuFSM(&actMode, &toSet, &toProg); //navigate through modes of operation and date/time setting
    
#ifdef SERIALDEBUG
    //Serial.println("ISRMenuButton!");
#endif

  }
}

//---------------
//AUXR button ISR
//---------------
void ISRAuxButton(){
  static unsigned long last;
  
  if(micros() - last >= DEBOUNCE) {
    last = micros();
    elapsedNotSTD = millis();
    
#ifdef SERIALDEBUG
    //Serial.println("ISRAuxButton!");
#endif

    refresh = 1; //set display to be updated
    dots = 1; //force flash on

    //check what to do now (depending on actual operating mode)
    switch (actMode) {
      
      //date & time setting mode
      case SETTIME:
        setTimeSwitch(&toSet, &now); //set date/time
        break;

      //programming mode
      case SETPROG:
        setProgSwitch(&toProg, &interval[actDWset-1][actPset]); //set programs
        break;
        
      default:
        break;
    }
  }
}

//--------------------
//Arduino LOOP routine
//--------------------
void loop() {
  float temperature, humidity;
  static unsigned long timestamp = millis();
  char time[16], date[16], r[16], program[16], dw[4], temp[4], en[8];

  //try to get room measurement
  measureRoom(&temperature, &humidity);

  //every second
  if( millis() - timestamp >=1000ul ){
    timestamp = millis(); //update timestamp ASAP -> no RTC!
    
    runCalendar(&now); //update date/time
    dots = !dots; //toggle flash signal
    
    refresh = 1; //set display to be updated
  }

  //if display needs to be cleaned
  if (clean){
    clean = 0;
    lcd.clear();
  }

  //if display needs to be updated
  if (refresh){

#ifdef SERIALDEBUG
    //Serial.println("Refresh!");
#endif

    refresh = 0; //set display just updated

    //check what to do now (depending on actual operating mode)
    switch (actMode) {
      
      //standard mode
      case STD:
        elapsedNotSTD = millis();
        formatRoom(r, temperature, humidity); //format room measurement string
        lcd.setCursor(0, 0); //set display position
        lcd.print(r); //print room measurement
        formatTime(time, now.h, now.m, now.s, dots); //format time string
        lcd.setCursor(11, 0); //set display position
        lcd.print(time); //print time
        formatDate(date, now.y, now.M, now.d, now.dw, dots); //format date string
        lcd.setCursor(0, 1); //set display position
        lcd.print(date); //print date
        break;

      //date & time setting mode
      case SETTIME:
        if( millis() - elapsedNotSTD >= STDTIMEOUT ){
          actMode = STD;
          clean = 1;
          saveTime(&now);
        }
        lcd.setCursor(0, 0); //set display position
        lcd.print("Set d/o   "); //print date & time setting string
        formatTime(time, now.h, now.m, now.s, dots); //format time string
        lcd.setCursor(11, 0); //set display position
        lcd.print(time); //print time
        formatDate(date, now.y, now.M, now.d, now.dw, dots); //format date string
        lcd.setCursor(0, 1); //set display position
        lcd.print(date); //print date
        break;

      //chrono programming mode
      case SETPROG:
        if( millis() - elapsedNotSTD >= STDTIMEOUT ){
          actMode = STD;
          clean = 1;
          saveProgram();
        }
        formatProgTimes(program, actPset, &interval[actDWset-1][actPset], dots);
        lcd.setCursor(0, 0); //set display position
        lcd.print(program); //print chrono programming string
        formatDW(dw, actDWset);
        lcd.setCursor(0, 1); //set display position
        lcd.print(dw);
        formatTemp(temp, &interval[actDWset-1][actPset], dots);
        lcd.setCursor(4, 1); //set display position
        lcd.print(temp);
        formatEn(en, &interval[actDWset-1][actPset], dots);
        lcd.setCursor(8, 1); //set display position
        lcd.print(en);
        break;
      
      default:
        break;
    }
  }
}
