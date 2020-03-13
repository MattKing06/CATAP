#ifndef LASER_MIRROR_RECORDS
#define LASER_MIRROR_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LaserMirrorRecords
{

}

namespace LaserMirrorState
{
	struct LaserMirrorStateStruct
	{   // proviude a default constructor
		LaserMirrorStateStruct() :
			numLaserMirrors(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numLaserMirrors;

	};
}

#endif