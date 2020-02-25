#ifndef RF_MODULATOR_RECORDS
#define RF_MODULATOR_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace RFModulatorRecords
{

}

namespace RFModulatorState
{
	struct RFModulatorStateStruct
	{   // proviude a default constructor
		RFModulatorStateStruct() :
			numRFModulators(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numRFModulators;

	};
}

#endif