#ifndef EPICS_CAMERA_INTERFACE_H_
#define EPICS_CAMERA_INTERFACE_H_
#ifndef EPICS_INTERFACE_H_
#include "EPICSInterface.h"
#include <iomanip>
#endif 
#include "Camera.h"
#include <chrono> 
#include <iostream>
#include <GlobalStateEnums.h>
#include "CameraPVRecords.h"

// forward declaratiOn of Magnet class
// tells compiler that we will use this class.
class Camera;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSCameraInterface : public EPICSInterface
{
public:
    EPICSCameraInterface();
    EPICSCameraInterface(const EPICSCameraInterface& copyInterface);
    ~EPICSCameraInterface();
    // We need to a STATIC messenger so that the static call back functions can use it to print messages 
    static LoggingSystem s_messenger;
    void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
    // EPICS calls these functiOns to update a variable

	void debugMessagesOff();
	void debugMessagesOn();
	void messagesOff();
	void messagesOn();
   
static void update_HDFB_image_buffer_filepath_RBV(const struct event_handler_args args);
static void update_HDFB_image_buffer_filename_RBV(const struct event_handler_args args);
static void update_HDFB_image_buffer_filenumber_RBV(const struct event_handler_args args);
static void update_ROI1_MinX_RBV(const struct event_handler_args args);
static void update_ROI1_MinY_RBV(const struct event_handler_args args);
static void update_ROI1_SizeX_RBV(const struct event_handler_args args);
static void update_ROI1_SizeY_RBV(const struct event_handler_args args);
static void update_ROI1_ImageData_RBV(const struct event_handler_args args);
static void update_HDF_WriteStatus(const struct event_handler_args args);
static void update_HDF_Capture_RBV(const struct event_handler_args args);
static void update_HDF_NumCapture_RBV(const struct event_handler_args args);
static void update_HDF_FilePath_RBV(const struct event_handler_args args);
static void update_HDF_FileName_RBV(const struct event_handler_args args);
static void update_HDF_FileNumber_RBV(const struct event_handler_args args);
static void update_HDF_WriteFile_RBV(const struct event_handler_args args);
static void update_HDF_WriteMessage_RBV(const struct event_handler_args args);
static void update_CAM_Acquire_RBV(const struct event_handler_args args);
static void update_CAM_AcquireTime_RBV(const struct event_handler_args args);
static void update_CAM_AcquirePeriod_RBV(const struct event_handler_args args);
static void update_CAM_ArrayRate_RBV(const struct event_handler_args args);
static void update_CAM_Gain_RBV(const struct event_handler_args args);
static void update_CAM_BlackLevel_RBV(const struct event_handler_args args);
static void update_CAM2_ArrayData(const struct event_handler_args args);
static void update_ANA_EnableCallbacks_RBV(const struct event_handler_args args);
static void update_ANA_UseFloor_RBV(const struct event_handler_args args);
static void update_ANA_FloorLevel_RBV(const struct event_handler_args args);
static void update_ANA_FlooredPoints_RBV(const struct event_handler_args args);
static void update_ANA_FlooredPercent_RBV(const struct event_handler_args args);
static void update_ANA_CPU_RBV(const struct event_handler_args args);
static void update_ANA_CPU_CropSubMask_RBV(const struct event_handler_args args);
static void update_ANA_CPU_Npoint_RBV(const struct event_handler_args args);
static void update_ANA_CPU_Dot_RBV(const struct event_handler_args args);
static void update_CAM_Temperature_RBV(const struct event_handler_args args);
static void update_ANA_PixW_RBV(const struct event_handler_args args);
static void update_ANA_PixH_RBV(const struct event_handler_args args);
static void update_ANA_PixMM_RBV(const struct event_handler_args args);
static void update_ANA_X_RBV(const struct event_handler_args args);
static void update_ANA_Y_RBV(const struct event_handler_args args);
static void update_ANA_SigmaX_RBV(const struct event_handler_args args);
static void update_ANA_SigmaY_RBV(const struct event_handler_args args);
static void update_ANA_CovXY_RBV(const struct event_handler_args args);
static void update_ANA_AvgIntensity_RBV(const struct event_handler_args args);
static void update_ANA_Intensity_RBV(const struct event_handler_args args);
static void update_ANA_XPix_RBV(const struct event_handler_args args);
static void update_ANA_YPix_RBV(const struct event_handler_args args);
static void update_ANA_SigmaXPix_RBV(const struct event_handler_args args);
static void update_ANA_SigmaYPix_RBV(const struct event_handler_args args);
static void update_ANA_CovXYPix_RBV(const struct event_handler_args args);
static void update_ANA_PixelResults_RBV(const struct event_handler_args args);
static void update_ANA_MMResults_RBV(const struct event_handler_args args);
static void update_ANA_MaskXCenter_RBV(const struct event_handler_args args);
static void update_ANA_MaskYCenter_RBV(const struct event_handler_args args);
static void update_ANA_MaskXRad_RBV(const struct event_handler_args args);
static void update_ANA_MaskYRad_RBV(const struct event_handler_args args);
static void update_ANA_CenterX_RBV(const struct event_handler_args args);
static void update_ANA_CenterY_RBV(const struct event_handler_args args);
static void update_ANA_NewBkgrnd_RBV(const struct event_handler_args args);
static void update_ANA_UseBkgrnd_RBV(const struct event_handler_args args);
static void update_ANA_UseNPoint_RBV(const struct event_handler_args args);
static void update_ANA_NPointStepSize_RBV(const struct event_handler_args args);
static void update_LED_Sta(const struct event_handler_args args);


static void update_CAM_Active_Count(const struct event_handler_args args);
static void update_CAM_Active_Limit(const struct event_handler_args args);

static void update_ANA_OVERLAY_1_CROSS_RBV(const struct event_handler_args args);
static void update_ANA_OVERLAY_2_RESULT_RBV(const struct event_handler_args args);
static void update_ANA_OVERLAY_3_MASK_RBV(const struct event_handler_args args);

static void update_OVERLAY_CROSS_HAIR(const struct event_handler_args args);
static void update_OVERLAY_CENTRE_OF_MASS(const struct event_handler_args args);
static void update_OVERLAY_MASK(const struct event_handler_args args);

    
//bool setUseFloor(const std::map<std::string, pvStruct>& pvStructs) { return putValue_flushio<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseFloor), GlobalConstants::one_ushort); }
//bool setDoNotUseFloor(const std::map<std::string, pvStruct>& pvStructs) { return putValue_flushio<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseFloor), GlobalConstants::zero_ushort); }

    static bool get_camera_array(std::vector<long>& data_vec, const pvStruct& pvs, size_t count)
    {
        s_messenger.printDebugMessage("getArrayValue Function");
        if (ca_state(pvs.CHID) == cs_conn)
        {
            s_messenger.printDebugMessage("ca_array_get ... ");
            int status = ca_array_get(DBR_LONG, count, pvs.CHID, &data_vec[0]);
            return sendToEPICSm2("this is from getArrayValue");
        }
        return false;
    }

};
#endif //EPICS_CAMERA_INTERFACE_H_