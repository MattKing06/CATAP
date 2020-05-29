#ifndef CAMERA_PV_RECORDS_
#define CAMERA_PV_RECORDS_
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LLRFRecords
{
extern std::vector<std::string> cameraRecordList;
}

namespace CameraState
{
	struct CameraStateStruct
	{   // proviude a default constructor
		CameraStateStruct()

		{};
	};
}

#endif // CAMERA_PV_RECORDS_