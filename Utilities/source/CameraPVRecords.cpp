#include <CameraPVRecords.h>


namespace CameraRecords
{
	std::string const HDFB_image_buffer_trigger		    = "HDFB_image_buffer_trigger";
	std::string const HDFB_image_buffer_filepath		    = "HDFB_image_buffer_filepath";
	std::string const HDFB_image_buffer_filepath_RBV      = "HDFB_image_buffer_filepath_RBV";
	std::string const HDFB_image_buffer_filename		    = "HDFB_image_buffer_filename";
	std::string const HDFB_image_buffer_filename_RBV	    = "HDFB_image_buffer_filename_RBV";
	std::string const HDFB_image_buffer_filenumber	    = "HDFB_image_buffer_filenumber";
	std::string const HDFB_image_buffer_filenumber_RBV    = "HDFB_image_buffer_filenumber_RBV";
	std::string const ROI1_MinX_RBV				    = "ROI1_MinX_RBV"				;
	std::string const ROI1_MinY_RBV				    = "ROI1_MinY_RBV"				;
	std::string const ROI1_MinX					    = "ROI1_MinX"					;
	std::string const ROI1_MinY					    = "ROI1_MinY"					;
	std::string const ROI1_SizeX_RBV				= "ROI1_SizeX_RBV"			;
	std::string const ROI1_SizeY_RBV				= "ROI1_SizeY_RBV"			;
	std::string const ROI1_SizeX					= "ROI1_SizeX"				;
	std::string const ROI1_SizeY					= "ROI1_SizeY"				;
	std::string const ROI1_ImageData_RBV			= "ROI1_ImageData_RBV"		;
	std::string const ROIandMask_SetX				= "ROIandMask_SetX"			;
	std::string const ROIandMask_SetY				= "ROIandMask_SetY"			;
	std::string const ROIandMask_SetXrad			= "ROIandMask_SetXrad"		;
	std::string const ROIandMask_SetYrad			= "ROIandMask_SetYrad"		;
	std::string const HDF_WriteStatus				= "HDF_WriteStatus"			;
	std::string const HDF_WriteMessage				= "HDF_WriteMessage"		;
	std::string const HDF_Capture					= "HDF_Capture"				;
	std::string const HDF_Capture_RBV				= "HDF_Capture_RBV"			;
	std::string const HDF_NumCapture				= "HDF_NumCapture"			;
	std::string const HDF_NumCapture_RBV			= "HDF_NumCapture_RBV"		;
	std::string const HDF_FilePath					= "HDF_FilePath"			;
	std::string const HDF_FilePath_RBV				= "HDF_FilePath_RBV"		;
	std::string const HDF_FileName					= "HDF_FileName"			;
	std::string const HDF_FileName_RBV				= "HDF_FileName_RBV"		;
	std::string const HDF_FileNumber				= "HDF_FileNumber"			;
	std::string const HDF_FileNumber_RBV			= "HDF_FileNumber_RBV"		;
	std::string const HDF_WriteFile				    = "HDF_WriteFile"				;
	std::string const HDF_WriteFile_RBV			    = "HDF_WriteFile_RBV"			;
	std::string const CAM_Acquire_RBV				= "CAM_Acquire_RBV"			;
	std::string const CAM_Start_Acquire		        = "CAM_Start_Acquire"			;
	std::string const CAM_Stop_Acquire				= "CAM_Stop_Acquire"		;
	std::string const CAM_Active_Count				= "CAM_Active_Count"		;
	std::string const CAM_Active_Limit				= "CAM_Active_Limit"		;
	std::string const CAM_AcquireTime_RBV			= "CAM_AcquireTime_RBV"		;
	std::string const CAM_AcquirePeriod_RBV		    = "CAM_AcquirePeriod_RBV"		;
	std::string const CAM_ArrayRate_RBV			    = "CAM_ArrayRate_RBV"			;
	std::string const CAM2_ArrayData				= "CAM2_ArrayData"			;
	std::string const CAM_Gain						= "CAM_Gain"				;
	std::string const CAM_Gain_RBV					= "CAM_Gain_RBV"			;
	std::string const CAM_BlackLevel				= "CAM_BlackLevel"			;
	std::string const CAM_BlackLevel_RBV			= "CAM_BlackLevel_RBV"		;
	std::string const ANA_EnableCallbacks			= "ANA_EnableCallbacks"		;
	std::string const ANA_EnableCallbacks_RBV		= "ANA_EnableCallbacks_RBV"	;
	std::string const ANA_UseFloor					= "ANA_UseFloor"			;
	std::string const ANA_UseFloor_RBV				= "ANA_UseFloor_RBV"		;
	std::string const ANA_FloorLevel				= "ANA_FloorLevel"			;
	std::string const ANA_FloorLevel_RBV			= "ANA_FloorLevel_RBV"		;
	std::string const ANA_FlooredPoints_RBV		    = "ANA_FlooredPoints_RBV"		;
	std::string const ANA_FlooredPercent_RBV		= "ANA_FlooredPercent_RBV"	;
	std::string const ANA_CPU_RBV					= "ANA_CPU_RBV"				;
	std::string const ANA_CPU_CropSubMask_RBV		= "ANA_CPU_CropSubMask_RBV"	;
	std::string const ANA_CPU_Npoint_RBV			= "ANA_CPU_Npoint_RBV"		;
	std::string const ANA_CPU_Dot_RBV				= "ANA_CPU_Dot_RBV"			;
	std::string const CAM_Temperature_RBV			= "CAM_Temperature_RBV"		;
	std::string const ANA_PixW_RBV					= "ANA_PixW_RBV"			;
	std::string const ANA_PixH_RBV					= "ANA_PixH_RBV"			;
	std::string const ANA_PixMM					    = "ANA_PixMM"					;
	std::string const ANA_PixMM_RBV				    = "ANA_PixMM_RBV"				;
	std::string const ANA_X_RBV					    = "ANA_X_RBV"					;
	std::string const ANA_Y_RBV					    = "ANA_Y_RBV"					;
	std::string const ANA_SigmaX_RBV				= "ANA_SigmaX_RBV"			;
	std::string const ANA_SigmaY_RBV				= "ANA_SigmaY_RBV"			;
	std::string const ANA_CovXY_RBV			        = "ANA_CovXY_RBV"				;
	std::string const ANA_AvgIntensity_RBV			= "ANA_AvgIntensity_RBV"	;
	std::string const ANA_Intensity_RBV			    = "ANA_Intensity_RBV"			;
	std::string const ANA_XPix_RBV					= "ANA_XPix_RBV"			;
	std::string const ANA_YPix_RBV					= "ANA_YPix_RBV"			;
	std::string const ANA_SigmaXPix_RBV			    = "ANA_SigmaXPix_RBV"			;
	std::string const ANA_SigmaYPix_RBV			    = "ANA_SigmaYPix_RBV"			;
	std::string const ANA_CovXYPix_RBV				= "ANA_CovXYPix_RBV"		;
	std::string const ANA_PixelResults_RBV			= "ANA_PixelResults_RBV"	;
	std::string const ANA_MMResults_RBV				= "ANA_MMResults_RBV"	;
	std::string const ANA_MaskXCenter_RBV			= "ANA_MaskXCenter_RBV"		;
	std::string const ANA_MaskXCenter				= "ANA_MaskXCenter"			;
	std::string const ANA_MaskYCenter_RBV			= "ANA_MaskYCenter_RBV"		;
	std::string const ANA_MaskYCenter				= "ANA_MaskYCenter"			;
	std::string const ANA_MaskXRad_RBV				= "ANA_MaskXRad_RBV"		;
	std::string const ANA_MaskXRad					= "ANA_MaskXRad"			;
	std::string const ANA_MaskYRad_RBV				= "ANA_MaskYRad_RBV"		;
	std::string const ANA_MaskYRad					= "ANA_MaskYRad"			;
	std::string const ANA_CenterX_RBV				= "ANA_CenterX_RBV"			;
	std::string const ANA_CenterX					= "ANA_CenterX"				;
	std::string const ANA_CenterY_RBV				= "ANA_CenterY_RBV"			;
	std::string const ANA_CenterY					= "ANA_CenterY"				;
	std::string const ANA_NewBkgrnd				    = "ANA_NewBkgrnd"				;
	std::string const ANA_NewBkgrnd_RBV			    = "ANA_NewBkgrnd_RBV"			;
	std::string const ANA_UseBkgrnd				    = "ANA_UseBkgrnd"				;
	std::string const ANA_UseBkgrnd_RBV			    = "ANA_UseBkgrnd_RBV"			;
	std::string const ANA_UseNPoint				    = "ANA_UseNPoint"				;
	std::string const ANA_UseNPoint_RBV			    = "ANA_UseNPoint_RBV"			;
	std::string const ANA_NPointStepSize_RBV		= "ANA_NPointStepSize_RBV"	;
	std::string const ANA_NPointStepSize			= "ANA_NPointStepSize"	;
	std::string const LED_Sta						= "LED_Sta"					;
	std::string const LED_Off						= "LED_Off"					;
	std::string const LED_On						= "LED_On"					;

