// ARMCC-5 GBK @dosconio 20240726
// SI5351
// Reference Adafruit(R)
// depends: git clone https://github.com/dosconio/unisym.git
#include "MCU/ST/STM32F4"
#include "Device/IIC"
#include <math.h>
using namespace uni;
GPIO_Pin& LEDR = GPIOI[5];

#define SI5351_ADDRESS (0x60<<1) // assume ADDR pin = low

static void inline_delay(void) {
	for0(i, 0x100);
}


/* Test setup from SI5351 ClockBuilder
 * -----------------------------------
 * XTAL:      25     MHz
 * Channel 0: 120.00 MHz
 * Channel 1: 12.00  MHz
 * Channel 2: 13.56  MHz
 */
static const uint8_t regs_15to92_149to170[100][2] =
{
	{  15, 0x00 },    /* Input source = crystal for PLLA and PLLB */
	{  16, 0x4F },    /* CLK0 Control: 8mA drive, Multisynth 0 as CLK0 source, Clock not inverted, Source = PLLA, Multisynth 0 in integer mode, clock powered up */
	{  17, 0x4F },    /* CLK1 Control: 8mA drive, Multisynth 1 as CLK1 source, Clock not inverted, Source = PLLA, Multisynth 1 in integer mode, clock powered up */
	{  18, 0x6F },    /* CLK2 Control: 8mA drive, Multisynth 2 as CLK2 source, Clock not inverted, Source = PLLB, Multisynth 2 in integer mode, clock powered up */
	{  19, 0x80 },    /* CLK3 Control: Not used ... clock powered down */
	{  20, 0x80 },    /* CLK4 Control: Not used ... clock powered down */
	{  21, 0x80 },    /* CLK5 Control: Not used ... clock powered down */
	{  22, 0x80 },    /* CLK6 Control: Not used ... clock powered down */
	{  23, 0x80 },    /* CLK7 Control: Not used ... clock powered down */
	{  24, 0x00 },    /* Clock disable state 0..3 (low when disabled) */
	{  25, 0x00 },    /* Clock disable state 4..7 (low when disabled) */
	/* PLL_A Setup */
	{  26, 0x00 }, {  27, 0x05 }, {  28, 0x00 }, {  29, 0x0C }, {  30, 0x66 }, {  31, 0x00 }, {  32, 0x00 }, {  33, 0x02 },
	/* PLL_B Setup */
	{  34, 0x02 }, {  35, 0x71 }, {  36, 0x00 }, {  37, 0x0C }, {  38, 0x1A }, {  39, 0x00 }, {  40, 0x00 }, {  41, 0x86 },
	/* Multisynth Setup */
	{  42, 0x00 }, {  43, 0x01 }, {  44, 0x00 }, {  45, 0x01 }, {  46, 0x00 }, {  47, 0x00 }, {  48, 0x00 }, {  49, 0x00 }, {  50, 0x00 }, {  51, 0x01 }, {  52, 0x00 }, {  53, 0x1C }, {  54, 0x00 }, {  55, 0x00 }, {  56, 0x00 }, {  57, 0x00 }, {  58, 0x00 }, {  59, 0x01 }, {  60, 0x00 }, {  61, 0x18 }, {  62, 0x00 }, {  63, 0x00 }, {  64, 0x00 }, {  65, 0x00 }, {  66, 0x00 }, {  67, 0x00 }, {  68, 0x00 }, {  69, 0x00 }, {  70, 0x00 }, {  71, 0x00 }, {  72, 0x00 }, {  73, 0x00 }, {  74, 0x00 }, {  75, 0x00 }, {  76, 0x00 }, {  77, 0x00 }, {  78, 0x00 }, {  79, 0x00 }, {  80, 0x00 }, {  81, 0x00 }, {  82, 0x00 }, {  83, 0x00 }, {  84, 0x00 }, {  85, 0x00 }, {  86, 0x00 }, {  87, 0x00 }, {  88, 0x00 }, {  89, 0x00 }, {  90, 0x00 }, {  91, 0x00 }, {  92, 0x00 },
	/* Misc Config Register */
	{ 149, 0x00 }, { 150, 0x00 }, { 151, 0x00 }, { 152, 0x00 }, { 153, 0x00 }, { 154, 0x00 }, { 155, 0x00 }, { 156, 0x00 }, { 157, 0x00 }, { 158, 0x00 }, { 159, 0x00 }, { 160, 0x00 }, { 161, 0x00 }, { 162, 0x00 }, { 163, 0x00 }, { 164, 0x00 }, { 165, 0x00 }, { 166, 0x00 }, { 167, 0x00 }, { 168, 0x00 }, { 169, 0x00 }, { 170, 0x00 }
};

