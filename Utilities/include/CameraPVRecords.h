#ifndef CAMERA_PV_RECORDS_
#define CAMERA_PV_RECORDS_
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace CameraRecords
{
	// BUFFER DUMPING (7)
	extern std::string const HDFB_Buffer_Trigger;                                 
	extern std::string const HDFB_Buffer_FilePath;
	extern std::string const HDFB_Buffer_FilePath_RBV;
	extern std::string const HDFB_Buffer_FileName;
	extern std::string const HDFB_Buffer_FileName_RBV;
	extern std::string const HDFB_Buffer_FileNumber;
	extern std::string const HDFB_Buffer_FileNumber_RBV;
	// REGION OF INTEREST (AND MASK) (13)
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
	// IMAGE DUMPIING  (14)
	extern std::string const HDF_WriteStatus;
	extern std::string const HDF_WriteMessage;
	extern std::string const HDF_Capture;
	extern std::string const HDF_Capture_RBV;
	extern std::string const HDF_NumCapture;
	extern std::string const HDF_NumCapture_RBV;
	extern std::string const HDF_FilePath;
	extern std::string const HDF_FilePath_RBV;
	extern std::string const HDF_FileName;
	extern std::string const HDF_FileName_RBV;
	extern std::string const HDF_FileNumber;
	extern std::string const HDF_FileNumber_RBV;
	extern std::string const HDF_WriteFile;
	extern std::string const HDF_WriteFile_RBV;
	// CAMERA IMAGE ACQUIISAITON (11)
	extern std::string const CAM_Acquire_RBV;
	extern std::string const CAM_Start_Acquire;
	extern std::string const CAM_Stop_Acquire;
	extern std::string const CAM_AcquireTime_RBV;
	extern std::string const CAM_AcquirePeriod_RBV;
	extern std::string const CAM_ArrayRate_RBV;
	extern std::string const CAM2_ArrayData;
	extern std::string const CAM_Gain;
	extern std::string const CAM_Gain_RBV;
	extern std::string const CAM_BlackLevel;
	extern std::string const CAM_BlackLevel_RBV;
	// ENABLE DISBALE ANALYSIS  (2)
	extern std::string const ANA_EnableCallbacks;
	extern std::string const ANA_EnableCallbacks_RBV;
	// ANALYSIS FLOOR (6)
	extern std::string const ANA_UseFloor;
	extern std::string const ANA_UseFloor_RBV;
	extern std::string const ANA_FloorLevel;
	extern std::string const ANA_FloorLevel_RBV;
	extern std::string const ANA_FlooredPoints_RBV;
	extern std::string const ANA_FlooredPercent_RBV;
	// ANALYSIS CPU USAGE
	extern std::string const ANA_CPU_RBV;
	extern std::string const ANA_CPU_CropSubMask_RBV;
	extern std::string const ANA_CPU_Npoint_RBV;
	extern std::string const ANA_CPU_Dot_RBV;
	// CAMERA SENSOR TEMPERATURE (1)
	extern std::string const CAM_Temperature_RBV;
	// PIXEL DEFINITIONS  (4)
	extern std::string const ANA_PixW_RBV;
	extern std::string const ANA_PixH_RBV;
	extern std::string const ANA_PixMM;
	extern std::string const ANA_PixMM_RBV;
	// ANSLYSI RESULTS (13)
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
	extern std::string const ANA_MMResults_RBV;
	// MASK DEFINITIONS (8)
	extern std::string const ANA_MaskXCenter_RBV;
	extern std::string const ANA_MaskXCenter;
	extern std::string const ANA_MaskYCenter_RBV;
	extern std::string const ANA_MaskYCenter;
	extern std::string const ANA_MaskXRad_RBV;
	extern std::string const ANA_MaskXRad;
	extern std::string const ANA_MaskYRad_RBV;
	extern std::string const ANA_MaskYRad;
	// ??? 
	extern std::string const ANA_CenterX_RBV;
	extern std::string const ANA_CenterX;
	extern std::string const ANA_CenterY_RBV;
	extern std::string const ANA_CenterY;
	// ANALYSIS BACKGROUND MASK (4)
	extern std::string const ANA_NewBkgrnd;
	extern std::string const ANA_NewBkgrnd_RBV;
	extern std::string const ANA_UseBkgrnd;
	extern std::string const ANA_UseBkgrnd_RBV;
	// ANALYSIS NPOINT SCALING (2)
	extern std::string const ANA_UseNPoint;
	extern std::string const ANA_UseNPoint_RBV;
	extern std::string const ANA_NPointStepSize_RBV;
	extern std::string const ANA_NPointStepSize;
	// LED STATUS (3)
	extern std::string const LED_Sta;
	extern std::string const LED_Off;
                                 
	// anlaysis overlay (6)
	extern std::string const ANA_OVERLAY_1_CROSS;
	extern std::string const ANA_OVERLAY_2_RESULT;
	extern std::string const ANA_OVERLAY_3_MASK;
	extern std::string const ANA_OVERLAY_1_CROSS_RBV;
	extern std::string const ANA_OVERLAY_2_RESULT_RBV;
	extern std::string const ANA_OVERLAY_3_MASK_RBV;

	extern std::string const LED_On;
	extern std::string const CAM_BlackLevel;
	extern std::string const CAM_BlackLevel_RBV;
	extern std::string const CAM_Gain;
	extern std::string const CAM_Gain_RBV;



	// MAGIC IS NOT USED (yet!!!) 
/*
	extern std::string const MAGICK_FileName;
	extern std::string const MAGICK_FilePath;
	extern std::string const MAGICK_FileNumber;
	extern std::string const MAGICK_Capture;
	extern std::string const MAGICK_WriteFile;
	extern std::string const MAGICK_NumCapture;
	extern std::string const MAGICK_NumCaptured_RBV;
	extern std::string const MAGICK_WriteFile_RBV;
	extern std::string const MAGICK_WriteStatus;
	extern std::string const MAGICK_WriteMessage;
	extern std::string const MAGICK_Capture_RBV;
	extern std::string const MAGICK_NumCapture_RBV;
*/
	extern std::vector<std::string> cameraRecordList;
	extern std::vector<std::string> cameraMonitorRecordsList;
}

#endif // CAMERA_PV_RECORDS_