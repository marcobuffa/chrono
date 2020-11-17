#ifdef __cplusplus
extern "C" {
#endif

void mainMenuFSM(enum mode *actMode, enum set *toSet);

void setTimeSwitch(enum set *toSet, date *now);

#ifdef __cplusplus
} // extern "C"
#endif
