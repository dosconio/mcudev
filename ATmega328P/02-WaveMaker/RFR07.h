const byte ROWS = 4;
const byte COLS = 4;

# define SamLim (128+2)// Max of samp-p of a period
double vImag[SamLim];
double vReal[SamLim];
uint16_t samples = 128;// This value MUST be a power of 2.

char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
arduinoFFT FFT = arduinoFFT();

char _sim_scrn[16 * 2]{ "                " };
char _pre_scrn[16 * 2]{ "                " };
byte rowPins[ROWS] = { 2,3,4,5 };
byte colPins[COLS] = { 6,7,8,9 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
byte ledPin = 13; 
boolean blink = false;
boolean ledPin_state;
boolean IO = false;// 0: Output State; 1: Input State
boolean FFTFinOnce = false;

LiquidCrystal_I2C lcd(0x27,16,2);

unsigned char cur = 0;
long long int rate = 2000;// 100;// signalFrequency, f=1/T
long long int midvar = 50,
  Avar = 50;// Amplitude
long long int samRate = 4500;
long long int samTimes = 20;

enum stt_out
{
	NOP=0, NOPW, RECT, RECTW, SIN, SINW, ANG, ANGW// till 7
} outst = NOP;
enum stt_in
{
	A_amp, B_mid, C_frq, D_sar
} inst = A_amp;

const char* shape_names[8]
{
	"NOP", "NOPW", "RECT", "RECTW", "SIN", "SINW", "ANG", "ANGW"
};

// sin((i * (twoPi * cycles)) / samples)
// i * 2p * cycles / samples : 2p
// RECT: i * 1 * cycles / samples / rate : 1/rate
double sin_rect(double x)
{
  if(x<0) x=-x;
  while(x>=2*3.1416) x-=2*3.1416;
  return (x<=3.1416)?1:0;
}

double sin_angle(double x)
{
  if(x<0) x=-x;
  while(x>=2*3.1416) x-=2*3.1416;
  return (x<=3.1416)?(1.-2.*x/3.1416):(-1.+2.*x/3.1416);
}


void LCDPrintChar(unsigned char posi, char chr)
{
	if (posi < 16 * 2)
	{
		_sim_scrn[posi] = chr;
		lcd.setCursor(posi % 16, posi / 16);
		lcd.print(chr);
	}
	lcd.setCursor(posi % 16, posi / 16);
}

void LCDPrintString(unsigned char posi, char* str)
{
	if (!str) return;
	lcd.setCursor(posi % 16, posi / 16);
	char c;
	lcd.print((const char[])str);
	lcd.setCursor(posi % 16, posi / 16);
	while (posi < 16 * 2 && (c = *str))
  {
    _sim_scrn[posi] = c;
    posi++;
    str++;
  }
}

void LCDClearTop(void)
{
	for (unsigned char i = 16 * 0; i < 16 * 1; i++)
		LCDPrintChar(i, ' ');
}

void LCDClearBtm(void)
{
	for (unsigned char i = 16 * 1; i < 16 * 2; i++)
		LCDPrintChar(i, ' ');
}

void LCDClear(void)
{
	for (unsigned char i = 0; i < 16 * 2; i++)
		LCDPrintChar(i, ' ');
}

void echo() {
  Serial.println("");
  for (int i = 16 * 0; i < 16 * 1; i++) Serial.print(_sim_scrn[i]);
  Serial.println("");
  for (int i = 16 * 1; i < 16 * 2; i++) Serial.print(_sim_scrn[i]);
  Serial.println("");
}

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      //if (key == '#')
      break;
    case RELEASED:
      break;
    case HOLD:
      break;
  }
}

void LCDCursor(unsigned char posi)
{
	lcd.setCursor(posi % 16, posi / 16);
	lcd.blink();
}

void EnterEdit()
{
	IO = true;
	cur = 16 + 3;
	LCDCursor(cur);
	// lcd.blink();
	LCDClearBtm();
	long long int crt = (inst == A_amp) ? Avar : (inst == B_mid) ? midvar :
		(inst == C_frq) ? rate : samRate;
	int crtcur = 16 + 3 + 9;
	for (int i = 0; i < 10; i++)
	{
		LCDPrintChar(crtcur, 0x30 + crt % 10);
		crt /= 10;
		crtcur--;
	}
}

