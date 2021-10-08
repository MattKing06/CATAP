#ifndef STAGE_RECORDS
#define STAGE_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace StageRecords
{
	extern std::string const MABSS;
	extern std::string const RPOSS;
	extern std::vector<std::string> stageRecordList;
}

namespace StageState
{
	struct StageStateStruct
	{   // proviude a default constructor
		StageStateStruct() :
			numStages(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numStages;

	};
}

#endif