	std::string const ANA_OVERLAY_1_CROSS = "ANA_OVERLAY_1_CROSS";
	std::string const ANA_OVERLAY_2_RESULT = "ANA_OVERLAY_2_RESULT";
	std::string const ANA_OVERLAY_3_MASK = "ANA_OVERLAY_3_MASK";
	std::string const ANA_OVERLAY_1_CROSS_RBV = "ANA_OVERLAY_1_CROSS_RBV";
	std::string const ANA_OVERLAY_2_RESULT_RBV = "ANA_OVERLAY_2_RESULT_RBV";
	std::string const ANA_OVERLAY_3_MASK_RBV = "ANA_OVERLAY_3_MASK_RBV";
	 
	std::vector<std::string> cameraRecordList = {
	HDFB_image_buffer_trigger               ,HDFB_image_buffer_filepath 			  ,HDFB_image_buffer_filepath_RBV 		  ,
	HDFB_image_buffer_filename 			  ,HDFB_image_buffer_filename_RBV 		  ,HDFB_image_buffer_filenumber 			  ,
	HDFB_image_buffer_filenumber_RBV 		  ,ROI1_MinX_RBV 					  ,ROI1_MinY_RBV 					  ,
	ROI1_MinX 						  ,ROI1_MinY 						  ,ROI1_SizeX_RBV					  ,
	ROI1_SizeY_RBV					  ,ROI1_SizeX						  ,ROI1_SizeY						  ,
	ROI1_ImageData_RBV				  ,ROIandMask_SetX					  ,ROIandMask_SetY					  ,
	ROIandMask_SetXrad				  ,ROIandMask_SetYrad				  ,HDF_WriteStatus					  ,
	HDF_WriteMessage				  ,HDF_Capture						  ,HDF_Capture_RBV					  ,
	HDF_NumCapture					  ,HDF_NumCapture_RBV				  ,HDF_FilePath					  ,
	HDF_FilePath_RBV				  ,HDF_FileName						   ,HDF_FileName_RBV				  ,
	HDF_FileNumber					  ,HDF_FileNumber_RBV				  ,HDF_WriteFile					  ,
	HDF_WriteFile_RBV				  ,CAM_Acquire_RBV					  ,CAM_Start_Acquire				  ,
	CAM_Stop_Acquire				  ,CAM_AcquireTime_RBV				  ,CAM_AcquirePeriod_RBV			  ,
	CAM_ArrayRate_RBV				  ,CAM2_ArrayData					  ,CAM_Gain						  ,
	CAM_Gain_RBV					  ,CAM_BlackLevel					  ,CAM_BlackLevel_RBV				  ,
	ANA_EnableCallbacks				  ,ANA_EnableCallbacks_RBV			  ,ANA_UseFloor					  ,
	ANA_UseFloor_RBV				  ,ANA_FloorLevel					  ,ANA_FloorLevel_RBV				  ,
	ANA_FlooredPoints_RBV			  ,ANA_FlooredPercent_RBV			  ,ANA_CPU_RBV						  ,
	ANA_CPU_CropSubMask_RBV			  ,ANA_CPU_Npoint_RBV				  ,ANA_CPU_Dot_RBV					  ,
	CAM_Temperature_RBV				  ,ANA_PixW_RBV				     	  ,ANA_PixH_RBV					  ,
	ANA_PixMM						  ,ANA_PixMM_RBV					  ,ANA_X_RBV						  ,
	ANA_Y_RBV						  ,ANA_SigmaX_RBV					  ,ANA_SigmaY_RBV					  ,
	ANA_CovXY_RBV					  ,ANA_AvgIntensity_RBV			      ,ANA_Intensity_RBV				  ,
	ANA_XPix_RBV					  ,ANA_YPix_RBV					      ,ANA_SigmaXPix_RBV				  ,
	ANA_SigmaYPix_RBV				  ,ANA_CovXYPix_RBV				      ,ANA_PixelResults_RBV			  ,
	ANA_MMResults_RBV,
	ANA_MaskXCenter_RBV				  ,ANA_MaskXCenter					  ,ANA_MaskYCenter_RBV				  ,
	ANA_MaskYCenter					  ,ANA_MaskXRad_RBV				      ,ANA_MaskXRad					  ,
	ANA_MaskYRad_RBV				  ,ANA_MaskYRad					      ,ANA_CenterX_RBV					  ,
	ANA_CenterX						  ,ANA_CenterY_RBV					  ,ANA_CenterY						  ,
	ANA_NewBkgrnd					  ,ANA_NewBkgrnd_RBV				  ,ANA_UseBkgrnd					  ,
	ANA_UseBkgrnd_RBV,				  
	ANA_UseNPoint					  ,ANA_UseNPoint_RBV				  ,
	ANA_NPointStepSize_RBV			  ,ANA_NPointStepSize				  ,LED_Sta							  ,
	LED_Off							  ,LED_On,
	ANA_OVERLAY_1_CROSS_RBV, ANA_OVERLAY_2_RESULT_RBV, ANA_OVERLAY_3_MASK_RBV,
	ANA_OVERLAY_1_CROSS, ANA_OVERLAY_2_RESULT, ANA_OVERLAY_3_MASK,
	CAM_Active_Limit, CAM_Active_Count
	};
         
