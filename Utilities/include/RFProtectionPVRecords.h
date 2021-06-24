#ifndef RF_PROTECTION_RECORDS
#define RF_PROTECTION_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace RFProtectionRecords
{
	extern std::string const Sta;
	extern std::string const Cmi;
	extern std::string const ON;
	extern std::string const OFF;
	extern std::string const RST;

	extern std::vector<std::string> const rfProtectionRecordsList;
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