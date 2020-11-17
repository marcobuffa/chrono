#ifdef __cplusplus
extern "C" {
#endif

enum set {MINUTE, HOUR, DAY, DAYOFWEEK, MONTH, YEAR};
enum mode {STD, SETTIME, SETPROG};

typedef struct {
  int s;
  int m;
  int h;
  int d;
  int dw;
  int M;
  int y;
} date;

void formatTime(char *t, int h, int m, int s, int dots);
void formatDate(char *t, int y, int M, int d, int dw, int dots);
void runCalendar(date *now);

#ifdef __cplusplus
} // extern "C"
#endif
