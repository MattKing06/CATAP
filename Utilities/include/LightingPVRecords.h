#ifndef LIGHTING_RECORDS
#define LIGHTING_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LightingRecords
{

}

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