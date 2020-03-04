#ifndef LED_RECORDS
#define LED_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LEDRecords
{

}

namespace LEDState
{
	struct LEDStateStruct
	{   // proviude a default constructor
		LEDStateStruct() :
			numLEDs(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numLEDs;

	};
}

#endif