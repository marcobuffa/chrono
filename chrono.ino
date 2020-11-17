#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include "time.h"
#include "room.h"

const int button2 = 2;
const int button3 = 3;

const unsigned long debounce = 250;
const int rs=7, en=8, d4=9, d5=10, d6=11, d7=12;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT_nonblocking dht_sensor(4, DHT_TYPE_11);

int refresh = 1;
int dots = 1;
enum mode actMode;
enum set toSet;
date now{30, 59, 15, 12, 4, 11, 2020};

//misurazione ambiente
void measureRoom(float *temperature, float *humidity)
{
  static unsigned long timestamp = millis();

  if(millis() - timestamp > 5000ul){
    if(dht_sensor.measure(temperature, humidity) == true){
      timestamp = millis();
    }
  }
  return;
}

void setup() {
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button2), ISRButton2, FALLING);
  attachInterrupt(digitalPinToInterrupt(button3), ISRButton3, FALLING);
  
  Serial.begin(9600);
  lcd.begin(16, 2); //inizializza display
  
  actMode = STD;
  toSet = HOUR;
}

void ISRButton2(){
  static unsigned long last;
  
  if(micros() - last >= debounce * 1000) {
    last = micros();

    refresh = true;
    
    switch (actMode) {
      case STD:
        actMode = SETTIME;
        break;
      case SETTIME:
        switch (toSet) {
          case HOUR:
            toSet = MINUTE;
            break;          
          case MINUTE:
            toSet = YEAR;
            break;
          case YEAR:
            toSet = MONTH;
            break;            
          case MONTH:
            toSet = DAY;
            break;
          case DAY:
            toSet = DAYOFWEEK;
            break;
          case DAYOFWEEK:
            toSet = HOUR;
            actMode = SETPROG;
          default:
            toSet = HOUR;
            actMode = SETPROG;
            break;
        }
        break;
      case SETPROG:
        actMode = STD;
        break;
      default:
        actMode = STD;
        break;
    }
  }
}

void ISRButton3(){
  static unsigned long last;
  int pos=0;
  
  if(micros() - last >= debounce * 1000) {
    last = micros();

    refresh = true;
    dots = 1;

    switch (actMode) {
      case SETTIME:
        switch (toSet) {
          case HOUR:
            now.h>=23 ? now.h=0 : now.h++;
            break;         
          case MINUTE:
            now.m>=59 ? now.m=0 : now.m++;
            break;
          case YEAR:
            now.M=1;
            now.d=1;
            now.y>=2099 ? now.y=2020 : now.y++;
            break;
          case MONTH:
            now.d=1;
            now.M>=12 ? now.M=1 : now.M++;
            break;
          case DAY:
            now.d=now.d+1;
            if (now.M==2){
              if ((now.y % 4)==0){
                pos=now.d > 29 ? 2 : pos;
              } else {
                pos=now.d > 28 ? 2 : pos;
              }
            } else {
              if ((now.M==1) || (now.M==3) || (now.M==5) || (now.M==7)
                    || (now.M==8) || (now.M==10) || (now.M==12)){
                pos=now.d > 31 ? 2 : pos;
              } else {
                pos=now.d > 30 ? 2 : pos;
              }
            }
            if (pos==2){
              now.d=1;
            }
            break;
          case DAYOFWEEK:
            now.dw>=7 ? now.dw=1 : now.dw++;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
}

void loop() {
  float temperature, humidity;
  static unsigned long timestamp = millis();
  char time[16], date[16], r[8];

  measureRoom(&temperature, &humidity); //misurazione ambiente

  if( millis() - timestamp >=1000ul ){ //se è passato un secondo
    timestamp = millis(); //aggiorna timestamp

    runCalendar(&now); //aggiorna data/ora
    dots = !dots; //lampeggio
    
    refresh = true; //aggiorna display
  }

  if (refresh){
    refresh = false;
    
    formatTime(time, now.h, now.m, now.s, dots);
    lcd.setCursor(11, 0);
    lcd.print(time);
    
    formatDate(date, now.y, now.M, now.d, now.dw, dots);
    lcd.setCursor(0, 1);
    lcd.print(date);
    
    switch (actMode) {
      case STD:
        formatRoom(r, temperature, humidity);
        lcd.setCursor(0, 0);
        lcd.print(r);
        break;
      case SETTIME:
        lcd.setCursor(0, 0);
        lcd.print("Set d/o   ");
        break;
      case SETPROG:
        lcd.setCursor(0, 0);
        lcd.print("Prog      ");
        break;
      default:
        break;
    }
  }
}
