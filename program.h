#ifdef __cplusplus
extern "C" {
#endif

#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

enum progSet {STARTMINUTE, STARTHOUR, STOPMINUTE, STOPHOUR, WEEKDAY, TEMP, ENABLED};

typedef struct {
  long int start;
  long int stop;
  int dw;
  float temperature; 
  int enabled;
} onInterval;

#endif //PROGRAM_H_INCLUDED

#ifdef __cplusplus
} // extern "C"
#endif
