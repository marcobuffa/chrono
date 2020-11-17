#include "time.h"

extern enum set toSet;
extern enum mode actMode;
const char dwn[7][3] = {"LUN", "MAR", "MER", "GIO", "VEN", "SAB", "DOM"};

//------------------------------
//time string formatting routine
//------------------------------
void formatTime(char *t, int h, int m, int s, int dots){
  char dd;
  dd = dots?':':' '; //flashing colon between hours and minutes

  //if time & date setting mode, enable digit flashing using dots as pacemaker
  if (actMode==SETTIME) {

    //depending on currently setting time field
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
  } else { //if standard operating mode
    sprintf(t, "%02d%c%02d", h, dd, m); //normal formatting with no flash
  }
  
  return;
}

//------------------------------
//date string formatting routine
//------------------------------
void formatDate(char *t, int y, int M, int d, int dw, int dots){

  //if time & date setting mode, enable digit flashing using dots as pacemaker
  if (actMode==SETTIME) {

    //depending on currently setting date field
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
  } else { //if standard operating mode
    sprintf(t, "%02d/%02d/%04d %c%c%c", d, M, y, dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]); //normal formatting with no flash
  }
  return;
}

//--------------------------------------------------------------------------------------------
//main clock/calendar update (to be called exactly one time per second -> no RTC, be precise!)
//Yes, I know this is a bunch of bad code -> need time to rewrite it better
//--------------------------------------------------------------------------------------------
void runCalendar(date *now){
  int pos=0;

  now->s++; //update seconds
  if (now->s>59){ //if over 59
    now->s=0; //reset seconds
    now->m++; //update minutes
    if (now->m>59){ //if over 59
      now->m=0; //reset minutes
      now->h++; //update hours
      if (now->h>23){ //if over 23 (sorry, no AM/PM time notation)
        now->h=0; //reset hours
        now->d++; //update day
        now->dw++; //update week day
        now->dw=now->dw > 7 ? 1 : now->dw; //if weekday over 7, reset week day
        if (now->M==2){ //if february
          if ((now->y % 4)==0){ //if leap year
            pos=now->d > 29 ? 2 : pos; //mark for month change after 29 days
          } else { //if normal year
            pos=now->d > 28 ? 2 : pos; //mark for month change after 28 days
          }
        } else { //if not february
          if ((now->M==1) || (now->M==3) || (now->M==5) || (now->M==7)
                || (now->M==8) || (now->M==10) || (now->M==12)){ //if 31-days month
            pos=now->d > 31 ? 2 : pos; //mark for month change after 31 days
          } else { //if 30-days month
            pos=now->d > 30 ? 2 : pos; //mark for month change after 30 days
          }
        }
      }
    }
  }
  
  if (pos==2){ //if time to update month
    now->d=1; //reset day
    now->M++; //update month
    if (now->M>12){ //if month over 12
      now->M=1; //reset month
      now->y++; //update year
    }
  }
}
