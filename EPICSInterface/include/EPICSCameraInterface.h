#ifndef EPICS_CAMERA_INTERFACE_H_
#define EPICS_CAMERA_INTERFACE_H_
#ifndef EPICS_INTERFACE_H_
#include "EPICSInterface.h"
#endif 
#include "Camera.h"
#include <iostream>
#include <GlobalStateEnums.h>

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
    static LoggingSystem messenger;
    void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
    // EPICS calls these functiOns to update a variable
    static void update_HDF_WriteFile_RBV(const struct event_handler_args args);
    static void update_HDF_WriteStatus(const struct event_handler_args args);
    static void update_HDF_FilePath_RBV(const struct event_handler_args args);
    static void update_HDF_FileName_RBV(const struct event_handler_args args);
    static void update_HDF_FileNumber_RBV(const struct event_handler_args args);
    static void update_HDF_WriteMessage(const struct event_handler_args args);
    static void update_HDF_NumCaptured_RBV(const struct event_handler_args args);
    static void update_HDF_Capture_RBV(const struct event_handler_args args);
    static void update_CAM_Acquire_RBV(const struct event_handler_args args);
    static void update_HDF_NumCapture_RBV(const struct event_handler_args args);
    static void update_MAGICK_NumCaptured_RBV(const struct event_handler_args args);
    static void update_MAGICK_WriteFile_RBV(const struct event_handler_args args);
    static void update_MAGICK_WriteStatus(const struct event_handler_args args);
    static void update_MAGICK_WriteMessage(const struct event_handler_args args);
    static void update_MAGICK_Capture_RBV(const struct event_handler_args args);
    static void update_MAGICK_NumCapture_RBV(const struct event_handler_args args);
    static void update_ANA_StepSize_RBV(const struct event_handler_args args);
    static void update_ANA_EnableCallbacks_RBV(const struct event_handler_args args);
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
    static void update_ANA_MaskXCenter_RBV(const struct event_handler_args args);
    static void update_ANA_MaskYCenter_RBV(const struct event_handler_args args);
    static void update_ANA_MaskXRad_RBV(const struct event_handler_args args);
    static void update_ANA_MaskYRad_RBV(const struct event_handler_args args);
    static void update_ANA_CenterX_RBV(const struct event_handler_args args);
    static void update_ANA_CenterY_RBV(const struct event_handler_args args);
    static void update_ANA_PixMM_RBV(const struct event_handler_args args);
    static void update_CAM_AcquireTime_RBV(const struct event_handler_args args);
    static void update_CAM_AcquirePeriod_RBV(const struct event_handler_args args);
    static void update_CAM_ArrayRate_RBV(const struct event_handler_args args);
    static void update_CAM_Temperature_RBV(const struct event_handler_args args);
    //static void update_ANA_UseNPoint(const struct event_handler_args args);

    //static void update_HDFB_Buffer_FilePath(const struct event_handler_args args);
    static void update_HDFB_Buffer_Trigger(const struct event_handler_args args);
    static void update_HDFB_Buffer_FilePath_RBV(const struct event_handler_args args);
    static void update_HDFB_Buffer_FileName_RBV(const struct event_handler_args args);
    static void update_HDFB_Buffer_FileNumber_RBV(const struct event_handler_args args);
    static void update_ROI1_MinX_RBV(const struct event_handler_args args);
    static void update_ROI1_MinY_RBV(const struct event_handler_args args);
    static void update_ROI1_SizeX_RBV(const struct event_handler_args args);
    static void update_ROI1_SizeY_RBV(const struct event_handler_args args);
    static void update_ROI1_ImageData_RBV(const struct event_handler_args args);
    //static void update_ROIandMask_SetX(const struct event_handler_args args);
    //static void update_ROIandMask_SetY(const struct event_handler_args args);
    //static void update_ROIandMask_SetXrad(const struct event_handler_args args);
    //static void update_ROIandMask_SetYrad(const struct event_handler_args args);
    static void update_ANA_UseFloor_RBV(const struct event_handler_args args);
    static void update_ANA_FloorLevel_RBV(const struct event_handler_args args);
    static void update_ANA_FlooredPoints_RBV(const struct event_handler_args args);
    static void update_ANA_FlooredPercent_RBV(const struct event_handler_args args);
    static void update_ANA_CPU_RBV(const struct event_handler_args args);
    static void update_ANA_CPU_CropSubMask_RBV(const struct event_handler_args args);
    static void update_ANA_CPU_Npoint_RBV(const struct event_handler_args args);
    static void update_ANA_CPU_Dot_RBV(const struct event_handler_args args);
    static void update_ANA_PixW_RBV(const struct event_handler_args args);
    static void update_ANA_PixH_RBV(const struct event_handler_args args);
    static void update_ANA_UseBkgrnd(const struct event_handler_args args);
    static void update_ANA_UseNPoint(const struct event_handler_args args);
    //static void update_ANA_PixMM_RBV(const struct event_handler_args args);
    static void update_LED_Sta(const struct event_handler_args args);

    static void update_BlackLevel_RBV(const struct event_handler_args args);
    static void update_Gain_RBV(const struct event_handler_args args);

    static void update_OVERLAY_CROSS_HAIR(const struct event_handler_args args);
    static void update_OVERLAY_CENTRE_OF_MASS(const struct event_handler_args args);
    static void update_OVERLAY_MASK(const struct event_handler_args args);


};
#endif //EPICS_CAMERA_INTERFACE_H_