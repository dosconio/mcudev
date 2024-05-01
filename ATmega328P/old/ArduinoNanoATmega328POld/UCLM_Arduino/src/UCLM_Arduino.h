#ifndef ModMCLMArduino
#define ModMCLMArduino



class TM1638
{
public:
	TM1638(uint8_t STB, uint8_t CLK, uint8_t DIO);
	uint8_t btn(void);
	void nixDec(uint32_t number);
	void led(uint8_t state);
private:;
};

#endif
