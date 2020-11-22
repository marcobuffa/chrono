#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

enum progSet {STARTMINUTE, STARTHOUR, STOPMINUTE, STOPHOUR, TEMP, ENABLED};

typedef struct {
  long int start;
  long int stop;
  float temperature; 
  int enabled;
} onInterval;

void formatDW(char *t, int dw);
void formatProgTimes(char *t, int actPset, onInterval *actInterval, int dots);
void formatEn(char *t, onInterval *actInterval, int dots);
void formatTemp(char *t, onInterval *actInterval, int dots);
void setProgSwitch(enum progSet *toProg, onInterval *actInterval);

#endif //PROGRAM_H_INCLUDED
