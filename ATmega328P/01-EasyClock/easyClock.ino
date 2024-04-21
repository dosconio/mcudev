// utf-8
// Haruno RFR08

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "arduinoFFT.h"
#include <Keypad.h>
#include "RFR08.h"
#include "RFR08x.h"
// 2023/05/08 [A]*
// 19:06:00
//*: alerting


// A able alerm
// B set alarm
// C set date
// D set time

// #define setmin() LCDPrintString(16+3,nn)
// #define setmin() LCDPrintString(16+3,nn)
// #define setmin() LCDPrintString(16+3,nn)

void setup() {
  Serial.begin(9600);
  while (!Serial);
  lcd.init();
  lcd.backlight();
  flush_all();
}

void loop() {
  
  while(1)
  {
    if(RunMode) Runlp();
    	LCDPrintChar(16 + 2, ':');
	LCDPrintChar(16 + 5, ':');
  for(int i=0; i<20; i++)
    {
      delay(500/20);
      char key = keypad.getKey();
      switch(key)
      {
        case 'A':
          A_AbleAlert();
          break;
        case 'B':
          B_SetAlert();
          break;
        case 'C':
          C_SetDate();
          break;
        case 'D':
          D_SetTime();
          break;
        default:
          break;
      }
    }
    	LCDPrintChar(16 + 2, ' ');
	LCDPrintChar(16 + 5, ' ');
  for(int i=0; i<20; i++)
    {
      delay(500/20);
      char key = keypad.getKey();
      switch(key)
      {
        case 'A':
          A_AbleAlert();
          break;
        case 'B':
          B_SetAlert();
          break;
        case 'C':
          C_SetDate();
          break;
        case 'D':
          D_SetTime();
          break;
        default:
          break;
      }
    }

  // chk alerm
  if(a_enable)
    if(hh[0]==aahh[0] && hh[1]==aahh[1] &&
    nn[0]==aann[0] && nn[1]==aann[1]&&
    ss[0]==aass[0] && ss[1]==aass[1])
    {
      Serial.println("!!!");
      LCDPrintChar(15,'!');
    }

  }
}

