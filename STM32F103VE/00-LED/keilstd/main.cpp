// ARMCC-5 GB2312 @dosconio

#define GPIOB_CLK (*(volatile unsigned int*)0x40021018)
#define GPIOB_CRL (*(volatile unsigned int*)0x40010C00)
#define GPIOB_ODR (*(volatile unsigned int*)0x40010C0C)	

//
#define GPIOB_LED_R 5
#define GPIOB_LED_G 0
#define GPIOB_LED_B 1

void lighton(unsigned led_id) {
	// ÍÆÍìÊä³ö
	GPIOB_CRL &= ~(0xf<<(4*led_id));
	GPIOB_CRL |= (2<<(4*led_id));
	GPIOB_ODR &= ~(0x1<<(1*led_id));
}

int main() {
	// Clock
	GPIOB_CLK |= (1<<3);
	
	lighton(GPIOB_LED_R);
	//lighton(GPIOB_LED_G);
	lighton(GPIOB_LED_B);
}
