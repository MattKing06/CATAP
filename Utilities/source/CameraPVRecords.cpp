#include <CameraPVRecords.h>


namespace CameraRecords
{
	std::string const HDFB_Buffer_Trigger = "HDFB_Buffer_Trigger";
	std::string const HDFB_Buffer_FilePath = "HDFB_Buffer_FilePath";
	std::string const HDFB_Buffer_FilePath_RBV = "HDFB_Buffer_FilePath_RBV";
	std::string const HDFB_Buffer_FileName = "HDFB_Buffer_FileName";
	std::string const HDFB_Buffer_FileName_RBV = "HDFB_Buffer_FileName_RBV";
	std::string const HDFB_Buffer_FileNumber = "HDFB_Buffer_FileNumber";
	std::string const HDFB_Buffer_FileNumber_RBV = "HDFB_Buffer_FileNumber_RBV";
	std::string const ROI1_MinY_RBV = "ROI1_MinY_RBV";
	std::string const ROI1_MinX_RBV = "ROI1_MinX_RBV";
	std::string const ROI1_MinX = "ROI1_MinX";
	std::string const ROI1_MinY = "ROI1_MinY";
	std::string const ROI1_SizeX_RBV = "ROI1_SizeX_RBV";
	std::string const ROI1_SizeY_RBV = "ROI1_SizeY_RBV";
	std::string const ROI1_SizeX = "ROI1_SizeX";
	std::string const ROI1_SizeY = "ROI1_SizeY";
	std::string const ROI1_ImageData_RBV = "ROI1_ImageData_RBV";
	std::string const ROIandMask_SetX = "ROIandMask_SetX";
	std::string const ROIandMask_SetY = "ROIandMask_SetY";
	std::string const ROIandMask_SetXrad = "ROIandMask_SetXrad";
	std::string const ROIandMask_SetYrad = "ROIandMask_SetYrad";
	std::string const ANA_UseFloor = "ANA_UseFloor";
	std::string const ANA_UseFloor_RBV = "ANA_UseFloor_RBV";
	std::string const ANA_FloorLevel = "ANA_FloorLevel";
	std::string const ANA_FloorLevel_RBV = "ANA_FloorLevel_RBV";
	std::string const ANA_FlooredPoints_RBV = "ANA_FlooredPoints_RBV";
	std::string const ANA_FlooredPercent_RBV = "ANA_FlooredPercent_RBV";
	std::string const ANA_CPU_RBV = "ANA_CPU_RBV";
	std::string const ANA_CPU_CropSubMask_RBV = "ANA_CPU_CropSubMask_RBV";
	std::string const ANA_CPU_Npoint_RBV = "ANA_CPU_Npoint_RBV";
	std::string const ANA_CPU_Dot_RBV = "ANA_CPU_Dot_RBV";
	std::string const ANA_PixW_RBV = "ANA_PixW_RBV";
	std::string const ANA_PixH_RBV = "ANA_PixH_RBV";
	std::string const HDF_WriteFile_RBV = "HDF_WriteFile_RBV";
	std::string const HDF_WriteStatus = "HDF_WriteStatus";
	std::string const HDF_WriteMessage = "HDF_WriteMessage";
	std::string const HDF_NumCaptured_RBV = "HDF_NumCaptured_RBV";
	std::string const HDF_Capture_RBV = "HDF_Capture_RBV";
	std::string const CAM_Acquire_RBV = "CAM_Acquire_RBV";
	std::string const HDF_NumCapture_RBV = "HDF_NumCapture_RBV";
	std::string const MAGICK_NumCaptured_RBV = "MAGICK_NumCaptured_RBV";
	std::string const MAGICK_WriteFile_RBV = "MAGICK_WriteFile_RBV";
	std::string const MAGICK_WriteStatus = "MAGICK_WriteStatus";
	std::string const MAGICK_WriteMessage = "MAGICK_WriteMessage";
	std::string const MAGICK_Capture_RBV = "MAGICK_Capture_RBV";
	std::string const MAGICK_NumCapture_RBV = "MAGICK_NumCapture_RBV";
	std::string const ANA_StepSize_RBV = "ANA_StepSize_RBV";
	std::string const ANA_EnableCallbacks_RBV = "ANA_EnableCallbacks_RBV";
	std::string const ANA_UseBkgrnd = "ANA_UseBkgrnd";
	std::string const ANA_X_RBV = "ANA_X_RBV";
	std::string const ANA_Y_RBV = "ANA_Y_RBV";
	std::string const ANA_SigmaX_RBV = "ANA_SigmaX_RBV";
	std::string const ANA_SigmaY_RBV = "ANA_SigmaY_RBV";
	std::string const ANA_CovXY_RBV = "ANA_CovXY_RBV";
	std::string const ANA_AvgIntensity_RBV = "ANA_AvgIntensity_RBV";
	std::string const ANA_Intensity_RBV = "ANA_Intensity_RBV";
	std::string const ANA_XPix_RBV = "ANA_XPix_RBV";
	std::string const ANA_YPix_RBV = "ANA_YPix_RBV";
	std::string const ANA_SigmaXPix_RBV = "ANA_SigmaXPix_RBV";
	std::string const ANA_SigmaYPix_RBV = "ANA_SigmaYPix_RBV";
	std::string const ANA_CovXYPix_RBV = "ANA_CovXYPix_RBV";
	std::string const ANA_PixelResults_RBV = "ANA_PixelResults_RBV";
	std::string const ANA_MaskXCenter_RBV = "ANA_MaskXCenter_RBV";
	std::string const ANA_MaskYCenter_RBV = "ANA_MaskYCenter_RBV";
	std::string const ANA_MaskXRad_RBV = "ANA_MaskXRad_RBV";
	std::string const ANA_MaskYRad_RBV = "ANA_MaskYRad_RBV";
	std::string const ANA_CenterX_RBV = "ANA_CenterX_RBV";
	std::string const ANA_CenterY_RBV = "ANA_CenterY_RBV";
	std::string const ANA_PixMM_RBV = "ANA_PixMM_RBV";
	std::string const CAM_AcquireTime_RBV = "CAM_AcquireTime_RBV";
	std::string const CAM_AcquirePeriod_RBV = "CAM_AcquirePeriod_RBV";
	std::string const CAM_ArrayRate_RBV = "CAM_ArrayRate_RBV";
	std::string const CAM_Temperature_RBV = "CAM_Temperature_RBV";
	std::string const HDF_FilePath = "HDF_FilePath";
	std::string const HDF_FileName = "HDF_FileName";
	std::string const HDF_FilePath_RBV = "HDF_FilePath_RBV";
	std::string const HDF_FileName_RBV = "HDF_FileName_RBV";
	std::string const HDF_FileNumber = "HDF_FileNumber";
	std::string const HDF_WriteFile = "HDF_WriteFile";
	std::string const CAM_Acquire = "CAM_Acquire";
	std::string const HDF_Capture = "HDF_Capture";
	std::string const HDF_NumCapture = "HDF_NumCapture";
	std::string const MAGICK_FileName = "MAGICK_FileName";
	std::string const MAGICK_FilePath = "MAGICK_FilePath";
	std::string const MAGICK_FileNumber = "MAGICK_FileNumber";
	std::string const MAGICK_Capture = "MAGICK_Capture";
	std::string const MAGICK_WriteFile = "MAGICK_WriteFile";
	std::string const MAGICK_NumCapture = "MAGICK_NumCapture";
	std::string const ANA_StepSize = "ANA_StepSize";
	std::string const ANA_EnableCallbacks = "ANA_EnableCallbacks";
	std::string const ANA_NewBkgrnd = "ANA_NewBkgrnd";

