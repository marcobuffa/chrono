#include "time.h"

void mainMenuFSM(enum mode *actMode, enum set *toSet) {
  switch (*actMode) {
    case STD:
      *actMode = SETTIME;
      break;
    case SETTIME:
      switch (*toSet) {
        case HOUR:
          *toSet = MINUTE;
          break;          
        case MINUTE:
          *toSet = YEAR;
          break;
        case YEAR:
          *toSet = MONTH;
          break;            
        case MONTH:
          *toSet = DAY;
          break;
        case DAY:
          *toSet = DAYOFWEEK;
          break;
        case DAYOFWEEK:
          *toSet = HOUR;
          *actMode = SETPROG;
        default:
          *toSet = HOUR;
          *actMode = SETPROG;
          break;
      }
      break;
    case SETPROG:
      *actMode = STD;
      break;
    default:
      *actMode = STD;
      break;
  }
}
