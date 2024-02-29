
#include <mcs51/8051.h>

void Delay(unsigned int ms);

void main() {
	// Period: 0.5 second
	while (1) {
		P2_1 = 0;
		Delay(250);
		P2_1 = 1;
		Delay(250); 
	}
}

void Delay(unsigned int ms){
	unsigned int a, b;
	for(a = ms; a > 0; a--)
		for(b = 100; b > 0; b--)
			;
}
