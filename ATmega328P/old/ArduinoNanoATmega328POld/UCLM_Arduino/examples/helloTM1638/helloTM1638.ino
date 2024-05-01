
#include "Arduino.h"
#include "UCLM_Arduino.h"

void setup()
{
	
  TM1638 tm(6,5,4);
  Serial.begin(9600);
  uint32_t i = 0;
	while(1)
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