class SI5351_t {
public:
	enum PLL {
		SI5351_PLL_A = 0,
		SI5351_PLL_B,
	};
protected:
	IIC_t IIC;

	bool state;// initialised;
	enum CrystalFreq
	{
		SI5351_CRYSTAL_FREQ_25MHZ = (25000000),
		SI5351_CRYSTAL_FREQ_27MHZ = (27000000)
	} CrystalFrequency;
	enum CrystalLoad {
		SI5351_CRYSTAL_LOAD_6PF = (1 << 6),
		SI5351_CRYSTAL_LOAD_8PF = (2 << 6),
		SI5351_CRYSTAL_LOAD_10PF = (3 << 6)
	} CrystalLoading;
	uint32 CrystalPPM;
	uint32 PLLA_Frequency;
	uint32 PLLB_Frequency;

	void Send(byte reg, byte val) {
		IIC.SendStart();
		IIC << (SI5351_ADDRESS);
		IIC << (reg);
		IIC << (val);
		IIC.SendStop();
	}

	byte Read(byte reg) {
		
		IIC.SendStart();
		IIC << (SI5351_ADDRESS);
		IIC << (reg);
		// IIC.SendStop();
		IIC.SendStart();
		IIC << (SI5351_ADDRESS | 0x01);
		byte res = IIC.ReadByte();//{?} 2 true?
		IIC.SendStop();
		return res;
	}

	void getPLLRegister(uint32 div_or_mul, uint32 num, uint32 denom, uint32& P1, uint32& P2, uint32& P3) {
		// Set the main PLL config registers
		if (num == 0) { // Integer mode
			P1 = 128 * div_or_mul - 512;
			P2 = num;
			P3 = denom;
		}
		else { // Fractional mode
			P1 = (uint32_t)(128 * div_or_mul + floor(128 * ((float)num / (float)denom)) - 512);
			P2 = (uint32_t)(128 * num - denom * floor(128 * ((float)num / (float)denom)));
			P3 = denom;
		}
	}

public:
	SI5351_t(GPIO_Pin& SDA, GPIO_Pin& SCL, void (*delay_ms)(void)) : IIC(SDA,SCL) {
		IIC.func_delay = inline_delay;
		state = false;
		CrystalFrequency = SI5351_CRYSTAL_FREQ_25MHZ;
		CrystalLoading = SI5351_CRYSTAL_LOAD_10PF;
		CrystalPPM = 30;
		PLLA_Frequency = 0;
		PLLB_Frequency = 0;
		state = true;
	}

	operator bool() { return state; }

	bool enAble(bool ena = true) {
		Send(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, ena ? 0x00 : 0xFF);
		return true;
	}

