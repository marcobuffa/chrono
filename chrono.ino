#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include "time.h"
#include "room.h"
#include "program.h"
#include "isr.h"
#include "HWsettings.h"
#include "SWsettings.h"

//global variables :-@
int refresh = 1;
int clean = 1;
int dots = 1;
int actPset = 0;
int actDWset = 1;
enum mode actMode = STD;
enum set toSet = HOUR;
enum progSet toProg = ENABLED;
datetype now{30, 59, 15, 12, 4, 11, 2020}; //initial date (guess why...)
onInterval interval[7][3] = { //start, stop, dw, temp, enabled
            {{32400, 36000, 1, 19, 1},{ 39600, 43200, 1, 19, 1},{ 46800, 50400, 1, 19, 1}},
            {{32400, 36000, 2, 19, 1},{ 39600, 43200, 2, 19, 1},{ 46800, 50400, 2, 19, 1}},
            {{32400, 36000, 3, 19, 1},{ 39600, 43200, 3, 19, 1},{ 46800, 50400, 3, 19, 1}},
            {{32400, 36000, 4, 19, 1},{ 39600, 43200, 4, 19, 1},{ 46800, 50400, 4, 19, 1}},
            {{32400, 36000, 5, 19, 1},{ 39600, 43200, 5, 19, 1},{ 46800, 50400, 5, 19, 1}},
            {{32400, 36000, 6, 19, 1},{ 39600, 43200, 6, 19, 1},{ 46800, 50400, 6, 19, 1}},
            {{32400, 36000, 7, 19, 1},{ 39600, 43200, 7, 19, 1},{ 46800, 50400, 7, 19, 1}}        
}; //ON intervals array (7 days x 3 intervals each days)

//instantiate display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//instantiate room sensor
DHT_nonblocking dht_sensor(dhtData, dhtType);

//------------------------
//room measurement routine
//------------------------
void measureRoom(float *temperature, float *humidity)
{
  static unsigned long timestamp = millis();

  if(millis() - timestamp > 5000ul){
    if(dht_sensor.measure(temperature, humidity) == true){
      timestamp = millis();

#ifdef SERIALDEBUG
      //Serial.println("New room data!");
#endif
    }
  }
  return;
}

//---------------------
//Arduino SETUP routine
//---------------------
void setup() {
  
  //init buttons & ISRs
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button2), ISRMenuButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(button3), ISRAuxButton, FALLING);
  
  //init serial debug port
#ifdef SERIALDEBUG
  Serial.begin(9600);
#endif

  //init 16x2 display
  lcd.begin(16, 2);
}

//---------------
//MENU button ISR
//---------------
void ISRMenuButton(){
  static unsigned long last;
  
  if(micros() - last >= DEBOUNCE) {
    last = micros();

    refresh = 1; //set display to be updated

    mainMenuFSM(&actMode, &toSet, &toProg); //navigate through modes of operation and date/time setting
    
#ifdef SERIALDEBUG
    Serial.println("ISRMenuButton!");
    Serial.print("actPset = ");
    Serial.print(actPset);
    Serial.print(" - actDWset = ");
    Serial.print(actDWset);
    Serial.print(" - toProg = ");
    Serial.println(toProg);
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

#ifdef SERIALDEBUG
    Serial.println("ISRAuxButton!");
#endif

    refresh = 1; //set display to be updated
    dots = 1; //force flash on

    //check what to do now (depending on actual operating mode)
    switch (actMode) {
      
      //date & time setting mode
      case SETTIME:
        setTimeSwitch(&toSet, &now); //set date/time
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
  char time[16], date[16], r[8], program[8], dw[8];

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
        formatProgTimes(program, actPset, &interval[actDWset-1][actPset], dots);
        lcd.setCursor(0, 0); //set display position
        lcd.print(program); //print chrono programming string
        formatDW(dw, actDWset);
        lcd.setCursor(0, 1); //set display position
        lcd.print(dw);
        break;
      
      default:
        break;
    }
  }
}
