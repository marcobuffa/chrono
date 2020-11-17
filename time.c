#include "time.h"

extern enum set toSet;
extern enum mode actMode;
const char dwn[7][3] = {"LUN", "MAR", "MER", "GIO", "VEN", "SAB", "DOM"};

//formattazione ora
void formatTime(char *t, int h, int m, int s, int dots){
  char dd;
  dd = dots?':':' ';

  if (actMode==SETTIME) {
    switch (toSet) {
      case HOUR:
        dots ? sprintf(t, "%02d%c%02d", h, dd, m) : 
                sprintf(t, "  %c%02d", dd, m);
        break;          
      case MINUTE:
        dots ? sprintf(t, "%02d%c%02d", h, dd, m) : 
                sprintf(t, "%02d%c  ", h, dd);
        break;
      default:
        sprintf(t, "%02d%c%02d", h, dd, m);
        break;
    }
  } else {
    sprintf(t, "%02d%c%02d", h, dd, m);
  }
  return;
}

//formattazione data
void formatDate(char *t, int y, int M, int d, int dw, int dots){
  if (actMode==SETTIME) {
    switch (toSet) {
      case YEAR:
        dots ? sprintf(t, "%02d/%02d/%04d %c%c%c", d, M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]) :
                sprintf(t, "%02d/%02d/     %c%c%c", d, M , dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]);
        break;
      case MONTH:
        dots ? sprintf(t, "%02d/%02d/%04d %c%c%c", d, M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]) :
                sprintf(t, "%02d/  /%04d %c%c%c", d, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]);
        break;
      case DAY:
        dots ? sprintf(t, "%02d/%02d/%04d %c%c%c", d, M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]) :
                sprintf(t, "  /%02d/%04d %c%c%c", M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]); 
        break; 
      case DAYOFWEEK:
        dots ? sprintf(t, "%02d/%02d/%04d %c%c%c", d, M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]) :
                sprintf(t, "%02d/%02d/%04d    ", d, M, y);
        break;
      default:
        sprintf(t, "%02d/%02d/%04d %c%c%c", d, M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]);
        break;
    }
  } else {
    sprintf(t, "%02d/%02d/%04d %c%c%c", d, M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]);
  }
  return;
}

//aggiornamento calendario (1s)
void runCalendar(date *now){
  int pos=0;

  now->s++;
  if (now->s>59){
    now->s=0;
    now->m++;
    if (now->m>59){
      now->m=0;
      now->h++;
      if (now->h>23){
        now->h=0;
        now->d++;
        now->dw++;
        now->dw=now->dw > 7 ? 1 : now->dw;
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
      }
    }
  }
  
  if (pos==2){
    now->d=1;
    now->M++;
    if (now->M>12){
      now->M=1;
      now->y++;
    }
  }
}
