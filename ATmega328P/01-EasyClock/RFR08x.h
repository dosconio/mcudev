const char str3[] = "SET ALERM:";
const char str4[] = "SET DATE:";
const char str5[] = "SET TIME:";

void A_AbleAlert()
{
	a_enable = !a_enable;
	LCDPrintString(11, a_enable? astr2: astr1);
}

void B_SetAlert()
{
	LCDPrintString(0, str3);
	// restore
	LCDPrintString(16+0,aahh);
	LCDPrintChar(16+2,':');
	LCDPrintString(16+3,aann);
	LCDPrintChar(16+5,':');
	LCDPrintString(16+6,aass);
	// reset
	unsigned char idx = 0;
	unsigned char* adpos[]{ aahh + 0, aahh + 1, aann + 0, aann + 1, aass + 0, aass + 1 };
	unsigned char posi[]{ 16 + 0,16 + 1,16 + 3,16 + 4,16 + 6,16 + 7 };
	unsigned char lim[]{ '2','9','5','9','5','9' };
	lcd.blink();
	LCDCursor(16);
	while (1)
	{
		char key = keypad.getKey();
		if (key >= '0' && key <= '9' && key <= lim[idx])
		{
			*adpos[idx] = key;
			
			LCDPrintChar(posi[idx], key);
			idx++;
			LCDCursor(posi[idx]);
		}
		if (key == '*' || key == '#' || idx >= 6)
		{
			// back
			lcd.noBlink();
			flush_all();
			return;
		}
	}
}

void C_SetDate()
{
	LCDPrintString(0, str4);
	// restore
	LCDPrintString(16+0,yy);
	LCDPrintChar(16+2+2,'/');
	LCDPrintString(16+3+2,mm);
	LCDPrintChar(16+5+2,'/');
	LCDPrintString(16 + 6+2, dd);
	
	unsigned char idx = 0;
	unsigned char* adpos[]{ yy + 0, yy + 1,yy+2,yy+3, mm + 0, mm + 1, dd + 0, dd + 1 };
	unsigned char posi[]{ 16 + 0,16 + 1,16 + 2,16 + 3, 16 + 3 + 2,16 + 4 + 2,16 + 6 + 2,16 + 7 + 2 };
	unsigned char lim[]{ '9','9','9','9','1','2','3','1' };
	lcd.blink();
	LCDCursor(16);
	while (1)
	{
		char key = keypad.getKey();
		if (key >= '0' && key <= '9' && key <= lim[idx])
		{
			*adpos[idx] = key;
			
			LCDPrintChar(posi[idx], key);
			idx++;
			LCDCursor(posi[idx]);
		}
		if (key == '*' || key == '#' || idx >= 8)
		{
			// back
			lcd.noBlink();
			flush_all();
			return;
		}
	}
}

void D_SetTime()
{
	LCDPrintString(0, str5);
	// reset
	unsigned char idx = 0;
	unsigned char* adpos[]{ hh + 0, hh + 1, nn + 0, nn + 1, ss + 0, ss + 1 };
	unsigned char posi[]{ 16 + 0,16 + 1,16 + 3,16 + 4,16 + 6,16 + 7 };
	unsigned char lim[]{ '2','9','5','9','5','9' };
	lcd.blink();
	LCDCursor(16);
	while (1)
	{
		char key = keypad.getKey();
		if (key >= '0' && key <= '9' && key <= lim[idx])
		{
			*adpos[idx] = key;
			
			LCDPrintChar(posi[idx], key);
			idx++;
			LCDCursor(posi[idx]);
		}
		if (key == '*' || key == '#' || idx >= 6)
		{
			// back
			lcd.noBlink();
			flush_all();
			return;
		}
	}
}

