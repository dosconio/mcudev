#ifndef _KEY_INC_
#define _KEY_INC_

#include "MCU/ST/STM32F103VEx"

//{TODO} class UnaryKey

#define GPIO_KEY_1 0
#define KEY_1_PORT 'A'                      

#define GPIO_KEY_2 13
#define KEY_2_PORT 'C'

void KEY_Initialize();

extern uni::GPIO_Pin& KEY_1_REF();

extern uni::GPIO_Pin& KEY_2_REF();

// If key is pressed, this will wait for it up;
// `neglogic` means when the key down, the pin will get D'DP.
bool KEY_Scanwait(uni::GPIO_Pin& key, bool neglogic = false);

#endif
