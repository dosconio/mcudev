// @ Dosho Haruno, RFE23
// using <arduinoFFT> 1.6 Ver.
// using <Keypad> 3.1.1 Ver.
// 因为LCD模块坏了，所以采取封装编程 QwQ
// UTF-8
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "arduinoFFT.h"
#include <Keypad.h>
#include "RFR07.h"

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(ledPin, OUTPUT);               // Sets the digital pin as output.
  digitalWrite(ledPin, HIGH);            // Turn the LED on.
  ledPin_state = digitalRead(ledPin);    // Store initial LED state. HIGH when LED is on.
  keypad.addEventListener(keypadEvent);  // Add an event listener for this keypad
  // LCDPrintString(1, "Hello Arduino Nano~ QwQ~");
  lcd.init();
  lcd.backlight();
  LCDPrintString(3, "READY");
  echo();
  lcd.blink();
}

void loop() {
  char c = 0;
  while (1) {
    lpchk(&c);
    // input or graphic output
    if (!IO)  // draw
    {
      if (Funcs[outst]) c = Funcs[outst]();
    }
    // delay(100);
  }
}

void lpchk(char* chr) {
  char key = *chr ? *chr : keypad.getKey();
  *chr = 0;
  if (key) {
    if (IO) {
      switch (key) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          LCDPrintChar(cur, key);
          cur++;
          if (cur >= 16 + 3 + 10) cur = cur = 16 + 3;
          LCDCursor(cur);
          break;
        case '*':
          // apply
          ApplyEdit();
          outst = NOP;
		  IO = false;
		  //lcd.noBlink();
		  LCDClearBtm();
          break;
        case '#':
          outst = NOP;
		  IO = false;
		//lcd.noBlink();
		  LCDClearBtm();
          // cancel, back output
          break;
        case 'A':
          ApplyEdit();
          outst = NOP;
		  IO = false;
		//lcd.noBlink();
		  LCDClearBtm();
          inst = A_amp;
          EnterEdit();
          break;
        case 'B':
          ApplyEdit();
          outst = NOP;
		  IO = false;
		//lcd.noBlink();
		  LCDClearBtm();
          inst = B_mid;
          EnterEdit();
          break;
        case 'C':
          ApplyEdit();
          outst = NOP;
		  IO = false;
		//lcd.noBlink();
		  LCDClearBtm();
          inst = C_frq;
          EnterEdit();
          break;
        case 'D':
          ApplyEdit();
          outst = NOP;
		  IO = false;
		//lcd.noBlink();
		  LCDClearBtm();
          inst = D_sar;
          EnterEdit();
          break;
        default:
          break;
      }
    } else  // out mode
    {
      switch (key) {
        case 'A':
          inst = A_amp;
          EnterEdit();
          break;
        case 'B':
          inst = B_mid;
          EnterEdit();
          break;
        case 'C':
          inst = C_frq;
          EnterEdit();
          break;
        case 'D':
          inst = D_sar;
          EnterEdit();
          break;
        case '*':
          // shape change
          outst = outst + 2;
          if (outst == 8) outst = NOP;
          else if (outst > 8) outst = NOPW;
          FFTFinOnce = false;
          LCDClearTop();
          LCDPrintString(6, shape_names[outst]);
          break;
        case '#':
          // mode change
          if (outst & 1) outst = (outst >> 1) << 1;
          else outst |= 1;
          FFTFinOnce = false;
          LCDClearTop();
          LCDPrintString(6, shape_names[outst]);
          break;
        default:
          break;
      }
    }
    // LCDPrintChar(16 * 2 - 1, key);
    // echo();
  }
}