	/*! Configures the Multisynth divider, which determines the
			output clock frequency based on the specified PLL input.
	@param  output    The output channel to use (0..2)
	@param  pllSource	The PLL input source to use, which must be one of:
					  - SI5351_PLL_A
					  - SI5351_PLL_B
	@param  div       The integer divider for the Multisynth output.
					  If pure integer values are used, this value must
					  be one of:
					  - SI5351_MULTISYNTH_DIV_4
					  - SI5351_MULTISYNTH_DIV_6
					  - SI5351_MULTISYNTH_DIV_8
					  If fractional output is used, this value must be
					  between 8 and 900.
	@param  num       The 20-bit numerator for fractional output
					  (0..1,048,575). Set this to '0' for integer output.
	@param  denom     The 20-bit denominator for fractional output
					  (1..1,048,575). Set this to '1' or higher to
					  avoid divide by zero errors.

	@section Output Clock Configuration

	The multisynth dividers are applied to the specified PLL output,
	and are used to reduce the PLL output to a valid range (500kHz
	to 160MHz). The relationship can be seen in this formula, where
	fVCO is the PLL output frequency and MSx is the multisynth
	divider:
		fOUT = fVCO / MSx
	Valid multisynth dividers are 4, 6, or 8 when using integers,
	or any fractional values between 8 + 1/1,048,575 and 900 + 0/1
	The following formula is used for the fractional mode divider:
		a + b / c
	a = The integer value, which must be 4, 6 or 8 in integer mode (MSx_INT=1)
		or 8..900 in fractional mode (MSx_INT=0).
	b = The fractional numerator (0..1,048,575)
	c = The fractional denominator (1..1,048,575)
	@note   Try to use integers whenever possible to avoid clock jitter
	@note   For output frequencies > 150MHz, you must set the divider
			to 4 and adjust to PLL to generate the frequency (for example
			a PLL of 640 to generate a 160MHz output clock). This is not
			yet supported in the driver, which limits frequencies to
			500kHz .. 150MHz.

	@note   For frequencies below 500kHz (down to 8kHz) Rx_DIV must be
			used, but this isn't currently implemented in the driver.
	*/
	// default parameters for integer output
	bool setMultisynth(PLL pll, byte outp, stduint div, stduint num = 0, stduint denom = 1) {
		if (pll == SI5351_PLL_A && !PLLA_Frequency) return false;
		if (pll == SI5351_PLL_B && !PLLB_Frequency) return false;
		uint32 P1;// Multisynth config register P1
		uint32 P2;// Multisynth config register P2
		uint32 P3;// Multisynth config register P3
		/* Output Multisynth Divider Equations
		 * where: a = div, b = num and c = denom
		 * P1 register is an 18-bit value using following formula:
		 * 	P1[17:0] = 128 * a + floor(128*(b/c)) - 512
		 * P2 register is a 20-bit value using the following formula:
		 * 	P2[19:0] = 128 * b - c * floor(128*(b/c))
		 * P3 register is a 20-bit value using the following formula:
		 * 	P3[19:0] = c
		 */
		getPLLRegister(div, num, denom, P1, P2, P3);
		// Get the appropriate starting point for the PLL registers
		uint8_t baseaddr = 0;
		switch (outp) {
		case 0:
			baseaddr = SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1;
			break;
		case 1:
			baseaddr = SI5351_REGISTER_50_MULTISYNTH1_PARAMETERS_1;
			break;
		case 2:
			baseaddr = SI5351_REGISTER_58_MULTISYNTH2_PARAMETERS_1;
			break;
		}
		// Set the MSx config registers
		Send(baseaddr, (P3 & 0x0000FF00) >> 8);
		Send(baseaddr + 1, (P3 & 0x000000FF));
		Send(baseaddr + 2, (P1 & 0x00030000) >> 16);	//{TODO} Add DIVBY4 (>150MHz) and R0 support (<500kHz) later
		Send(baseaddr + 3, (P1 & 0x0000FF00) >> 8);
		Send(baseaddr + 4, (P1 & 0x000000FF));
		Send(baseaddr + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
		Send(baseaddr + 6, (P2 & 0x0000FF00) >> 8);
		Send(baseaddr + 7, (P2 & 0x000000FF));
		// Configure the clk control and enable the output
		byte clkControlReg = 0x0F;// 8mA drive strength, MS0 as CLK0 source, Clock not inverted, powered up
		if (pll == SI5351_PLL_B) clkControlReg |= (1 << 5);
		if (num == 0) clkControlReg |= (1 << 6);
		switch (outp)
		{
		case 0:
			Send(SI5351_REGISTER_16_CLK0_CONTROL, clkControlReg);
			break;
		case 1:
			Send(SI5351_REGISTER_17_CLK1_CONTROL, clkControlReg);
			break;
		case 2:
			Send(SI5351_REGISTER_18_CLK2_CONTROL, clkControlReg);
			break;
		}
		return true;
	}


	/*! Sets the multiplier for the specified PLL
	@param  pll   The PLL to configure, which must be one of the following:
				  - SI5351_PLL_A
				  - SI5351_PLL_B
	@param  mult  The PLL integer multiplier (must be between 15 and 90)
	@param  num   The 20-bit numerator for fractional output (0..1,048,575).
				  Set this to '0' for integer output.
	@param  denom The 20-bit denominator for fractional output (1..1,048,575).
				  Set this to '1' or higher to avoid divider by zero errors.

	@section PLL Configuration

	fVCO is the PLL output, and must be between 600..900MHz, where:

		fVCO = fXTAL * (a+(b/c))

	fXTAL = the crystal input frequency
	a     = an integer between 15 and 90
	b     = the fractional numerator (0..1,048,575)
	c     = the fractional denominator (1..1,048,575)

	NOTE: Try to use integers whenever possible to avoid clock jitter
	(only use the a part, setting b to '0' and c to '1').

	See: http://www.silabs.com/Support%20Documents/TechnicalDocs/AN619.pdf
	Feedback Multisynth Divider Equation
	* where: a = mult, b = num and c = denom
	* P1 register is an 18-bit value using following formula:
	* 	P1[17:0] = 128 * mult + floor(128*(num/denom)) - 512
	* P2 register is a 20-bit value using the following formula:
	* 	P2[19:0] = 128 * num - denom * floor(128*(num/denom))
	* P3 register is a 20-bit value using the following formula:
	* 	P3[19:0] = denom
	*/
	// default parameters are for integer mode
	bool setPLL(PLL pll, byte mult, stduint num = 0, stduint denom = 1) {

		uint32 P1;
		uint32 P2;
		uint32 P3;
		getPLLRegister(mult, num, denom, P1, P2, P3);
		// : Get the appropriate starting point for the PLL registers
		uint8_t baseaddr = (pll == SI5351_PLL_A ? 26 : 34);
		// : The datasheet is a nightmare of typos and inconsistencies here
		Send(baseaddr, (P3 & 0x0000FF00) >> 8);
		Send(baseaddr + 1, (P3 & 0x000000FF));
		Send(baseaddr + 2, (P1 & 0x00030000) >> 16);
		Send(baseaddr + 3, (P1 & 0x0000FF00) >> 8);
		Send(baseaddr + 4, (P1 & 0x000000FF));
		Send(baseaddr + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
		Send(baseaddr + 6, (P2 & 0x0000FF00) >> 8);
		Send(baseaddr + 7, (P2 & 0x000000FF));
		// : Reset both PLLs
		Send(SI5351_REGISTER_177_PLL_RESET, (1 << 7) | (1 << 5));
		// : Store the frequency settings for use with the Multisynth helper
		float fvco = CrystalFrequency * (mult + ((float)num / (float)denom));
		(pll == SI5351_PLL_A ? PLLA_Frequency : PLLB_Frequency) = (uint32_t)floor(fvco);
		return true;
	}



	typedef enum {
		SI5351_R_DIV_1 = 0,
		SI5351_R_DIV_2,
		SI5351_R_DIV_4,
		SI5351_R_DIV_8,
		SI5351_R_DIV_16,
		SI5351_R_DIV_32,
		SI5351_R_DIV_64,
		SI5351_R_DIV_128,
	} RDiv;
	bool setRdiv(RDiv div, byte outp) {
		if (outp < 3); else return false;
		byte Rreg, regval;
		if (outp == 0) Rreg = SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3;
		if (outp == 1) Rreg = SI5351_REGISTER_52_MULTISYNTH1_PARAMETERS_3;
		if (outp == 2) Rreg = SI5351_REGISTER_60_MULTISYNTH2_PARAMETERS_3;
		regval = Read(Rreg) & 0x0F;
		byte divider = div & 0x07;
		divider <<= 4;
		regval |= divider;
		Send(Rreg, regval);
		return true;
	}

	// Configures the Si5351 with config settings generated in 	ClockBuilder.You can use this function to make sure that your HW is properly configure and that there are no problems with the board itself. Running this function should provide the following output :
	// * Channel 0 : 120.00 MHz
	// * Channel 1 : 12.00  MHz
	// * Channel 2 : 13.56  MHz
	bool setClockBuilderData(void)
	{
		// : Disable all outputs setting CLKx_DIS high
		Send(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, 0xFF);
		// : Writes configuration data to device using the register map contents 	 generated by ClockBuilder Desktop (registers 15-92 + 149-170)
		for (stduint i = 0; i < sizeof(regs_15to92_149to170) / 2; i++) {
			Send(regs_15to92_149to170[i][0], regs_15to92_149to170[i][1]);
		}
		// : Apply soft reset
		Send(SI5351_REGISTER_177_PLL_RESET, 0xAC);
		// : Enabled desired outputs (see Register 3)
		Send(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, 0x00);
		return true;
	}

	bool setMode() {
//{TODO} ToDo: Make sure we're actually connected
// : Disable all outputs setting CLKx_DIS high
		Send(SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, 0xFF);
		// : Power down all output drivers
		Send(SI5351_REGISTER_16_CLK0_CONTROL, 0x80);
		Send(SI5351_REGISTER_17_CLK1_CONTROL, 0x80);
		Send(SI5351_REGISTER_18_CLK2_CONTROL, 0x80);
		Send(SI5351_REGISTER_19_CLK3_CONTROL, 0x80);
		Send(SI5351_REGISTER_20_CLK4_CONTROL, 0x80);
		Send(SI5351_REGISTER_21_CLK5_CONTROL, 0x80);
		Send(SI5351_REGISTER_22_CLK6_CONTROL, 0x80);
		Send(SI5351_REGISTER_23_CLK7_CONTROL, 0x80);
		// : Set the load capacitance for the XTAL */
		Send(SI5351_REGISTER_183_CRYSTAL_INTERNAL_LOAD_CAPACITANCE, CrystalLoading);

		// Set interrupt masks as required (see Register 2 description in AN619). By default, ClockBuilder Desktop sets this register to 0x18. Note that the least significant nibble must remain 0x8, but the most significant nibble may be modified to suit your needs.
		// : Reset the PLL config fields just in case we call init again
		PLLA_Frequency = 0;
		PLLB_Frequency = 0;
		return true;
	}


	
/* See http://www.silabs.com/Support%20Documents/TechnicalDocs/AN619.pdf for registers 26..41 */
enum {
	SI5351_REGISTER_0_DEVICE_STATUS                       = 0,
	SI5351_REGISTER_1_INTERRUPT_STATUS_STICKY             = 1,
	SI5351_REGISTER_2_INTERRUPT_STATUS_MASK               = 2,
	SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL               = 3,
	SI5351_REGISTER_9_OEB_PIN_ENABLE_CONTROL              = 9,
	SI5351_REGISTER_15_PLL_INPUT_SOURCE                   = 15,
	SI5351_REGISTER_16_CLK0_CONTROL                       = 16,
	SI5351_REGISTER_17_CLK1_CONTROL                       = 17,
	SI5351_REGISTER_18_CLK2_CONTROL                       = 18,
	SI5351_REGISTER_19_CLK3_CONTROL                       = 19,
	SI5351_REGISTER_20_CLK4_CONTROL                       = 20,
	SI5351_REGISTER_21_CLK5_CONTROL                       = 21,
	SI5351_REGISTER_22_CLK6_CONTROL                       = 22,
	SI5351_REGISTER_23_CLK7_CONTROL                       = 23,
	SI5351_REGISTER_24_CLK3_0_DISABLE_STATE               = 24,
	SI5351_REGISTER_25_CLK7_4_DISABLE_STATE               = 25,
	SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1           = 42,
	SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2           = 43,
	SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3           = 44,
	SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4           = 45,
	SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5           = 46,
	SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6           = 47,
	SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7           = 48,
	SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8           = 49,
	SI5351_REGISTER_50_MULTISYNTH1_PARAMETERS_1           = 50,
	SI5351_REGISTER_51_MULTISYNTH1_PARAMETERS_2           = 51,
	SI5351_REGISTER_52_MULTISYNTH1_PARAMETERS_3           = 52,
	SI5351_REGISTER_53_MULTISYNTH1_PARAMETERS_4           = 53,
	SI5351_REGISTER_54_MULTISYNTH1_PARAMETERS_5           = 54,
	SI5351_REGISTER_55_MULTISYNTH1_PARAMETERS_6           = 55,
	SI5351_REGISTER_56_MULTISYNTH1_PARAMETERS_7           = 56,
	SI5351_REGISTER_57_MULTISYNTH1_PARAMETERS_8           = 57,
	SI5351_REGISTER_58_MULTISYNTH2_PARAMETERS_1           = 58,
	SI5351_REGISTER_59_MULTISYNTH2_PARAMETERS_2           = 59,
	SI5351_REGISTER_60_MULTISYNTH2_PARAMETERS_3           = 60,
	SI5351_REGISTER_61_MULTISYNTH2_PARAMETERS_4           = 61,
	SI5351_REGISTER_62_MULTISYNTH2_PARAMETERS_5           = 62,
	SI5351_REGISTER_63_MULTISYNTH2_PARAMETERS_6           = 63,
	SI5351_REGISTER_64_MULTISYNTH2_PARAMETERS_7           = 64,
	SI5351_REGISTER_65_MULTISYNTH2_PARAMETERS_8           = 65,
	SI5351_REGISTER_66_MULTISYNTH3_PARAMETERS_1           = 66,
	SI5351_REGISTER_67_MULTISYNTH3_PARAMETERS_2           = 67,
	SI5351_REGISTER_68_MULTISYNTH3_PARAMETERS_3           = 68,
	SI5351_REGISTER_69_MULTISYNTH3_PARAMETERS_4           = 69,
	SI5351_REGISTER_70_MULTISYNTH3_PARAMETERS_5           = 70,
	SI5351_REGISTER_71_MULTISYNTH3_PARAMETERS_6           = 71,
	SI5351_REGISTER_72_MULTISYNTH3_PARAMETERS_7           = 72,
	SI5351_REGISTER_73_MULTISYNTH3_PARAMETERS_8           = 73,
	SI5351_REGISTER_74_MULTISYNTH4_PARAMETERS_1           = 74,
	SI5351_REGISTER_75_MULTISYNTH4_PARAMETERS_2           = 75,
	SI5351_REGISTER_76_MULTISYNTH4_PARAMETERS_3           = 76,
	SI5351_REGISTER_77_MULTISYNTH4_PARAMETERS_4           = 77,
	SI5351_REGISTER_78_MULTISYNTH4_PARAMETERS_5           = 78,
	SI5351_REGISTER_79_MULTISYNTH4_PARAMETERS_6           = 79,
	SI5351_REGISTER_80_MULTISYNTH4_PARAMETERS_7           = 80,
	SI5351_REGISTER_81_MULTISYNTH4_PARAMETERS_8           = 81,
	SI5351_REGISTER_82_MULTISYNTH5_PARAMETERS_1           = 82,
	SI5351_REGISTER_83_MULTISYNTH5_PARAMETERS_2           = 83,
	SI5351_REGISTER_84_MULTISYNTH5_PARAMETERS_3           = 84,
	SI5351_REGISTER_85_MULTISYNTH5_PARAMETERS_4           = 85,
	SI5351_REGISTER_86_MULTISYNTH5_PARAMETERS_5           = 86,
	SI5351_REGISTER_87_MULTISYNTH5_PARAMETERS_6           = 87,
	SI5351_REGISTER_88_MULTISYNTH5_PARAMETERS_7           = 88,
	SI5351_REGISTER_89_MULTISYNTH5_PARAMETERS_8           = 89,
	SI5351_REGISTER_90_MULTISYNTH6_PARAMETERS             = 90,
	SI5351_REGISTER_91_MULTISYNTH7_PARAMETERS             = 91,
	SI5351_REGISTER_092_CLOCK_6_7_OUTPUT_DIVIDER          = 92,
	SI5351_REGISTER_165_CLK0_INITIAL_PHASE_OFFSET         = 165,
	SI5351_REGISTER_166_CLK1_INITIAL_PHASE_OFFSET         = 166,
	SI5351_REGISTER_167_CLK2_INITIAL_PHASE_OFFSET         = 167,
	SI5351_REGISTER_168_CLK3_INITIAL_PHASE_OFFSET         = 168,
	SI5351_REGISTER_169_CLK4_INITIAL_PHASE_OFFSET         = 169,
	SI5351_REGISTER_170_CLK5_INITIAL_PHASE_OFFSET         = 170,
	SI5351_REGISTER_177_PLL_RESET                         = 177,
	SI5351_REGISTER_183_CRYSTAL_INTERNAL_LOAD_CAPACITANCE	= 183
};

typedef enum
{
	SI5351_MULTISYNTH_DIV_4 = 4,
	SI5351_MULTISYNTH_DIV_6 = 6,
	SI5351_MULTISYNTH_DIV_8 = 8
} MultisynthDiv;


};

int main() {
	LEDR.setMode(GPIOMode::OUT_PushPull);
	if (!RCC.setClock(SysclkSource::HSE)) while (true);
	SI5351_t my_si(GPIOE[1], GPIOI[9], inline_delay);
	while (!my_si);
	// Set PLLA to 900MHz, Set Output #0 to 112.5MHz
	/* INTEGER ONLY MODE --> most accurate output */
	/* Setup PLLA to integer only mode @ 900MHz (must be 600..900MHz) */
	/* Set Multisynth 0 to 112.5MHz using integer only mode (div by 4/6/8) */
	/* 25MHz * 36 = 900 MHz, then 900 MHz / 8 = 112.5 MHz */
	{
		my_si.setPLL(SI5351_t::SI5351_PLL_A, 36);
		my_si.setMultisynth(SI5351_t::SI5351_PLL_A, 0, SI5351_t::SI5351_MULTISYNTH_DIV_8);
	}
	// Set Output #1 to 13.553115MHz
	/* FRACTIONAL MODE --> More flexible but introduce clock jitter */
	/* Setup PLLB to fractional mode @616.66667MHz (XTAL * 24 + 2/3) */
	/* Setup Multisynth 1 to 13.55311MHz (PLLB/45.5) */
	// Set Output #2 to 10.706 KHz
	/* Multisynth 2 is not yet used and won't be enabled, but can be */
	/* Use PLLB @ 616.66667MHz, then divide by 900 -> 685.185 KHz */
	/* then divide by 64 for 10.706 KHz */
	/* configured using either PLL in either integer or fractional mode */
	{
		my_si.setPLL(SI5351_t::SI5351_PLL_B, 24, 2, 3);
		my_si.setMultisynth(SI5351_t::SI5351_PLL_B, 1, 45, 1, 2);
		my_si.setMultisynth(SI5351_t::SI5351_PLL_B, 2, 900, 0, 1);
		my_si.setRdiv(SI5351_t::SI5351_R_DIV_64, 2);
	}
	
	my_si.enAble();
	while (true) {
		LEDR.Toggle();
		SysDelay(500);// ms
	}
}
