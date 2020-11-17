#include "room.h"
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
