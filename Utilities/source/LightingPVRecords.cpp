#include <LightingPVRecords.h>

namespace LightingRecords
{
	std::string const CLARA_LED_Sta = "CLARA_LED_Sta";
	std::string const CLARA_LED_Off = "CLARA_LED_Off";
	std::string const CLARA_LED_On = "CLARA_LED_On";
	std::string const VELA_LED_Sta = "VELA_LED_Sta";
	std::string const VELA_LED_Off = "VELA_LED_Off";
	std::string const VELA_LED_On = "VELA_LED_On";

	std::string const ACCELERATOR_HALL_LIGHT_Sta = "ACCELERATOR_HALL_LIGHT_Sta"; // = "VELA_LED_Off";
	std::string const ACCELERATOR_HALL_LIGHT_Off = "ACCELERATOR_HALL_LIGHT_Off"; // = "VELA_LED_Off";
	std::string const ACCELERATOR_HALL_LIGHT_On = "ACCELERATOR_HALL_LIGHT_On"; // = "VELA_LED_Off";

	std::string const BA1_LIGHT_Sta = "BA1_LIGHT_Sta"; // = "VELA_LED_Off";
	std::string const BA1_LIGHT_Off = "BA1_LIGHT_Off"; // = "VELA_LED_Off";
	std::string const BA1_LIGHT_On = "BA1_LIGHT_On"; // = "VELA_LED_Off";

	std::vector<std::string> lightRecordList = { CLARA_LED_Sta,CLARA_LED_Off,CLARA_LED_On,VELA_LED_Sta,VELA_LED_Off,VELA_LED_On,
	ACCELERATOR_HALL_LIGHT_Sta, ACCELERATOR_HALL_LIGHT_Off, ACCELERATOR_HALL_LIGHT_On, BA1_LIGHT_Sta, BA1_LIGHT_Off, BA1_LIGHT_On};

	std::vector<std::string> lightMonitorRecordList = { CLARA_LED_Sta,VELA_LED_Sta,ACCELERATOR_HALL_LIGHT_Sta,BA1_LIGHT_Sta };

}
