#include "room.h"

//-----------------------------------
//room measurement formatting routine
//-----------------------------------
void formatRoom(char *r, float t, float rh){
  sprintf(r, "%dC %d%%", (int)t, (int)rh); //sorry, no Fahreneit/Kelvin unit at the moment
  return;
}
