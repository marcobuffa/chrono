#ifdef __cplusplus
extern "C" {
#endif

#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

enum progSet {STARTMINUTE, STARTHOUR, STOPMINUTE, STOPHOUR, TEMP, ENABLED};

typedef struct {
  long int start;
  long int stop;
  int dw;
  float temperature; 
  int enabled;
} onInterval;

typedef struct {
  onInterval interval[3];
} dailyProg;

typedef struct {
  dailyProg day[7];
} weeklyProg;

void formatDW(char *t, int dw);
void formatProgTimes(char *t, int actPset, onInterval *actInterval, int dots);
void formatEn(char *t, onInterval *actInterval, int dots);
void formatTemp(char *t, onInterval *actInterval, int dots);
void setProgSwitch(enum progSet *toProg, onInterval *actInterval);

#endif //PROGRAM_H_INCLUDED

#ifdef __cplusplus
} // extern "C"
#endif
