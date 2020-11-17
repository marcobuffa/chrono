#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include "time.h"
#include "room.h"
#include "isr.h"
#include "HWsettings.h"
#include "SWsettings.h"

//global variables :-@
int refresh = 1;
int dots = 1;
enum mode actMode = STD;
enum set toSet = HOUR;

//instantiate display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//instantiate room sensor
DHT_nonblocking dht_sensor(dhtData, dhtType);

//initial date (guess why...)
date now{30, 59, 15, 12, 4, 11, 2020};

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
      Serial.println("New room data!");
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
  
  if(micros() - last >= debounce * 1000) {
    last = micros();

#ifdef SERIALDEBUG
    Serial.println("ISRMenuButton!");
#endif

    refresh = 1; //set display to be updated

    mainMenuFSM(&actMode, &toSet); //navigate through modes of operation and date/time setting
  }
}

//---------------
//AUXR button ISR
//---------------
void ISRAuxButton(){
  static unsigned long last;
  
  if(micros() - last >= debounce * 1000) {
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
  char time[16], date[16], r[8];

  //try to get room measurement
  measureRoom(&temperature, &humidity);

  //every second
  if( millis() - timestamp >=1000ul ){
    timestamp = millis(); //update timestamp ASAP -> no RTC!
    
    runCalendar(&now); //update date/time
    dots = !dots; //toggle flash signal
    
    refresh = 1; //set display to be updated
  }

  //if display needs update
  if (refresh){

#ifdef SERIALDEBUG
    Serial.println("Refresh!");
#endif

    refresh = 0; //set display just updated
    
    formatTime(time, now.h, now.m, now.s, dots); //format time string
    lcd.setCursor(11, 0); //set display position
    lcd.print(time); //print time
    
    formatDate(date, now.y, now.M, now.d, now.dw, dots); //format date string
    lcd.setCursor(0, 1); //set display position
    lcd.print(date); //print date

    //check what to do now (depending on actual operating mode)
    switch (actMode) {
      
      //standard mode
      case STD:
        formatRoom(r, temperature, humidity); //format room measurement string
        lcd.setCursor(0, 0); //set display position
        lcd.print(r); //print room measurement
        break;

      //date & time setting mode
      case SETTIME:
        lcd.setCursor(0, 0); //set display position
        lcd.print("Set d/o   "); //print date & time setting string
        break;

      //chrono programming mode
      case SETPROG:
        lcd.setCursor(0, 0); //set display position
        lcd.print("Prog      "); //print chrono programming string
        break;
      
      default:
        break;
    }
  }
}
