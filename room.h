#ifdef __cplusplus
extern "C" {
#endif

#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "program.h"
#include "time.h"

void formatRoom(char *r, float t, float rh);
int shouldHeat(float actTemperature, datetype *now, onInterval *fullProg);

#endif //ROOM_H_INCLUDED

#ifdef __cplusplus
} // extern "C"
#endif
