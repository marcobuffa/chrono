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

void setTimeSwitch(enum set *toSet, date *now) {
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
      now->d=now->d+1;
      if (now->M==2){
        if ((now->y % 4)==0){
          pos=now->d > 29 ? 2 : pos;
        } else {
          pos=now->d > 28 ? 2 : pos;
        }
      } else {
        if ((now->M==1) || (now->M==3) || (now->M==5) || (now->M==7)
              || (now->M==8) || (now->M==10) || (now->M==12)){
          pos=now->d > 31 ? 2 : pos;
        } else {
          pos=now->d > 30 ? 2 : pos;
        }
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
