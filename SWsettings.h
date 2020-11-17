#ifndef SWSETTINGS_H_INCLUDED
#define SWSETTINGS_H_INCLUDED

//define to enable serial port initialization
//and basic operation log
//#define SERIALDEBUG

//Localization
#define LOC_IT

#ifdef LOC_IT
  #define DAY1 "LUN"
  #define DAY2 "MAR"
  #define DAY3 "MER"
  #define DAY4 "GIO"
  #define DAY5 "VEN"
  #define DAY6 "SAB"
  #define DAY7 "DOM"
#endif //LOC_IT

#ifdef LOC_EN
  #define DAY1 "SUN"
  #define DAY2 "MON"
  #define DAY3 "TUE"
  #define DAY4 "WED"
  #define DAY5 "THU"
  #define DAY6 "FRI"
  #define DAY7 "SAT"
#endif //LOC_EN

//Physical buttons debounce time (us)
//Debounce is micros() implemented
#define DEBOUNCE 200000

#endif //SWSETTINGS_H_INCLUDED
