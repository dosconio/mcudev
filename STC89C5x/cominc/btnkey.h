#ifndef DevKey
#define DevKey

extern const unsigned char BtnkeyCount;// user do not define
extern unsigned char btnkey;
extern void (*EventBtnkeyDown)(unsigned char ki), (*EventBtnkeyUp)(unsigned char ki);
extern void (*EventBtnkeyEvent)(unsigned char ki);

unsigned char Btnkey_Scan();// is going to use multi-button-at-a-time

unsigned char BtnkeyScan();// is going to use multi-button-at-a-time
void BtnkeyLoop();

#endif