	std::vector<std::string> cameraMonitorRecordsList
	{
		HDFB_image_buffer_filepath_RBV            ,		HDFB_image_buffer_filename_RBV			,		HDFB_image_buffer_filenumber_RBV			,  //1
		HDF_Capture_RBV						,		HDF_NumCapture_RBV					,		HDF_FilePath_RBV					,  //4
		HDF_FileName_RBV					,		HDF_FileNumber_RBV					,		HDF_WriteFile_RBV					,  //5
		ROI1_MinX_RBV						,		ROI1_MinY_RBV						,		ROI1_SizeX_RBV						,  //2
		ROI1_SizeY_RBV						,		HDF_WriteStatus						,  		HDF_WriteMessage					,  // 58
		CAM_Acquire_RBV						,		CAM_AcquireTime_RBV					,		CAM_AcquirePeriod_RBV				,  //6
		CAM_ArrayRate_RBV					,		CAM_Gain_RBV						,		CAM_BlackLevel_RBV					,  //7
		ANA_EnableCallbacks_RBV				,		ANA_UseFloor_RBV					,		ANA_FloorLevel_RBV					,  //8
		ANA_FlooredPoints_RBV				,		ANA_FlooredPercent_RBV				,		ANA_CPU_RBV							,  //9
		ANA_CPU_CropSubMask_RBV				,		ANA_CPU_Npoint_RBV					,		ANA_CPU_Dot_RBV						,  //10
		CAM_Temperature_RBV					,		ANA_PixW_RBV						,		ANA_PixH_RBV						,  //11
		ANA_PixMM_RBV						,		ANA_X_RBV							,		ANA_Y_RBV							,  //12
		ANA_SigmaX_RBV						,		ANA_SigmaY_RBV						,		ANA_CovXY_RBV						,  //13
		ANA_AvgIntensity_RBV				,		ANA_Intensity_RBV					,		ANA_XPix_RBV						,  //14
		ANA_YPix_RBV						,		ANA_SigmaXPix_RBV					,		ANA_SigmaYPix_RBV					,  //15
		ANA_CovXYPix_RBV					,		ANA_PixelResults_RBV				,		ANA_MMResults_RBV					,  //16
		ANA_MaskYCenter_RBV					,		ANA_MaskXRad_RBV					,		ANA_MaskYRad_RBV					,  //17
		ANA_CenterX_RBV						,		ANA_CenterY_RBV						,		ANA_NewBkgrnd_RBV					,  //18
		ANA_OVERLAY_1_CROSS_RBV				,		ANA_OVERLAY_2_RESULT_RBV			,		ANA_OVERLAY_3_MASK_RBV				,  //20	
		ANA_MaskXCenter_RBV,
		//// ATM these ones don't come with an _RBV suffix for a read only PV 
		LED_Sta,	      // 59
		CAM_Active_Limit, // 59
		CAM_Active_Count  // 59
	};

}
