#ifndef ModMCLMArduino
#define ModMCLMArduino
//AVOID CONFLICITING WITH DIFFERENT BOARD.

// 1 3V3
#define P6_0 2
#define P3_2 3
#define P3_3 4
#define P4_1 5
#define P4_3 6
#define P1_5 7
#define P4_6 8
#define P6_5 9
#define P6_4 10
//-------------


#define P3_6 11
#define P5_2 12
#define P5_0 13 
#define P1_7 14
#define P1_6 15
// 16 RESET
#define P5_7 17
#define P3_0 18
#define P2_5 19
// 20 GND

/* pins 41-56 */
#define P8_5 41
#define P9_0 42
#define P8_4 43
#define P8_2 44
#define P9_2 45
#define P6_2 46
#define P7_3 47
#define P7_1 48
#define P9_4 49
#define P9_6 50
#define P8_0 51
#define P7_4 52
#define P7_6 53
#define P10_0 54
#define P10_2 55
#define P10_4 56

#define P8_6 57
#define P8_7 58
#define P9_1 59
#define P8_3 60
#define P5_3 61
#define P9_3 62
#define P6_3 63 
#define P7_2 64
#define P7_0 65
#define P9_5 66
#define P9_7 67
#define P7_5 68
#define P7_7 69
#define P10_1 70
#define P10_3 71
#define P10_5 72
#define P1_1 73
#define P1_4 74
#define P2_0 75
#define P2_1 76
#define P2_2 77
#define P1_0 78

//#define LED_R 75
//#define LED_G 76
//#define LED_B 77
//#define LEB_Y 78
//#define LED1 78
//#define PUSH1 73
//#define PUSH2 74

//#define A0 30
#define P5_5 30

//#define A1 29
#define P5_4 29

//#define A2 61
//#define A3 12
//#define A4 33
//#define A5 13

//#define A6 28
#define P4_7 28

//#define A7 8
//#define A8 27
//#define A9 26
//#define A10 6

//#define A11 25
#define P4_2 25

//#define A12 5

//#define A13 24
#define P4_0 24

//#define A14 23
#define P6_1 23

//#define A15 2
//#define A16 59
//#define A17 42
//#define A18 58
//#define A19 57
//#define A20 41
//#define A21 43
//#define A22 69
//#define A23 44

#include <stdint.h>
class TM1638
{
public:
	TM1638(uint8_t STB, uint8_t CLK, uint8_t DIO);
	uint8_t btn(void);
	void nixDec(uint32_t number);
	void led(uint8_t state);
private:;
};

#endif
