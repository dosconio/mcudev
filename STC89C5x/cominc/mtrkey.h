#ifndef DevMatrixKey
#define DevMatrixKey

// @Haruno RFE22:19
// Return [15~0] for the press of matrix key 16~1, e.g. if only S1 is pressed, this will return 0b[0000_0000_0000_0001]
// Want "REGX52.h" Library
unsigned short MtrKeyReadMap()
{
	// by Column Scan Method
	unsigned short map = 0;
	// Here he used the enum. but the loop because the easily countable keys.
	P1 = 0xFF; P1_3 = 0;
	if(!P1_7) map |= 0x0001;
	if(!P1_6) map |= 0x0010;
	if(!P1_5) map |= 0x0100;
	if(!P1_4) map |= 0x1000;

	P1 = 0xFF; P1_2 = 0;
	if(!P1_7) map |= 0x0002;
	if(!P1_6) map |= 0x0020;
	if(!P1_5) map |= 0x0200;
	if(!P1_4) map |= 0x2000;

	P1 = 0xFF; P1_1 = 0;
	if(!P1_7) map |= 0x0004;
	if(!P1_6) map |= 0x0040;
	if(!P1_5) map |= 0x0400;
	if(!P1_4) map |= 0x4000;

	P1 = 0xFF; P1_0 = 0;
	if(!P1_7) map |= 0x0008;
	if(!P1_6) map |= 0x0080;
	if(!P1_5) map |= 0x0800;
	if(!P1_4) map |= 0x8000;
	return map; 
}

#endif
