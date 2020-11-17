#ifdef __cplusplus
extern "C" {
#endif

#ifndef ISR_H_INCLUDED
#define ISR_H_INCLUDED

void mainMenuFSM(enum mode *actMode, enum set *toSet, enum progSet *toProg);

void setTimeSwitch(enum set *toSet, datetype *now);

#endif //ISR_H_INCLUDED

#ifdef __cplusplus
} // extern "C"
#endif
