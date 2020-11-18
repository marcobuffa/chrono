#include "time.h"
#include "program.h"

extern int actPset;
extern int actDWset;
extern int clean;

void mainMenuFSM(enum mode *actMode, enum set *toSet, enum progSet *toProg) {
  switch (*actMode) {
    
    case STD:
      *actMode = SETTIME;
      clean = 1;
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
          clean = 1;
          break;
      }
      break;
      
    case SETPROG:
      switch (*toProg) {
        case ENABLED:
          *toProg = STARTHOUR;
          break;
        case STARTHOUR:
          *toProg = STARTMINUTE;
          break;          
        case STARTMINUTE:
          *toProg = STOPHOUR;
          break;
        case STOPHOUR:
          *toProg = STOPMINUTE;
          break;
        case STOPMINUTE:
          *toProg = TEMP;
          break;
        case TEMP:
          *toProg = ENABLED;
          if (actPset<2){
            actPset++;
          } else {
            actPset=0;
            if (actDWset<7){
              actDWset++;
            } else {
              actDWset=1;
              *actMode = STD;
              clean = 1;
            }
          }
          break;
        default:
          *toProg = ENABLED;
          break;
      }
      break;
      
    default:
      *actMode = STD;
      clean = 1;
      break;
  }
}