	std::string const ANA_UseNPoint = "ANA_UseNPoint";
	std::string const CAM2_ArrayData = "CAM2_ArrayData";
	std::string const ANA_MaskXCenter = "ANA_MaskXCenter";
	std::string const ANA_MaskYCenter = "ANA_MaskYCenter";
	std::string const ANA_MaskXRad = "ANA_MaskXRad";
	std::string const ANA_MaskYRad = "ANA_MaskYRad";
	std::string const ANA_CenterX = "ANA_CenterX";
	std::string const ANA_CenterY = "ANA_CenterY";
	std::string const ANA_PixMM = "ANA_PixMM";
	std::string const LED_Sta = "LED_Sta";
	std::string const LED_Off = "LED_Off";
	std::string const LED_On = "LED_On";

	std::vector<std::string> cameraRecordList = { 
	HDFB_Buffer_Trigger,
	HDFB_Buffer_FilePath,
	HDFB_Buffer_FilePath_RBV,
	HDFB_Buffer_FileName,	
	HDFB_Buffer_FileName_RBV,
	HDFB_Buffer_FileNumber,
	HDFB_Buffer_FileNumber_RBV,
	ROI1_MinX_RBV,
	ROI1_MinY_RBV,
	ROI1_MinX,
	ROI1_MinY,
	ROI1_SizeX_RBV,
	ROI1_SizeY_RBV,
	ROI1_SizeX,
	ROI1_SizeY,
	ROI1_ImageData_RBV,
	ROIandMask_SetX,
	ROIandMask_SetY,
	ROIandMask_SetXrad,
	ROIandMask_SetYrad,
	ANA_UseFloor_RBV,
	ANA_FloorLevel_RBV,
	ANA_FlooredPoints_RBV,
	ANA_FlooredPercent_RBV,
	ANA_CPU_RBV,
	ANA_CPU_CropSubMask_RBV,
	ANA_CPU_Npoint_RBV,
	ANA_CPU_Dot_RBV,
	ANA_PixW_RBV,
	ANA_PixH_RBV,
	HDF_WriteFile_RBV,
	HDF_WriteStatus,
	HDF_WriteMessage,
	HDF_NumCaptured_RBV,
	HDF_Capture_RBV,
	CAM_Acquire_RBV,
	HDF_NumCapture_RBV,
	MAGICK_NumCaptured_RBV,
	MAGICK_WriteFile_RBV,
	MAGICK_WriteStatus,
	MAGICK_WriteMessage,
	MAGICK_Capture_RBV,
	MAGICK_NumCapture_RBV,
	ANA_StepSize_RBV,
	ANA_EnableCallbacks_RBV,
	ANA_UseBkgrnd,
	ANA_UseNPoint,
	ANA_X_RBV,
	ANA_Y_RBV ,
	ANA_SigmaX_RBV,
	ANA_SigmaY_RBV,
	ANA_CovXY_RBV,
	ANA_AvgIntensity_RBV,
	ANA_Intensity_RBV,
	ANA_XPix_RBV,
	ANA_YPix_RBV,
	ANA_SigmaXPix_RBV,
	ANA_SigmaYPix_RBV,
	ANA_CovXYPix_RBV,
	ANA_PixelResults_RBV,
	ANA_MaskXCenter_RBV ,
	ANA_MaskYCenter_RBV ,
	ANA_MaskXRad_RBV,
	ANA_MaskYRad_RBV,
	ANA_CenterX_RBV ,
	ANA_CenterY_RBV,
	ANA_PixMM_RBV ,
	CAM_AcquireTime_RBV,
	CAM_AcquirePeriod_RBV,
	CAM_ArrayRate_RBV ,
	CAM_Temperature_RBV,
	HDF_FilePath,
	HDF_FileName ,
	HDF_FileNumber,
	HDF_FileName_RBV ,
	HDF_FilePath_RBV,
	HDF_WriteFile ,
	CAM_Acquire ,
	HDF_Capture ,
	HDF_NumCapture  ,
	MAGICK_FileName,
	MAGICK_FilePath,
	MAGICK_FileNumber,
	MAGICK_Capture  ,
	MAGICK_WriteFile ,
	MAGICK_NumCapture ,
	ANA_StepSize ,
	ANA_EnableCallbacks,
	ANA_NewBkgrnd ,
	ANA_UseNPoint ,
	CAM2_ArrayData ,
	ANA_MaskXCenter,
	ANA_MaskYCenter,
	ANA_MaskXRad,
	ANA_MaskYRad,
	ANA_CenterX ,
	ANA_CenterY ,
	ANA_PixMM ,
	LED_Sta,
	LED_Off,
	LED_On };                  


