char yy[4+1]="2023";
char mm[2+1]="05";
char dd[2+1]="08";
char hh[2+1]="19";
char nn[2+1]="06";
char ss[2 + 1] = "00";

char aahh[2 + 1] = "19";
char aann[2 + 1] = "07";
char aass[2 + 1] = "10";


const char astr1[] = "[a]";
const char astr2[]="[A]";
bool RunMode = true;
// Alarm function
bool a_enable = false;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = { 2,3,4,5 };
byte colPins[COLS] = { 6,7,8,9 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
byte ledPin = 13; 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define setsec() LCDPrintString(16+6,ss)
#define setmin() LCDPrintString(16+3,nn)
#define setour() LCDPrintString(16+0,hh)

void LCDCursor(unsigned char posi)
{
	lcd.setCursor(posi % 16, posi / 16);
}

void LCDPrintChar(unsigned char posi, char chr)
{
	if (posi < 16 * 2)
	{
		LCDCursor(posi);
		lcd.print(chr);
	}
}

void LCDPrintString(unsigned char posi, char* str)
{
	if (!str) return;
	LCDCursor(posi);
	char c;
	lcd.print((const char[])str);
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


///
void StepDay()
{
	
}

void Runlp()
{
	ss[1]++; 
	if(ss[1]>'9') 
	{
		ss[0]++;
		ss[1] = '0';
		if (ss[0] >= '6')
		{
			nn[1]++;
			ss[0] = '0';
			if(nn[1]>'9')
			{
				nn[0]++;
				nn[1] = '0';
				if (nn[0] >= '6')
				{
					hh[1]++;
					nn[0] = '0';
					if (hh[1] > '9' || hh[1] == '4')
					{
						if (hh[0] == '2' && hh[1] == '4')
						{
							hh[0] = hh[1] = '0';
							StepDay();
						}
						else
						{
							hh[0]++;
							hh[1] = '0';
						}
					}
					setour();
				}
			}
			setmin();
		}
	}
	setsec();
	// LCDPrintChar(16 + 2, (ss[1] & 1) ? ':' : ' ');
}

void flush_all()
{
	const char ss[]{ "                                " };
	LCDPrintString(0, ss);
	LCDPrintString(0, yy);
	LCDPrintChar(4,'/');
	LCDPrintString(5,mm);
	LCDPrintChar(7,'/');
	LCDPrintString(8,dd);
	LCDPrintChar(10,' ');
	LCDPrintString(11, a_enable? astr2: astr1);
	LCDPrintString(16+0,hh);
	LCDPrintChar(16+2,':');
	LCDPrintString(16+3,nn);
	LCDPrintChar(16+5,':');
	LCDPrintString(16 + 6, ss);
}

