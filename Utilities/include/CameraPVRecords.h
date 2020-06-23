#ifndef CAMERA_PV_RECORDS_
#define CAMERA_PV_RECORDS_
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace CameraRecords
{
    //"ANA:X_RBV", 
    //"ANA:Y_RBV", 
    //"ANA:SigmaX_RBV", 
    //"ANA:SigmaY_RBV", 
    //"ANA:CovXY_RBV",
    //"ANA:AvgIntensity_RBV", 
    //"ANA:Intensity_RBV", 
    //"ANA:XPix_RBV", 
    //"ANA:YPix_RBV", 
    //"ANA:SigmaXPix_RBV",
    //"ANA:SigmaYPix_RBV", 
    //"ANA:CovXYPix_RBV", 
    //"ANA:PixMM_RBV",
    //"ANA:PixelResults_RBV",


	extern std::string const ANA_X_RBV; 
	extern std::string const ANA_Y_RBV; 
	extern std::string const ANA_SigmaX_RBV; 
	extern std::string const ANA_SigmaY_RBV; 
	extern std::string const ANA_CovXY_RBV; 
	extern std::string const ANA_AvgIntensity_RBV; 
	extern std::string const ANA_XPix_RBV;
	extern std::string const ANA_YPix_RBV;
	extern std::string const ANA_SigmaXPix_RBV;
	extern std::string const ANA_SigmaYPix_RBV;
	extern std::string const ANA_CovXYPix_RBV;
	extern std::string const ANA_AvgIntensity_RBV;
	extern std::string const ANA_Intensity_RBV;
	extern std::string const ANA_PixMM_RBV;


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