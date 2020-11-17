#ifdef __cplusplus
extern "C" {
#endif

#ifndef ISR_H_INCLUDED
#define ISR_H_INCLUDED

void mainMenuFSM(enum mode *actMode, enum set *toSet);

void setTimeSwitch(enum set *toSet, date *now);

#endif //ISR_H_INCLUDED

#ifdef __cplusplus
} // extern "C"
#endif
