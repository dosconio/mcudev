//
#include "Arduino.h"
#include "UCLM_MSP432P401R.h"

#define SetPinMod(x,y) pinMode((x),(y))
// Meaningless, for debug.

static char uclm[] = { "UCLM@D.O.S.C.O.N.I.O." };

const static int ledBins[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

// to fit the clock?
static uint8_t TM1638CLK;
static uint8_t TM1638STB;
static uint8_t TM1638DIO;

// TM1638 //
static void TM1638WriteData(uint8_t value)
{
	// bit by bit from low.
	// how to bind this with class? By using "friend"? --RFR21 HRN.
	SetPinMod(TM1638DIO, OUTPUT);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(TM1638CLK, LOW);
		digitalWrite(TM1638DIO, value & 1 ? HIGH : LOW);
		value >>= 1;// SAR? --RFR15
		digitalWrite(TM1638CLK, HIGH);
	}
	digitalWrite(TM1638CLK, LOW);
}

static void TM1638WriteCmd(uint8_t value)
{	
	digitalWrite(TM1638STB, LOW);
	TM1638WriteData(value);
	digitalWrite(TM1638STB, HIGH);
}

static uint8_t TM1638ScanfDIO()
{
	// read a byte from DIO.
	uint8_t value = 0;
	SetPinMod(TM1638DIO, INPUT);// set input mode.
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(TM1638CLK, LOW);
		delay(1);// avoid short shock?
		if (digitalRead(TM1638DIO) == HIGH) value |= 1 << i;
		digitalWrite(TM1638CLK, HIGH);
	}
	digitalWrite(TM1638CLK, LOW);
	SetPinMod(TM1638DIO, OUTPUT);// set output mode.
	return value;
}

TM1638::TM1638(uint8_t STB, uint8_t CLK, uint8_t DIO)
{
	SetPinMod(TM1638STB = STB, OUTPUT);
	SetPinMod(TM1638CLK = CLK, OUTPUT);
	SetPinMod(TM1638DIO = DIO, OUTPUT);
	TM1638WriteCmd(0x8b); // set light, 0x88-0x8f
	TM1638WriteCmd(0x40); // set auto increment mode
	digitalWrite(TM1638STB, LOW);
	TM1638WriteData(0xc0);   // set starting address to 0
	for (uint8_t i = 0; i < 16; i++)
		TM1638WriteData(0x00);
	digitalWrite(TM1638STB, HIGH);
}

uint8_t TM1638::btn(void)
{
	//ScanfButtons
	uint8_t buttons = 0;
	digitalWrite(TM1638STB, LOW);
	SetPinMod(TM1638DIO, OUTPUT);
	shiftOut(TM1638DIO, TM1638CLK, LSBFIRST, 0x42);
	SetPinMod(TM1638DIO, INPUT);
	for (uint8_t i = 0; i < 4; i++)// Why 4?
	{
		uint8_t v = TM1638ScanfDIO() << i;
		buttons |= v;
	}
	SetPinMod(TM1638DIO, OUTPUT);
	digitalWrite(TM1638STB, HIGH);
	return buttons;
}

void TM1638::nixDec(uint32_t number)
{
	int pos = 8;
	while (pos > 0) {
		digitalWrite(TM1638STB, LOW);
		TM1638WriteData(0xc0 + (pos - 1) * 2);
		if (number > 0) {
			uint32_t dig = number % 10;
			TM1638WriteData(ledBins[dig]);
			number /= 10;
		}
		else if (pos == 8){
			TM1638WriteData(ledBins[0]);
		} else TM1638WriteData(0);
		digitalWrite(TM1638STB, HIGH);
		pos--;
	}
}

void TM1638::led(uint8_t state)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		SetPinMod(TM1638DIO, OUTPUT);
		TM1638WriteCmd(0x44);
		digitalWrite(TM1638STB, LOW);
		TM1638WriteData(0xC1 + (i << 1));
		TM1638WriteData(state & 1);
		state >>= 1;
		digitalWrite(TM1638STB, HIGH);
	}
}