#ifdef __cplusplus
extern "C" {
#endif

#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include "SWsettings.h"

enum set {MINUTE, HOUR, DAY, DAYOFWEEK, MONTH, YEAR};
enum mode {STD, SETTIME, SETPROG};

static const char dwn[7][3] = {DAY1, DAY2, DAY3, DAY4, DAY5, DAY6, DAY7};

typedef struct {
  int s;
  int m;
  int h;
  int d;
  int dw;
  int M;
  int y;
} datetype;

void formatTime(char *t, int h, int m, int s, int dots);
void formatDate(char *t, int y, int M, int d, int dw, int dots);
void runCalendar(datetype *now);
long int time2sec (datetype *now);
void sec2time (datetype *now, long int s);
void setTimeSwitch(enum set *toSet, datetype *now);


#endif //TIME_H_INCLUDED

#ifdef __cplusplus
} // extern "C"
#endif
