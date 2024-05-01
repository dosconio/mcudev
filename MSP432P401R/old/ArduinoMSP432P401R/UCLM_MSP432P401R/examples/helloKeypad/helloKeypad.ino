#include <Keypad.h>
#include <UCLM_MSP432P401R.h>
// Harunosan yo RFT10 for MSP432P401R

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
byte rowPins[ROWS] = { P6_0, P3_2, P3_3, P4_1 };
byte colPins[COLS] = { P4_3, P1_5, P4_6, P6_5 };
// P6.0 is close to '*' and P6.5 to 'D'
Keypad kpad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
TM1638 tm(P6_1, P4_0, P4_2);// STB, CLK, DIO

void setup()
{
  
}

void loop()
{ 
  char c;
  tm.nixDec(20230610);
  while(1)
  {
    c = kpad.getKey();
    if(c && c>='0' && c<='9')
    {
      tm.nixDec(c-'0');
    }
    delay(50);
  }
}
