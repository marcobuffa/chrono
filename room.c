#include "room.h"

void formatRoom(char *r, float t, float rh){
  sprintf(r, "%dC %d%%", (int)t, (int)rh);
  return;
}