	std::vector<std::string> cameraMonitorRecordsList
	{
		HDFB_Buffer_Trigger,
		HDFB_Buffer_FilePath_RBV,
		HDFB_Buffer_FileName_RBV,
		HDFB_Buffer_FileNumber_RBV,
		ROI1_MinX_RBV,
		ROI1_MinY_RBV,
		ROI1_SizeX_RBV,
		ROI1_SizeY_RBV,
		ROI1_ImageData_RBV,
		ROIandMask_SetX,
		ROIandMask_SetY,
		ROIandMask_SetXrad,
		ROIandMask_SetYrad,
		ANA_UseFloor_RBV,
		ANA_FloorLevel_RBV,
		ANA_FlooredPoints_RBV,
		ANA_FlooredPercent_RBV,
		ANA_CPU_RBV,
		ANA_CPU_CropSubMask_RBV,
		ANA_CPU_Npoint_RBV,
		ANA_CPU_Dot_RBV,
		ANA_PixW_RBV,
		ANA_PixH_RBV,
		HDF_WriteFile_RBV,
		HDF_WriteStatus,
		HDF_WriteMessage,
		HDF_FilePath_RBV,
		HDF_FileName_RBV,
		HDF_NumCaptured_RBV,
		HDF_Capture_RBV,
		CAM_Acquire_RBV,
		HDF_NumCapture_RBV,
		MAGICK_NumCaptured_RBV,
		MAGICK_WriteFile_RBV,
		MAGICK_WriteStatus,
		MAGICK_WriteMessage,
		MAGICK_Capture_RBV,
		MAGICK_NumCapture_RBV,
		ANA_StepSize_RBV,
		ANA_EnableCallbacks_RBV,
		ANA_X_RBV,
		ANA_Y_RBV,
		ANA_SigmaX_RBV,
		ANA_SigmaY_RBV,
		ANA_CovXY_RBV,
		ANA_AvgIntensity_RBV,
		ANA_Intensity_RBV,
		ANA_XPix_RBV,
		ANA_YPix_RBV,
		ANA_SigmaXPix_RBV,
		ANA_SigmaYPix_RBV,
		ANA_CovXYPix_RBV,
		ANA_PixelResults_RBV,
		ANA_MaskXCenter_RBV,
		ANA_MaskYCenter_RBV,
		ANA_MaskXRad_RBV,
		ANA_MaskYRad_RBV,
		ANA_CenterX_RBV,
		ANA_CenterY_RBV,
		ANA_PixMM_RBV,
		CAM_AcquireTime_RBV,
		CAM_AcquirePeriod_RBV,
		CAM_ArrayRate_RBV,
		CAM_Temperature_RBV,
		ANA_UseNPoint,
		ANA_UseBkgrnd


	};

};