char LCDScanChar(unsigned char posi)
{
	if (posi < 16 * 2) return _sim_scrn[posi];
	return ' ';
}

void ApplyEdit()
{
	int crtcur = 16 + 3;
	((inst == A_amp) ? Avar : (inst == B_mid) ? midvar :
		(inst == C_frq) ? rate : samRate) = 0;
	for (int i = 0; i < 10; i++)
	{
		((inst == A_amp) ? Avar : (inst == B_mid) ? midvar :
			(inst == C_frq) ? rate : samRate) *= 10;
		((inst == A_amp) ? Avar : (inst == B_mid) ? midvar :
			(inst == C_frq) ? rate : samRate) += LCDScanChar(crtcur) - 0x30;
		crtcur++;
	}
}

void PrintVector(double *vData, uint16_t bufferSize)// Na w dime
{
  vData[0] = 0;
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa = ((i * 1.0 * samRate) / samples);
    Serial.print(abscissa, 6);
    Serial.print("Hz ");
    Serial.println(vData[i], 4);
  }
}

char FunDrawRect()
{
	long long int picsamRate = rate / samTimes;
	for (int i = 0; i < picsamRate / 2; i++)
    {
      Serial.println((double)(midvar+Avar));
	  delay(1000 / picsamRate);
	  char key = keypad.getKey();
	  if (key) return key;
	}
    for(int i=0; i<picsamRate/2; i++)
    {
      Serial.println((double)(midvar-Avar));
	  delay(1000 / picsamRate);
		  char key = keypad.getKey();
	  if (key) return key;
	}
	return 0;
}

char FunDrawRectW()
{
	if (FFTFinOnce) return 0;
	FFTFinOnce = true;
	double cycles = (((samples-1) * rate) / samRate);
	for (uint16_t i = 0; i < samples; i++)
	{
		vReal[i] = ((double)(Avar * (sin_rect((i * (twoPi * cycles)) / samples))) / 2.0);
		vImag[i] = 0.;
	}
	FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
	FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
	FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
	PrintVector(vReal, (samples >> 1));
	return 0;
}

char FunDrawSin()
{
	long long int picsamRate = rate / samTimes;
    for(int i=0; i<picsamRate; i++)
    {
      Serial.println((double)sin(i*3.1416*2/picsamRate)*Avar+midvar);
	  delay(1000 / picsamRate);
		  char key = keypad.getKey();
	  if (key) return key;
	}
	return 0;
}

char FunDrawSinW()
{
	if (FFTFinOnce) return 0;
	FFTFinOnce = true;
	double cycles = (((samples-1) * rate) / samRate);
	for (uint16_t i = 0; i < samples; i++)
	{
		vReal[i] = ((double)(Avar * (sin((i * (twoPi * cycles)) / samples))) / 2.0);
		vImag[i] = 0.;
	}
	FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
	FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
	FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
	PrintVector(vReal, (samples >> 1));
	return 0;
}

char FunDrawArg()
{
	long long int picsamRate = rate / samTimes;
	for (int i = 0; i < picsamRate / 2; i++)
    {
      if(i==picsamRate/2) Serial.println((double)midvar-Avar);
      else Serial.println((double)midvar+Avar-i*(2*Avar)/(picsamRate/2));
	  delay(1000 / picsamRate);
		  char key = keypad.getKey();
	  if (key) return key;
	}
    for(int i=0; i<picsamRate/2; i++)
    {
      if(i==picsamRate/2) Serial.println((double)midvar+Avar);
      else Serial.println((double)midvar-Avar+i*(2*Avar)/(picsamRate/2));
	  delay(1000 / picsamRate);
		  char key = keypad.getKey();
	  if (key) return key;
	}
	return 0;
}

char FunDrawArgW()
{
	if (FFTFinOnce) return 0;
	FFTFinOnce = true;
	double cycles = (((samples-1) * rate) / samRate);
	for (uint16_t i = 0; i < samples; i++)
	{
		vReal[i] = (((double)Avar * (sin_angle((i * (twoPi * cycles)) / samples))) / 2.0);
		vImag[i] = 0.;
	}
	FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
	FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
	FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
	PrintVector(vReal, (samples >> 1));
	return 0;
}

char (*Funcs[8]) (void)
{
	0, 0, FunDrawRect, FunDrawRectW,
	FunDrawSin, FunDrawSinW, FunDrawArg, FunDrawArgW
};
