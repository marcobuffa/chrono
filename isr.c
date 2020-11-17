#include "time.h"
#include "program.h"

extern int actPset;
extern int actDWset;

void mainMenuFSM(enum mode *actMode, enum set *toSet, enum progSet *toProg) {
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
          *toProg = WEEKDAY;
          break;
        case WEEKDAY:
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
      break;
  }
}

void setTimeSwitch(enum set *toSet, datetype *now) {
  int pos=0;
  
  switch (*toSet) {
    case HOUR:
      now->h>=23 ? now->h=0 : now->h++;
      break;         
    case MINUTE:
      now->m>=59 ? now->m=0 : now->m++;
      break;
    case YEAR:
      now->M=1;
      now->d=1;
      now->y>=2099 ? now->y=2020 : now->y++;
      break;
    case MONTH:
      now->d=1;
      now->M>=12 ? now->M=1 : now->M++;
      break;
    case DAY:
      now->d++;
      switch (now->M){
        case 2: //febrary
          if ((now->y % 4)==0){ //if leap year
            pos=now->d > 29 ? 2 : pos; //mark for month change after 29 days
          } else { //if normal year
            pos=now->d > 28 ? 2 : pos; //mark for month change after 28 days
          }
          break;
        case 4: //april
        case 6: //june
        case 9: //september
        case 11: //november
          pos=now->d > 30 ? 2 : pos; //mark for month change after 30 days
          break;
        default: //january, march, may, july, august, october, december
          pos=now->d > 31 ? 2 : pos; //mark for month change after 31 days            
          break;
      }
      if (pos==2){
        now->d=1;
      }
      break;
    case DAYOFWEEK:
      now->dw>=7 ? now->dw=1 : now->dw++;
      break;
    default:
      break;
  }
}
