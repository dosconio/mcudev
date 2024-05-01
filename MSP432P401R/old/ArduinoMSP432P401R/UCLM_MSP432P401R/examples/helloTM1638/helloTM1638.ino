
#include "Arduino.h"
#include "UCLM_MSP432P401R.h"

void setup()
{
	TM1638 tm(P6_1, P4_0, P4_2);// STB, CLK, DIO
	Serial.begin(9600);
	uint32_t i = 0;
	while (1)
	{
		unsigned char s = tm.btn();
		tm.led(s);
		tm.nixDec(i++);
		delay(10);
	}
}

void loop()
{
  
}
