#ifndef IMG_RECORDS
#define IMG_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace IMGRecords
{

}

namespace IMGState
{
	struct IMGStateStruct
	{   // proviude a default constructor
		IMGStateStruct() :
			numIMGs(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numIMGs;

	};
}

#endif