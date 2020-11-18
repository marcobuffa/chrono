#include "room.h"
#include "time.h"
#include "program.h"
#include "SWsettings.h"

//-----------------------------------
//room measurement formatting routine
//-----------------------------------
void formatRoom(char *r, float t, float rh){

#ifdef LOC_EN
  t = t*9.0/5.0+32.0; //Fahreneit conversion
  sprintf(r, "%dF %d%%", (int)t, (int)rh);
#else
  sprintf(r, "%dC %d%%", (int)t, (int)rh);
#endif
  
  return;
}

int shouldHeat(float actTemperature, datetype *now, onInterval fullProg[7][3]){
  long int sNow = time2sec(now);
  int on, i, found = 0;
  
  //check which program is currently the right one
  for (i=0; i<=2; i++){
    if (fullProg[now->dw][i].start<=sNow && fullProg[now->dw][i].stop>sNow){ //if now is an interval of the current day of week
      if (fullProg[now->dw][i].enabled){ //if the interval is active
        found = 1;
        if (actTemperature<fullProg[now->dw][i].temperature){ //if it's colder that the required temperature
          on = 1; //enable heater system
        } else if (actTemperature>=fullProg[now->dw][i].temperature+HIST){ //histeresys
          on = 0;
        }
      }
    }
  }
  
  if (found){
    return (on);
  } else {
    return (0);
  }
}
