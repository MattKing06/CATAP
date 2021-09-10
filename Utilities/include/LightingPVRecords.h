#ifndef LIGHTING_RECORDS
#define LIGHTING_RECORDS
#include <string>
#include <vector>
//#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LightingRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const CLARA_LED_Sta; // = "CLARA_LED_Sta";
	extern std::string const CLARA_LED_Off; // = "CLARA_LED_Off";
	extern std::string const CLARA_LED_On; // = "CLARA_LED_On";
	extern std::string const VELA_LED_Sta; // = "VELA_LED_Sta";
	extern std::string const VELA_LED_Off; // = "VELA_LED_Off";
	extern std::string const VELA_LED_On; // = "VELA_LED_Off";

	extern std::string const ACCELERATOR_HALL_LIGHT_Sta; // = "VELA_LED_Off";
	extern std::string const ACCELERATOR_HALL_LIGHT_Off; // = "VELA_LED_Off";
	extern std::string const ACCELERATOR_HALL_LIGHT_On; // = "VELA_LED_Off";

	extern std::string const BA1_LIGHT_Sta; // = "VELA_LED_Off";
	extern std::string const BA1_LIGHT_Off; // = "VELA_LED_Off";
	extern std::string const BA1_LIGHT_On; // = "VELA_LED_Off";

	extern std::vector<std::string> lightRecordList;
	extern std::vector<std::string> lightMonitorRecordList;
}
/* hmmm */
namespace LightingState
{
	struct LightingStateStruct
	{   // proviude a default constructor
		LightingStateStruct() :
			numLightings(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numLightings;

	};
}

#endif