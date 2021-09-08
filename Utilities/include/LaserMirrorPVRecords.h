#ifndef LASER_MIRROR_RECORDS
#define LASER_MIRROR_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LaserMirrorRecords
{
	extern std::string const H_MREL;
	extern std::string const V_MREL;
	extern std::string const H_POS;
	extern std::string const V_POS;
	extern std::string const POSBTN;
	extern std::vector<std::string> laserMirrorRecordList;
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