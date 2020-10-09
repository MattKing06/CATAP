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
    static void update_ANA_UseNPoint(const struct event_handler_args args);

    static void update_HDFB_Buffer_FilePath(const struct event_handler_args args);




   //     void cameraBase::updateCamValue(const CAM_PV_TYPE pv, const std::string& objName,
   //         const event_handler_args& args)
   // {

   //     using namespace cameraStructs;
   //     cameraObject& camObj = allCamData.at(objName);
   //     switch (pv)
   //     {
   //         /*
   //                     CLARA CAMER STATES, ON/OFF, ACQUIRING, ANALYSING
   //         */
   //     case CAM_PV_TYPE::CAM_WRITE_FILE_RBV:
   //         updateWriteState(args, camObj.daq.writeState);
   //         message(camObj.name, " (", camObj.screenName, "): Write Status is ", ENUM_TO_STRING(camObj.daq.writeState));
   //         break;
   //     case CAM_PV_TYPE::CAM_FILE_WRITE_STATUS:
   //         updateWriteCheck(args, camObj.daq.writeCheck);
   //         message(camObj.name, " (", camObj.screenName, "): Write Status is ", ENUM_TO_STRING(camObj.daq.writeCheck));
   //         break;
   //     case CAM_PV_TYPE::CAM_FILE_WRITE_ERROR_MESSAGE_RBV:
   //         updateWriteErrorMessage(args, camObj.daq.writeErrorMessage);
   //         message(camObj.name, " (", camObj.screenName, "): Write Error message is: ", camObj.daq.writeErrorMessage);
   //         break;
   //     case CAM_PV_TYPE::CAM_STATUS:
   //         updateCamState(args, camObj.state.power);
   //         message(camObj.name, " (", camObj.screenName, "): power is ", ENUM_TO_STRING(camObj.state.power));
   //         break;
   //     case CAM_PV_TYPE::CAM_CAPTURE_RBV:
   //         updateCollectingState(args, camObj.daq.collectingState);
   //         message(camObj.name, " (", camObj.screenName, "): Collecting state is ", ENUM_TO_STRING(camObj.daq.collectingState));
   //         break;
   //     case CAM_PV_TYPE::CAM_ACQUIRE_RBV:
   //         updateAcquiring(args, camObj.state.acquire);
   //         message(camObj.name, " (", camObj.screenName, "): Aquiring state is ", ENUM_TO_STRING(camObj.state.acquire));
   //         if (isAcquiring(camObj))
   //             updateSelectedCamRef(camObj);
   //         break;
   //         /*
   //             How many shots have been captured
   //         */
   //     case CAM_PV_TYPE::CAM_NUM_CAPTURED_RBV:
   //         camObj.daq.shotsTaken = getDBRlong(args);
   //         break;
   //         /*
   //             How many shots to capture
   //         */
   //     case CAM_PV_TYPE::CAM_NUM_CAPTURE_RBV:
   //         camObj.daq.numberOfShots = getDBRlong(args);
   //         break;


   //         //        case CAM_PV_TYPE::CAM_BKGRND_DATA:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_FILE_NAME:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_FILE_PATH:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_CAPTURE:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_FILE_WRITE:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_NUM_CAPTURED:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_FILE_WRITE_RBV:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //
   //         //        case CAM_PV_TYPE::JPG_FILE_WRITE_CHECK:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_FILE_WRITE_MESSAGE:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_CAPTURE_RBV:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_NUM_CAPTURE:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;
   //         //        case CAM_PV_TYPE::JPG_NUM_CAPTURE_RBV:
   //         //            //pilaser.HWP = getDBRdouble(args);
   //         //            break;

   //     case CAM_PV_TYPE::BLACKLEVEL_RBV:
   //         camObj.state.Blacklevel = (int)getDBRlong(args);
   //         break;

   //     case CAM_PV_TYPE::GAINRAW_RBV:
   //         camObj.state.gain = (int)getDBRlong(args);
   //         break;

   //     case CAM_PV_TYPE::STEP_SIZE_RBV:
   //         camObj.data.analysis.step_size = getDBRint(args);
   //         break;

   //     case CAM_PV_TYPE::START_IA_RBV:
   //         camObj.state.analysing = (bool)getDBRunsignedShort(args);
   //         message(camObj.name, " analysing_data = ", camObj.state.analysing);
   //         //pilaser.HWP = getDBRdouble(args);
   //         break;

   //     case CAM_PV_TYPE::USE_BKGRND_RBV:
   //         camObj.state.use_background = (bool)getDBRunsignedShort(args);
   //         message(camObj.name, " use_background = ", camObj.state.use_background);
   //         break;

   //     case CAM_PV_TYPE::USE_NPOINT_RBV:
   //         camObj.state.use_npoint = (bool)getDBRunsignedShort(args);
   //         message(camObj.name, " use_npoint = ", camObj.state.use_npoint);
   //         break;


   //     case CAM_PV_TYPE::BIT_DEPTH:
   //         /*
   //             DONT KNOW PV YET !!!!!!
   //         */
   //         break;
   //         /*
   //                     CLARA CAMERAS SET-UP READ BACK VALUES
   //         */
   //     case CAM_PV_TYPE::CAM_EXPOSURE_TIME_RBV:
   //         camObj.daq.exposureTime = getDBRdouble(args);
   //         break;

   //     case CAM_PV_TYPE::CAM_ACQUIRE_PERIOD_RBV:
   //         camObj.daq.acquisitionPeriod = getDBRdouble(args);
   //         break;
   //     case CAM_PV_TYPE::CAM_FREQ_RBV:
   //         camObj.daq.frequency = getDBRdouble(args);
   //         break;
   //     case CAM_PV_TYPE::CAM_SENSOR_TEMP_RBV:
   //         updateSensorTemp(args, camObj);
   //         break;
   //         /*
   //                     IMAGE ANALYSIS RESULTS
   //         */
   //     case CAM_PV_TYPE::X_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.x,
   //             camObj.data.analysis.x_buf,
   //             camObj.data.analysis.x_rs,
   //             camObj.data.analysis);
   //         break;
   //     case CAM_PV_TYPE::Y_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.y,
   //             camObj.data.analysis.y_buf,
   //             camObj.data.analysis.y_rs,
   //             camObj.data.analysis);
   //         break;
   //     case CAM_PV_TYPE::SIGMA_X_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.sig_x,
   //             camObj.data.analysis.sig_x_buf,
   //             camObj.data.analysis.sig_x_rs,
   //             camObj.data.analysis);
   //         break;
   //     case CAM_PV_TYPE::SIGMA_Y_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.sig_y,
   //             camObj.data.analysis.sig_y_buf,
   //             camObj.data.analysis.sig_y_rs,
   //             camObj.data.analysis);
   //         break;
   //     case CAM_PV_TYPE::COV_XY_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.sig_xy,
   //             camObj.data.analysis.sig_xy_buf,
   //             camObj.data.analysis.sig_xy_rs,
   //             camObj.data.analysis);
   //         break;
   //     case CAM_PV_TYPE::X_PIX_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.x_pix,
   //             camObj.data.analysis.x_pix_buf,
   //             camObj.data.analysis.x_pix_rs,
   //             camObj.data.analysis);
   //         camObj.data.analysis.x_private_rs.Push(camObj.data.analysis.x_pix);
   //         /*
   //             sets flags if x-positino is tool close to the edge of the image
   //         */

   //         //message("new x_pix = ",camObj.data.analysis.x_pix);
   //         if (camObj.data.analysis.x_pix > camObj.data.analysis.pix_val_x_hi)
   //         {
   //             camObj.state.is_pix_val_x_hi = true;
   //             camObj.state.is_pix_val_x_lo = false;
   //         }
   //         else if (camObj.data.analysis.x_pix < camObj.data.analysis.pix_val_x_lo)
   //         {
   //             camObj.state.is_pix_val_x_hi = false;
   //             camObj.state.is_pix_val_x_lo = true;
   //         }
   //         else
   //         {
   //             camObj.state.is_pix_val_x_hi = false;
   //             camObj.state.is_pix_val_x_lo = false;
   //         }

   //         break;
   //     case CAM_PV_TYPE::Y_PIX_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.y_pix,
   //             camObj.data.analysis.y_pix_buf,
   //             camObj.data.analysis.y_pix_rs,
   //             camObj.data.analysis);
   //         camObj.data.analysis.y_private_rs.Push(camObj.data.analysis.y_pix);
   //         /*
   //             sets flags if x-positino is tool close to the edge of the image
   //         */
   //         //message("Y_PIX_RBV, ", camObj.data.analysis.y_pix, ", ", camObj.data.analysis.pix_val_y_hi);
   //         if (camObj.data.analysis.y_pix > camObj.data.analysis.pix_val_y_hi)
   //         {
   //             camObj.state.is_pix_val_y_hi = true;
   //             camObj.state.is_pix_val_y_lo = false;
   //         }
   //         else if (camObj.data.analysis.y_pix < camObj.data.analysis.pix_val_y_lo)
   //         {
   //             camObj.state.is_pix_val_y_hi = false;
   //             camObj.state.is_pix_val_y_lo = true;
   //         }
   //         else
   //         {
   //             camObj.state.is_pix_val_y_hi = false;
   //             camObj.state.is_pix_val_y_lo = false;
   //         }
   //     case CAM_PV_TYPE::SIGMA_X_PIX_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.sig_x_pix,
   //             camObj.data.analysis.sig_x_pix_buf,
   //             camObj.data.analysis.sig_x_pix_rs,
   //             camObj.data.analysis);
   //         break;
   //     case CAM_PV_TYPE::SIGMA_Y_PIX_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.sig_y_pix,
   //             camObj.data.analysis.sig_y_pix_buf,
   //             camObj.data.analysis.sig_y_pix_rs,
   //             camObj.data.analysis);
   //         break;
   //     case CAM_PV_TYPE::COV_XY_PIX_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.sig_xy_pix,
   //             camObj.data.analysis.sig_xy_pix_buf,
   //             camObj.data.analysis.sig_xy_pix_rs,
   //             camObj.data.analysis);
   //     case CAM_PV_TYPE::AVG_PIX_INTENSITY_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.avg_pix,
   //             camObj.data.analysis.avg_pix_buf,
   //             camObj.data.analysis.avg_pix_rs,
   //             camObj.data.analysis);
   //         /*
   //             update flag to 'guess' if beam is on screen
   //         */
   //         camObj.state.latest_avg_pix_has_beam = camObj.data.analysis.avg_pix > camObj.data.analysis.avg_pix_beam_level;
   //case CAM_PV_TYPE::SUM_PIX_INTENSITY_RBV:
   //         updateAnalysislResult(args, camObj.data.analysis.sum_pix,
   //             camObj.data.analysis.sum_pix_buf,
   //             camObj.data.analysis.sum_pix_rs,
   //             camObj.data.analysis);
   //     case CAM_PV_TYPE::PIXEL_RESULTS_RBV:
   //         updatePixelResults(args, camObj);
   //         maskFeedBack(camObj);
   //     case CAM_PV_TYPE::MASK_X_RBV:
   //         camObj.data.mask.mask_x = getDBRunsignedShort(args);
   //     case CAM_PV_TYPE::MASK_Y_RBV:
   //         camObj.data.mask.mask_y = getDBRunsignedShort(args);
   //     case CAM_PV_TYPE::MASK_X_RAD_RBV:
   //         camObj.data.mask.mask_x_rad = getDBRunsignedShort(args);
   //         //message(camObj.name," mask_x_rad = ",camObj.data.mask.mask_x_rad, " ", getDBRunsignedShort(args) );
   //     case CAM_PV_TYPE::MASK_Y_RAD_RBV:
   //         camObj.data.mask.mask_y_rad = getDBRunsignedShort(args);
   //     case CAM_PV_TYPE::X_CENTER_RBV:
   //         camObj.data.analysis.x_centre = (size_t)getDBRunsignedLong(args);
   //     case CAM_PV_TYPE::Y_CENTER_RBV:
   //         camObj.data.analysis.y_centre = (size_t)getDBRunsignedLong(args);
   //     case CAM_PV_TYPE::PIX_MM_RBV:
   //         camObj.data.analysis.pix_2_mm = getDBRdouble(args);
   //     case CAM_PV_TYPE::ARRAY_DATA:
   //         updateArrayData(camObj, args);
   //     case CAM_PV_TYPE::LED_STA:
   //         updateCLaraLEDState(args);
   //     case CAM_PV_TYPE::VELA_LED_STA:
   //         updateVelaLEDState(args);
   //     }
   // }





};

#endif //EPICS_CAMERA_INTERFACE_H_