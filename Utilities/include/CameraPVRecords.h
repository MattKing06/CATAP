#ifndef CAMERA_PV_RECORDS_
#define CAMERA_PV_RECORDS_
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace CameraRecords
{
	extern std::string const CAM_BlackLevel;
	extern std::string const CAM_BlackLevel_RBV;
	extern std::string const CAM_Gain;
	extern std::string const CAM_Gain_RBV;
	extern std::string const HDFB_Buffer_Trigger;
	extern std::string const HDFB_Buffer_FilePath;
	extern std::string const HDFB_Buffer_FilePath_RBV;
	extern std::string const HDFB_Buffer_FileName;
	extern std::string const HDFB_Buffer_FileName_RBV;
	extern std::string const HDFB_Buffer_FileNumber;
	extern std::string const HDFB_Buffer_FileNumber_RBV;
	extern std::string const ROI1_MinX_RBV;
	extern std::string const ROI1_MinY_RBV;
	extern std::string const ROI1_MinX;
	extern std::string const ROI1_MinY;
	extern std::string const ROI1_SizeX_RBV;
	extern std::string const ROI1_SizeY_RBV;
	extern std::string const ROI1_SizeX;
	extern std::string const ROI1_SizeY;
	extern std::string const ROI1_ImageData_RBV;
	extern std::string const ROIandMask_SetX;
	extern std::string const ROIandMask_SetY;
	extern std::string const ROIandMask_SetXrad;
	extern std::string const ROIandMask_SetYrad;
	extern std::string const ANA_MaskXRad_RBV;
	extern std::string const ANA_MaskYRad_RBV;
	extern std::string const ANA_UseFloor;
	extern std::string const ANA_UseFloor_RBV;
	extern std::string const ANA_FloorLevel;
	extern std::string const ANA_FloorLevel_RBV;
	extern std::string const ANA_FlooredPoints_RBV;
	extern std::string const ANA_FlooredPercent_RBV;
	extern std::string const ANA_CPU_RBV;
	extern std::string const ANA_CPU_CropSubMask_RBV;
	extern std::string const ANA_CPU_Npoint_RBV;
	extern std::string const ANA_CPU_Dot_RBV;
	extern std::string const ANA_PixW_RBV;
	extern std::string const ANA_PixH_RBV;
	extern std::string const HDF_WriteFile_RBV;
	extern std::string const HDF_WriteStatus;
	extern std::string const HDF_WriteMessage;
	extern std::string const HDF_NumCaptured_RBV;
	extern std::string const HDF_Capture_RBV;
	extern std::string const CAM_Acquire_RBV;
	extern std::string const CAM_Start_Acquire;
	extern std::string const CAM_Stop_Acquire;
	extern std::string const HDF_NumCapture_RBV;
	extern std::string const MAGICK_NumCaptured_RBV;
	extern std::string const MAGICK_WriteFile_RBV;
	extern std::string const MAGICK_WriteStatus;
	extern std::string const MAGICK_WriteMessage;
	extern std::string const MAGICK_Capture_RBV;
	extern std::string const MAGICK_NumCapture_RBV;
	extern std::string const ANA_StepSize_RBV;
	extern std::string const ANA_EnableCallbacks_RBV;
	extern std::string const ANA_UseBkgrnd;
	extern std::string const ANA_UseNPoint;
	extern std::string const ANA_X_RBV;
	extern std::string const ANA_Y_RBV;
	extern std::string const ANA_SigmaX_RBV;
	extern std::string const ANA_SigmaY_RBV;
	extern std::string const ANA_CovXY_RBV;
	extern std::string const ANA_AvgIntensity_RBV;
	extern std::string const ANA_Intensity_RBV;
	extern std::string const ANA_XPix_RBV;
	extern std::string const ANA_YPix_RBV;
	extern std::string const ANA_SigmaXPix_RBV;
	extern std::string const ANA_SigmaYPix_RBV;
	extern std::string const ANA_CovXYPix_RBV;
	extern std::string const ANA_PixelResults_RBV;
	extern std::string const ANA_MaskXCenter_RBV;
	extern std::string const ANA_MaskYCenter_RBV;
	extern std::string const ANA_MaskXRad_RBV;
	extern std::string const ANA_MaskYRad_RBV;
	extern std::string const ANA_CenterX_RBV;
	extern std::string const ANA_CenterY_RBV;
	extern std::string const ANA_PixMM_RBV;
	extern std::string const CAM_AcquireTime_RBV;
	extern std::string const CAM_AcquirePeriod_RBV;
	extern std::string const CAM_ArrayRate_RBV;
	extern std::string const CAM_Temperature_RBV;
	extern std::string const HDF_FilePath;
	extern std::string const HDF_FileName;
	extern std::string const HDF_FilePath_RBV;
	extern std::string const HDF_FileName_RBV;
	extern std::string const HDF_FileNumber;
	extern std::string const HDF_FileNumber_RBV;
	extern std::string const HDF_WriteFile;
	extern std::string const HDF_Capture;
	extern std::string const HDF_NumCapture;
	extern std::string const MAGICK_FileName;
	extern std::string const MAGICK_FilePath;
	extern std::string const MAGICK_FileNumber;
	extern std::string const MAGICK_Capture;
	extern std::string const MAGICK_WriteFile;
	extern std::string const MAGICK_NumCapture;
	extern std::string const ANA_StepSize;
	extern std::string const ANA_EnableCallbacks;
	extern std::string const ANA_NewBkgrnd;
	extern std::string const ANA_UseBkgrnd;
	extern std::string const ANA_UseNPoint;
	extern std::string const CAM2_ArrayData;
	extern std::string const ANA_MaskXCenter;
	extern std::string const ANA_MaskYCenter;
	extern std::string const ANA_MaskXRad;
	extern std::string const ANA_MaskYRad;
	extern std::string const ANA_CenterX;
	extern std::string const ANA_CenterY;
	extern std::string const ANA_PixMM;
	extern std::string const LED_Sta;
	extern std::string const LED_Off;
	extern std::string const LED_On;
	extern std::string const CAM_BlackLevel;
	extern std::string const CAM_BlackLevel_RBV;
	extern std::string const CAM_Gain;
	extern std::string const CAM_Gain_RBV;

	extern std::string const OVERLAY_CROSS_HAIR;
	extern std::string const OVERLAY_CENTRE_OF_MASS;
	extern std::string const OVERLAY_MASK;



	extern std::vector<std::string> cameraRecordList;

	extern std::vector<std::string> cameraMonitorRecordsList;
	// extern std::string const ANA_AvgIntensity_RBV;
	// extern std::string const ANA_Intensity_RBV;
	// extern std::string const ANA_PixMM_RBV;


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