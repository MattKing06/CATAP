#ifndef EPICS_CAMERA_INTERFACE_H_
#define EPICS_CAMERA_INTERFACE_H_
#include <EPICSInterface.h>
#include "Camera.h"


class Camera;
typedef void(*updateFunctionPtr)(struct event_handler_args args);
class EPICSCameraInterface : public EPICSInterface
{
public:
	EPICSCameraInterface();
    EPICSCameraInterface(const EPICSCameraInterface& copyInterface);
    ~EPICSCameraInterface();
	std::string ownerName;

    void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;


    //"HDF:WriteFile_RBV", 
    //"HDF:WriteStatus", 
    //"HDF:WriteMessage", 
    //"HDF:NumCaptured_RBV", 
    //"HDF:Capture_RBV",
    //"CAM:Acquire_RBV", 
    //"HDF:NumCapture_RBV", 
    //"MAGICK:NumCaptured_RBV", 
    //"MAGICK:WriteFile_RBV", 
    //"MAGICK:WriteStatus",
    //"MAGICK:WriteMessage", 
    //"MAGICK:Capture_RBV", 
    //"MAGICK:NumCapture_RBV", 

    // analysis settings
    //"ANA:StepSize_RBV", 
    //"ANA:EnableCallbacks_RBV",
    //"ANA:UseBkgrnd", 
    //"ANA:UseNPoint", 


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
    // EPICS calls these functiOns to update a variable
    static void updateX_RBV(const struct event_handler_args args);
    static void updateY_RBV(const struct event_handler_args args);
    static void updateSigmaX_RBV(const struct event_handler_args args);
    static void updateSigmaY_RBV(const struct event_handler_args args);
    static void updateCovXY_RBV(const struct event_handler_args args);
    static void updateXPix_RBV(const struct event_handler_args args);
    static void updateYPix_RBV(const struct event_handler_args args);
    static void updateSigmaXPix_RBV(const struct event_handler_args args);
    static void updateSigmaYPix_RBV(const struct event_handler_args args);
    static void updateCovXYPix_RBV(const struct event_handler_args args);
    static void updateAvgIntensity_RBV(const struct event_handler_args args);
    static void updateIntensity_RBV(const struct event_handler_args args);
    static void updatePixMM_RBV(const struct event_handler_args args);



       
    // analysis settings
    //"ANA:MaskXCenter_RBV", 
    //"ANA:MaskYCenter_RBV",
    //"ANA:MaskXRad_RBV", 
    //"ANA:MaskYRad_RBV", 
    //"ANA:CenterX_RBV", 
    //"ANA:CenterY_RBV", 
    //"ANA:StepSize", 
    //"ANA:EnableCallbacks", 
    //"ANA:NewBkgrnd", 
    //"ANA:UseBkgrnd", 
    //"ANA:UseNPoint",
    //"ANA:MaskXCenter", 
    //"ANA:MaskYCenter", 
    //"ANA:MaskXRad", 
    //"ANA:MaskYRad", 
    //"ANA:CenterX",
    //"ANA:CenterY", 
    //"ANA:PixMM"

    // cam on/off
    //"CAM:AcquireTime_RBV", 
    //"CAM:AcquirePeriod_RBV", 
    //"CAM:ArrayRate_RBV", 
    //"CAM:Temperature_RBV", 
    
    // cam data 
    //"CAM2:ArrayData"

    // file saving stuff
    //"HDF:FilePath",
    //    "HDF:FileName", "HDF:FileNumber", "HDF:WriteFile", "CAM:Acquire", "CAM:Acquire", "HDF:Capture", "HDF:NumCapture",
    //    "MAGICK:FileName", "MAGICK:FilePath", "MAGICK:FileNumber", "MAGICK:Capture", "MAGICK:WriteFile",
    //    "MAGICK:NumCapture", 

    // We need to a STATIC messenger so that the static call back functions can use it to print messages 
    static LoggingSystem messenger;
};

#endif //EPICS_CAMERA_INTERFACE_H_