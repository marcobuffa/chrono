//define to enable serial port initialization
//and basic operation log
//#define SERIALDEBUG

//Localization
#define LOC_EN

#ifdef LOC_IT
  #define DAY1 "LUN"
  #define DAY2 "MAR"
  #define DAY3 "MER"
  #define DAY4 "GIO"
  #define DAY5 "VEN"
  #define DAY6 "SAB"
  #define DAY7 "DOM"
#endif

#ifdef LOC_EN
  #define DAY1 "SUN"
  #define DAY2 "MON"
  #define DAY3 "TUE"
  #define DAY4 "WED"
  #define DAY5 "THU"
  #define DAY6 "FRI"
  #define DAY7 "SAT"
#endif

//Physical buttons debounce time (us)
//Debounce is micros() implemented
const unsigned long debounce = 200;
