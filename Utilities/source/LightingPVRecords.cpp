#include <LightingPVRecords.h>

namespace LightingRecords
{
	std::string const CLARA_LED_Sta;
	std::string const CLARA_LED_Off;
	std::string const CLARA_LED_On; 
	std::string const VELA_LED_Sta; 
	std::string const VELA_LED_Off; 
	std::string const VELA_LED_On; 

	std::vector<std::string> lightMonitorRecordList = { CLARA_LED_Sta,CLARA_LED_Off,CLARA_LED_On,VELA_LED_Sta,VELA_LED_Off,VELA_LED_On};
	std::vector<std::string> magnetRecordList = { CLARA_LED_Sta,VELA_LED_Sta,};

}
