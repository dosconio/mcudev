
#include "../cominc/key.h"

using namespace uni;

void KEY_Initialize() {
	GPIO[KEY_1_PORT].enClock();
	GPIO[KEY_2_PORT].enClock();
	KEY_1_REF().setMode(GPIOMode::IN_Floating);// auto speed for IN: Atmost_Input
	KEY_2_REF().setMode(GPIOMode::IN_Floating);
}

GPIO_Pin& KEY_1_REF() {
	return GPIO[KEY_1_PORT][GPIO_KEY_1];
}

GPIO_Pin& KEY_2_REF() {
	return GPIO[KEY_2_PORT][GPIO_KEY_2];
}

// static function but class method
//{TODO} neglogic into this class
bool KEY_Scanwait(GPIO_Pin& key, bool neglogic) {
	if(key ^ neglogic) {	 
		while(key ^ neglogic);
		return 	!neglogic; // true ^ neglogic
	}
	else return neglogic; // false ^ neglogic
}
