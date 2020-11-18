#include "program.h"
#include "SWsettings.h"
#include "time.h"

extern enum set toProg;
extern enum mode actMode;

//-------------------------------------
//day of week string formatting routine
//-------------------------------------
void formatDW(char *t, int dw){
  sprintf(t, "%c%c%c", dwn[dw-1][0], dwn[dw-1][1], dwn[dw-1][2]);
  return;
}

void formatProgTimes(char *t, int actPset, onInterval *actInterval, int dots){
  datetype setStart, setStop;
  sec2time(&setStart, actInterval->start);
  sec2time(&setStop, actInterval->stop);
   
  //if programming mode, enable digit flashing using dots as pacemaker
  if (actMode==SETPROG) {
    //depending on currently setting time field
    switch (toProg) {
      case STARTHOUR:
        dots ? sprintf(t, "P%d:   :%02d->%02d:%02d", actPset+1, setStart.m, setStop.h, setStop.m) : 
                sprintf(t, "P%d: %02d:%02d->%02d:%02d", actPset+1, setStart.h, setStart.m, setStop.h, setStop.m);
        break;
      case STARTMINUTE:
        dots ? sprintf(t, "P%d: %02d:  ->%02d:%02d", actPset+1, setStart.h, setStop.h, setStop.m) : 
                sprintf(t, "P%d: %02d:%02d->%02d:%02d", actPset+1, setStart.h, setStart.m, setStop.h, setStop.m);
        break;
      case STOPHOUR:
        dots ? sprintf(t, "P%d: %02d:%02d->  :%02d", actPset+1, setStart.h, setStart.m, setStop.m) : 
                sprintf(t, "P%d: %02d:%02d->%02d:%02d", actPset+1, setStart.h, setStart.m, setStop.h, setStop.m);
        break;
      case STOPMINUTE:
        dots ? sprintf(t, "P%d: %02d:%02d->%02d:  ", actPset+1, setStart.h, setStart.m, setStop.h) : 
                sprintf(t, "P%d: %02d:%02d->%02d:%02d", actPset+1, setStart.h, setStart.m, setStop.h, setStop.m);
        break;
      default:
        sprintf(t, "P%d: %02d:%02d->%02d:%02d", actPset+1, setStart.h, setStart.m, setStop.h, setStop.m);
        break;
    }
  } else { //if standard operating mode
    ssprintf(t, "P%d: %02d:%02d->%02d:%02d", actPset+1, setStart.h, setStart.m, setStop.h, setStop.m);
  }
        

  
  
  return;



  
}
