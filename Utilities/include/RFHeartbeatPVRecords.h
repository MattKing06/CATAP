#ifndef RF_HEARTBEAT_RECORDS
#define RF_HEARTBEAT_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace RFHeartbeatRecords
{

}

namespace RFHeartbeatState
{
	struct RFHeartbeatStateStruct
	{   // proviude a default constructor
		RFHeartbeatStateStruct() :
			numRFHeartbeats(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numRFHeartbeats;

	};
}

#endif