#ifndef RF_PROTECTION_RECORDS
#define RF_PROTECTION_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace RFProtectionRecords
{

}

namespace RFProtectionState
{
	struct RFProtectionStateStruct
	{   // proviude a default constructor
		RFProtectionStateStruct() :
			numRFProtections(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numRFProtections;

	};
}

#endif