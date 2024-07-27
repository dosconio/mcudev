// ASCII CPP TAB4 CRLF
// Docutitle: (Driver) LCD Screen
// Codifiers: @dosconio: 20240724
// Attribute: 
// Copyright: MCUDEV @dosconio

#include "driver/Video/ILI9320.h"
#include "ustring.h"

extern "C" const byte asc2_1608[95][16];
extern "C" const byte asc2_2412[95][36];
extern "C" const byte asc2_3216[95][128];// 20240725: Why it is 128£¿

namespace uni {
	
	static void DrawASCII(const Point& disp, const DisplayFont& font, const byte byt, ILI9320_FreePins& pare, const byte* lib) {
		const byte* chp = lib;
		word ch = 0x100 | (word)*chp++;
		for0 (i, font.size.x) {
			for0 (j, font.size.y) {
				if (ch & 0x80) pare.Draw(Point(disp.x+i, disp.y+j), font.forecolor);
				if (ch & 0x8000) ch = 0x100 | (word)*chp++; // haruno style
				else ch <<= 1;
			}
		}
	}
	
	void ILI9320_FreePins::DrawFont(const Point& disp, const DisplayFont& font) {
		// Half-size Characters
		pureptr_t fonts = 0;
		if (font.size.y == 2*font.size.x && font.addr) {
			// ((font.size.y==12)||(font.size.y==16)||(font.size.y==24)||(font.size.y==32))
			switch (font.size.y) {
			case 16: fonts = (pureptr_t)asc2_1608; break;
			case 24: fonts = (pureptr_t)asc2_2412; break;
			case 32: fonts = (pureptr_t)asc2_3216; break;
			default: break;
			}
			if (!fonts) return;
			stduint font_offset = (font.size.x * font.size.y) >> 3;
			if (font.size.y == 32) font_offset <<= 1;
			Point d = disp;
			for0 (i, /*font.numbers*/ StrLength((char*)font.addr)) {
				byte ch = ((byte*)font.addr)[i];
				DrawASCII(d, font, ch, self, (byte*)fonts + (ch-0x20) * font_offset);
				d.x += font.size.x;
				if (d.x >= xlim) {
					d.x = 0;
					if ((d.y += font.size.y) >= ylim) d.y = 0;
				}
			}
		}
	}

}
