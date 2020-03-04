#ifndef SHUTTER_RECORDS
#define SHUTTER_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace ShutterRecords
{

}

namespace ShutterState
{
	struct ShutterStateStruct
	{   // proviude a default constructor
		ShutterStateStruct() :
			numShutters(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numShutters;

	};
}

#endif