#include "EPICSCameraInterface.h"
#include "CameraPVRecords.h"
// REMOVE THIS LATER.
#include <iostream>
#include <mutex>          // std::mutex
std::mutex cam_interface_mtx;           // mutex for critical section


LoggingSystem EPICSCameraInterface::messenger;

EPICSCameraInterface::EPICSCameraInterface() :
	EPICSInterface()
{
	messenger = LoggingSystem(true, true);
	messenger.printDebugMessage("EPICSCameraInterface Constructor Called");
}

EPICSCameraInterface::EPICSCameraInterface(const EPICSCameraInterface& copyInterface)
{
	messenger.printDebugMessage("EPICSCameraInterface Copy Constructor Called");
}
EPICSCameraInterface::~EPICSCameraInterface()
{
	messenger.printDebugMessage("EPICSCameraInterface Destructor Called");
}
void EPICSCameraInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	using namespace CameraRecords;
	// TODOD could be a const map and then a lookup ... might be neater?? 
	if (pvStruct.pvRecord == HDF_WriteFile_RBV)	{		pvStruct.updateFunction = this->update_HDF_WriteFile_RBV;	}
	else if (pvStruct.pvRecord == HDF_WriteStatus)		{		pvStruct.updateFunction = this->update_HDF_WriteStatus;	}
	else if (pvStruct.pvRecord == HDF_WriteMessage)		{		pvStruct.updateFunction = this->update_HDF_WriteMessage_RBV;	}
	else if (pvStruct.pvRecord == HDF_FileName_RBV)		{		pvStruct.updateFunction = this->update_HDF_FileName_RBV;	}
	else if (pvStruct.pvRecord == HDF_FilePath_RBV)		{		pvStruct.updateFunction = this->update_HDF_FilePath_RBV;	}
	else if (pvStruct.pvRecord == HDF_FileNumber_RBV)	{		pvStruct.updateFunction = this->update_HDF_FileNumber_RBV;	}
//	else if (pvStruct.pvRecord == HDF_NumCapture_RBV)	{		pvStruct.updateFunction = this->update_HDF_NumCapture_RBV;	}
	else if (pvStruct.pvRecord == HDF_Capture_RBV)		{		pvStruct.updateFunction = this->update_HDF_Capture_RBV;	}
	else if (pvStruct.pvRecord == CAM_Acquire_RBV)		{		pvStruct.updateFunction = this->update_CAM_Acquire_RBV;	}
	else if (pvStruct.pvRecord == HDF_NumCapture_RBV)	{		pvStruct.updateFunction = this->update_HDF_NumCapture_RBV;	}
	else if (pvStruct.pvRecord == ANA_NPointStepSize_RBV)	{		pvStruct.updateFunction = this->update_ANA_NPointStepSize_RBV;	}
	else if (pvStruct.pvRecord == ANA_EnableCallbacks_RBV)	{		pvStruct.updateFunction = this->update_ANA_EnableCallbacks_RBV;	}
	else if (pvStruct.pvRecord == ANA_X_RBV)			{		pvStruct.updateFunction = this->update_ANA_X_RBV;	}
	else if (pvStruct.pvRecord == ANA_Y_RBV)			{		pvStruct.updateFunction = this->update_ANA_Y_RBV;	}
	else if (pvStruct.pvRecord == ANA_SigmaX_RBV)		{		pvStruct.updateFunction = this->update_ANA_SigmaX_RBV;	}
	else if (pvStruct.pvRecord == ANA_SigmaY_RBV)		{		pvStruct.updateFunction = this->update_ANA_SigmaY_RBV;	}
	else if (pvStruct.pvRecord == ANA_CovXY_RBV)		{		pvStruct.updateFunction = this->update_ANA_CovXY_RBV;	}
	else if (pvStruct.pvRecord == ANA_AvgIntensity_RBV)	{		pvStruct.updateFunction = this->update_ANA_AvgIntensity_RBV;	}
	else if (pvStruct.pvRecord == ANA_Intensity_RBV)	{		pvStruct.updateFunction = this->update_ANA_Intensity_RBV;	}
	else if (pvStruct.pvRecord == ANA_XPix_RBV)			{		pvStruct.updateFunction = this->update_ANA_XPix_RBV;	}
	else if (pvStruct.pvRecord == ANA_YPix_RBV)			{		pvStruct.updateFunction = this->update_ANA_YPix_RBV;	}
	else if (pvStruct.pvRecord == ANA_SigmaXPix_RBV)	{		pvStruct.updateFunction = this->update_ANA_SigmaXPix_RBV;	}
	else if (pvStruct.pvRecord == ANA_SigmaYPix_RBV)	{		pvStruct.updateFunction = this->update_ANA_SigmaYPix_RBV;	}
	else if (pvStruct.pvRecord == ANA_CovXYPix_RBV)		{		pvStruct.updateFunction = this->update_ANA_CovXYPix_RBV;	}
	else if (pvStruct.pvRecord == ANA_PixelResults_RBV)	{		pvStruct.updateFunction = this->update_ANA_PixelResults_RBV;	}
	else if (pvStruct.pvRecord == ANA_MMResults_RBV)	{		pvStruct.updateFunction = this->update_ANA_MMResults_RBV;	}
	else if (pvStruct.pvRecord == ANA_MaskXCenter_RBV)	{		pvStruct.updateFunction = this->update_ANA_MaskXCenter_RBV;	}
	else if (pvStruct.pvRecord == ANA_MaskYCenter_RBV)	{		pvStruct.updateFunction = this->update_ANA_MaskYCenter_RBV;	}
	else if (pvStruct.pvRecord == ANA_MaskXRad_RBV)		{		pvStruct.updateFunction = this->update_ANA_MaskXRad_RBV;	}
	else if (pvStruct.pvRecord == ANA_MaskYRad_RBV)		{		pvStruct.updateFunction = this->update_ANA_MaskYRad_RBV;	}
	else if (pvStruct.pvRecord == ANA_CenterX_RBV)		{		pvStruct.updateFunction = this->update_ANA_CenterX_RBV;	}	
	else if (pvStruct.pvRecord == ANA_CenterY_RBV)		{		pvStruct.updateFunction = this->update_ANA_CenterY_RBV;	}
	else if (pvStruct.pvRecord == ANA_PixMM_RBV)		{		pvStruct.updateFunction = this->update_ANA_PixMM_RBV;	}
	else if (pvStruct.pvRecord == CAM_AcquireTime_RBV)	{		pvStruct.updateFunction = this->update_CAM_AcquireTime_RBV;	}
	else if (pvStruct.pvRecord == CAM_AcquirePeriod_RBV){		pvStruct.updateFunction = this->update_CAM_AcquirePeriod_RBV;	}
	else if (pvStruct.pvRecord == CAM_ArrayRate_RBV)	{		pvStruct.updateFunction = this->update_CAM_ArrayRate_RBV;	}
	else if (pvStruct.pvRecord == CAM_Temperature_RBV)	{		pvStruct.updateFunction = this->update_CAM_Temperature_RBV;	}	
	else if (pvStruct.pvRecord == HDFB_Buffer_FilePath_RBV)	{		pvStruct.updateFunction = this->update_HDFB_Buffer_FilePath_RBV;	}
	else if (pvStruct.pvRecord == HDFB_Buffer_FileName_RBV)	{		pvStruct.updateFunction = this->update_HDFB_Buffer_FileName_RBV;	}
	else if (pvStruct.pvRecord == HDFB_Buffer_FileNumber_RBV)	{		pvStruct.updateFunction = this->update_HDFB_Buffer_FileNumber_RBV;	}
	else if (pvStruct.pvRecord == ROI1_MinX_RBV)		{		pvStruct.updateFunction = this->update_ROI1_MinX_RBV;	}
	else if (pvStruct.pvRecord == ROI1_MinY_RBV)		{		pvStruct.updateFunction = this->update_ROI1_MinY_RBV;	}
	else if (pvStruct.pvRecord == ROI1_SizeX_RBV)		{		pvStruct.updateFunction = this->update_ROI1_SizeX_RBV;	}
	else if (pvStruct.pvRecord == ROI1_SizeY_RBV)		{		pvStruct.updateFunction = this->update_ROI1_SizeY_RBV;	}
	else if (pvStruct.pvRecord == ROI1_ImageData_RBV)	{		pvStruct.updateFunction = this->update_ROI1_ImageData_RBV;	}
	else if (pvStruct.pvRecord == ANA_UseFloor_RBV)		{		pvStruct.updateFunction = this->update_ANA_UseFloor_RBV;	}
	else if (pvStruct.pvRecord == ANA_FloorLevel_RBV)	{		pvStruct.updateFunction = this->update_ANA_FloorLevel_RBV;	}
	else if (pvStruct.pvRecord == ANA_FlooredPoints_RBV){		pvStruct.updateFunction = this->update_ANA_FlooredPoints_RBV;	}
	else if (pvStruct.pvRecord == ANA_FlooredPercent_RBV)	{		pvStruct.updateFunction = this->update_ANA_FlooredPercent_RBV;	}
	else if (pvStruct.pvRecord == ANA_CPU_RBV)			{		pvStruct.updateFunction = this->update_ANA_CPU_RBV;	}
	else if (pvStruct.pvRecord == ANA_CPU_CropSubMask_RBV)	{		pvStruct.updateFunction = this->update_ANA_CPU_CropSubMask_RBV;	}
	else if (pvStruct.pvRecord == ANA_CPU_Npoint_RBV)	{		pvStruct.updateFunction = this->update_ANA_CPU_Npoint_RBV;	}
	else if (pvStruct.pvRecord == ANA_CPU_Dot_RBV)		{		pvStruct.updateFunction = this->update_ANA_CPU_Dot_RBV;	}
	else if (pvStruct.pvRecord == ANA_PixW_RBV)		{		pvStruct.updateFunction = this->update_ANA_PixW_RBV;}
	else if (pvStruct.pvRecord == ANA_PixH_RBV)		{		pvStruct.updateFunction = this->update_ANA_PixH_RBV;	}
	else if (pvStruct.pvRecord == ANA_UseBkgrnd)	{	pvStruct.updateFunction = this->update_ANA_UseBkgrnd_RBV;	}
	else if (pvStruct.pvRecord == ANA_NPointStepSize_RBV)	{		pvStruct.updateFunction = this->update_ANA_NPointStepSize_RBV;	}
	else if (pvStruct.pvRecord == ANA_UseNPoint_RBV)	{		pvStruct.updateFunction = this->update_ANA_UseNPoint_RBV;	}
	else if (pvStruct.pvRecord == ANA_NewBkgrnd)	{	pvStruct.updateFunction = this->update_ANA_NewBkgrnd_RBV;	}
	else if (pvStruct.pvRecord == CAM_BlackLevel_RBV)	{		pvStruct.updateFunction = this->update_CAM_BlackLevel_RBV;	}
	else if (pvStruct.pvRecord == CAM_Gain_RBV)		{		pvStruct.updateFunction = this->update_CAM_Gain_RBV;	}
	else if (pvStruct.pvRecord == ANA_OVERLAY_1_CROSS_RBV)	{		pvStruct.updateFunction = this->update_ANA_OVERLAY_1_CROSS_RBV;	}
	else if (pvStruct.pvRecord == ANA_OVERLAY_2_RESULT_RBV)	{		pvStruct.updateFunction = this->update_ANA_OVERLAY_2_RESULT_RBV;	}
	else if (pvStruct.pvRecord == ANA_OVERLAY_3_MASK_RBV)	{		pvStruct.updateFunction = this->update_ANA_OVERLAY_3_MASK_RBV;	}
	else	
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSCameraInterface::update_ANA_OVERLAY_1_CROSS_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->cross_overlay.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->cross_overlay.second = STATE::DISABLED; break;
	case GlobalConstants::one_ushort:  recastCamera->cross_overlay.second = STATE::ENABLED; break;
	default:
		recastCamera->cross_overlay.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_OVERLAY_1_CROSS_RBV = ",
		ENUM_TO_STRING(recastCamera->cross_overlay.second));
}
void EPICSCameraInterface::update_ANA_OVERLAY_2_RESULT_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->result_overlay.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->result_overlay.second = STATE::DISABLED; break;
	case GlobalConstants::one_ushort:  recastCamera->result_overlay.second = STATE::ENABLED; break;
	default:
		recastCamera->result_overlay.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_OVERLAY_2_CROSS_RBV = ",
		ENUM_TO_STRING(recastCamera->result_overlay.second));
}
void EPICSCameraInterface::update_ANA_OVERLAY_3_MASK_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->mask_overlay.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->mask_overlay.second = STATE::DISABLED; break;
	case GlobalConstants::one_ushort:  recastCamera->mask_overlay.second = STATE::ENABLED; break;
	default:
		recastCamera->mask_overlay.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_OVERLAY_3_CROSS_RBV = ",
		ENUM_TO_STRING(recastCamera->mask_overlay.second));
}
void EPICSCameraInterface::update_HDF_WriteFile_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->write_state_check.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->write_state_check.second = STATE::WRITE_CHECK_OK; break;
	case GlobalConstants::one_ushort:  recastCamera->write_state_check.second = STATE::WRITE_CHECK_ERROR; break;
	default:
		recastCamera->write_state_check.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteFile_RBV = ",
		ENUM_TO_STRING(recastCamera->write_state_check.second));
}
void EPICSCameraInterface::update_HDF_WriteStatus(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->write_state.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->write_state.second = STATE::WRITE_CHECK_OK; break;
	case GlobalConstants::one_ushort:  recastCamera->write_state.second = STATE::WRITE_CHECK_ERROR; break;
	default:
		recastCamera->write_state.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteStatus = ",
		ENUM_TO_STRING(recastCamera->write_state.second));
}
void EPICSCameraInterface::update_HDF_WriteMessage_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const dbr_time_char* new_data = (const struct dbr_time_char*)args.dbr;
	recastCamera->write_error_message.first = new_data->stamp;
	const dbr_char_t* value;
	value = &new_data->value;
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	// char array must have a const size (defined at compile time (or we use an array on the heap with new and delete)
	char dummy_char[256]; // MAGIC NUMBER
	for (int i = 0; i != 256; ++i)
	{
		dummy_char[i] = value[i];
		//std::cout << (int)value[i] << std::endl;
	}
	std::string dummy_string(dummy_char);
	recastCamera->write_error_message.second = dummy_string;
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteMessage_RBV (write_error_message) = ",
		recastCamera->write_error_message.second);
}
void EPICSCameraInterface::update_HDF_NumCapture_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->capture_count);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_NumCapture_RBV = ",
		recastCamera->capture_count.second);
}
void EPICSCameraInterface::update_HDF_Capture_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->capture_state.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->capture_state.second = STATE::NOT_CAPTURING; break;
	case GlobalConstants::one_ushort:  recastCamera->capture_state.second = STATE::CAPTURING; break;
	default:
		recastCamera->capture_state.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_Capture_RBV = ",
		ENUM_TO_STRING(recastCamera->capture_state.second));

}
void EPICSCameraInterface::update_CAM_Acquire_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->acquire_state.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->acquire_state.second = STATE::NOT_ACQUIRING; break;
	case GlobalConstants::one_ushort:  recastCamera->acquire_state.second = STATE::ACQUIRING; break;
	default:
		recastCamera->acquire_state.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_CAM_Acquire_RBV = ",
		ENUM_TO_STRING(recastCamera->acquire_state.second));
}
void EPICSCameraInterface::update_LED_Sta(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->led_state.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->led_state.second = STATE::OFF; break;
	case GlobalConstants::one_ushort:  recastCamera->led_state.second = STATE::ON; break;
	default:
		recastCamera->acquire_state.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_LED_Sta = ",
		recastCamera->led_state.second);

}
void EPICSCameraInterface::update_ANA_PixMM_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->pixel_to_mm);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_PixMM_RBV = ",
		recastCamera->pixel_to_mm.second);
}
void EPICSCameraInterface::update_CAM_BlackLevel_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->black_level);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_BlackLevel_RBV = ",
		recastCamera->black_level.second);
}
void EPICSCameraInterface::update_CAM_Gain_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->gain);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_Gain_RBV = ",
		recastCamera->gain.second);
}
void EPICSCameraInterface::update_ANA_NPointStepSize_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->step_size);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_NPointStepSize_RBV = ",
		recastCamera->step_size.second);
}
void EPICSCameraInterface::update_ANA_EnableCallbacks_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->analysis_state.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->analysis_state.second = STATE::NOT_ANALYSING; break;
	case GlobalConstants::one_ushort:  recastCamera->analysis_state.second = STATE::ANALYSING; break;
	default:
		recastCamera->analysis_state.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_EnableCallbacks_RBV = ",
		ENUM_TO_STRING(recastCamera->analysis_state.second));
}
void EPICSCameraInterface::update_ANA_X_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->x_mm);
	//recastCamera->x_mm_rs.Push(recastCamera->x_mm.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_X_RBV = ", 
	//	recastCamera->x_mm.second);
}
void EPICSCameraInterface::update_ANA_Y_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->y_mm);
	//recastCamera->y_mm_rs.Push(recastCamera->y_mm.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_Y_RBV = ",
	//	recastCamera->y_mm.second);
}
void EPICSCameraInterface::update_ANA_SigmaX_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_x_mm);
	//recastCamera->sigma_x_mm_rs.Push(recastCamera->sigma_x_mm.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_SigmaX_RBV = ", 
	//	recastCamera->sigma_x_mm.second);
}
void EPICSCameraInterface::update_ANA_SigmaY_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_y_mm);
	//recastCamera->sigma_y_mm_rs.Push(recastCamera->sigma_y_mm.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_SigmaY_RBV = ", 
	//	recastCamera->sigma_y_mm.second);
}
void EPICSCameraInterface::update_ANA_CovXY_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_xy_mm);
	//recastCamera->sigma_xy_mm_rs.Push(recastCamera->sigma_xy_mm.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CovXY_RBV = ", 
	//	recastCamera->sigma_xy_mm.second);
}
void EPICSCameraInterface::update_ANA_AvgIntensity_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->avg_intensity);
	recastCamera->avg_intensity_rs.Push<double>(recastCamera->avg_intensity.second);
	//recastCamera->avg_intensity_rs.Push(recastCamera->avg_intensity.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_AvgIntensity_RBV = ", 
	//	recastCamera->avg_intensity.second);
}
void EPICSCameraInterface::update_ANA_Intensity_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sum_intensity);
	recastCamera->sum_intensity_rs.Push<double>(recastCamera->sum_intensity.second);
	//recastCamera->sum_intensity_rs.Push(recastCamera->sum_intensity.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_Intensity_RBV = ", 
	//	recastCamera->sum_intensity.second);
}
void EPICSCameraInterface::update_ANA_XPix_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->x_pix);
	//recastCamera->x_pix_rs.Push(recastCamera->x_pix.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_XPix_RBV = ", 
	//	recastCamera->x_pix.second);
}
void EPICSCameraInterface::update_ANA_YPix_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->y_pix);
	//recastCamera->y_pix_rs.Push(recastCamera->y_pix.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_YPix_RBV = ",
	//	recastCamera->y_pix.second);
}
void EPICSCameraInterface::update_ANA_SigmaXPix_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_x_pix);
	//recastCamera->sigma_x_pix_rs.Push(recastCamera->sigma_x_pix.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_SigmaXPix_RBV = ", 
	//	recastCamera->sigma_x_pix.second);
}
void EPICSCameraInterface::update_ANA_SigmaYPix_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_y_pix);
	//recastCamera->sigma_y_pix_rs.Push(recastCamera->sigma_y_pix.second);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_SigmaYPix_RBV = ",
	//	recastCamera->sigma_y_pix.second);
}
void EPICSCameraInterface::update_ANA_CovXYPix_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_xy_pix);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CovXYPix_RBV = ", 
	//	recastCamera->sigma_xy_pix.second);
}
void EPICSCameraInterface::update_ANA_PixelResults_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	recastCamera->lastResultsUpdateTime = recastCamera->pixelResults.first;
	updateTimeStampDoubleVectorPair(args, recastCamera->pixelResults, recastCamera->pixelResults.second.size());
	if (recastCamera->areNotEqualEpicsTimeStamps(recastCamera->lastResultsUpdateTime, recastCamera->pixelResults.first))
	{
		recastCamera->isResultUpdated = true;

		recastCamera->x_pix_rs.Push<double>(recastCamera->pixelResults.second[0]);
		recastCamera->y_pix_rs.Push<double>(recastCamera->pixelResults.second[1]);
		recastCamera->sigma_x_pix_rs.Push<double>(recastCamera->pixelResults.second[2]);
		recastCamera->sigma_y_pix_rs.Push<double>(recastCamera->pixelResults.second[3]);
		recastCamera->sigma_xy_pix_rs.Push<double>(recastCamera->pixelResults.second[4]);
		messenger.printDebugMessage("update_ANA_PixelResults_RBV = ", recastCamera->pixelResults.second[0], " ",
			recastCamera->pixelResults.second[1], " ",	recastCamera->pixelResults.second[2], " ",
			recastCamera->pixelResults.second[3], " ",	recastCamera->pixelResults.second[4]);
		//messenger.printDebugMessage("RS = ", recastCamera->x_pix_rs.Mean(), " ",
		//	recastCamera->y_pix_rs.Mean(), " ",		//	recastCamera->sigma_x_pix_rs.Mean(), " ",
		//	recastCamera->sigma_y_pix_rs.Mean(), " ",		//	recastCamera->sigma_xy_pix_rs.Mean());
	}
	else
	{
		recastCamera->isResultUpdated = false;
	}
}
void EPICSCameraInterface::update_ANA_MMResults_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	recastCamera->lastResultsUpdateTime_mm_ana_results = recastCamera->mmResults.first;
	updateTimeStampDoubleVectorPair(args, recastCamera->mmResults, recastCamera->mmResults.second.size());
	if (recastCamera->areNotEqualEpicsTimeStamps(recastCamera->lastResultsUpdateTime, recastCamera->mmResults.first))
	{
		recastCamera->x_mm_rs.Push<double>(recastCamera->mmResults.second[0]);
		recastCamera->y_mm_rs.Push<double>(recastCamera->mmResults.second[1]);
		recastCamera->sigma_x_mm_rs.Push<double>(recastCamera->mmResults.second[2]);
		recastCamera->sigma_y_mm_rs.Push<double>(recastCamera->mmResults.second[3]);
		recastCamera->sigma_xy_mm_rs.Push<double>(recastCamera->mmResults.second[4]);
		//messenger.printDebugMessage("update_ANA_MMResults_RBV = ", recastCamera->mmResults.second[0], " ",
		//	recastCamera->mmResults.second[1], " ",
		//	recastCamera->mmResults.second[2], " ",
		//	recastCamera->mmResults.second[3], " ",
		//	recastCamera->mmResults.second[4]);
		//messenger.printDebugMessage("RS = ", recastCamera->x_mm_rs.Mean(), " ",
		//	recastCamera->y_mm_rs.Mean(), " ",
		//	recastCamera->sigma_x_mm_rs.Mean(), " ",
		//	recastCamera->sigma_y_mm_rs.Mean(), " ",
		//	recastCamera->sigma_xy_mm_rs.Mean());

	}
}
void EPICSCameraInterface::update_ANA_MaskXCenter_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_x_center);
	messenger.printDebugMessage(recastCamera->hardwareName, "update_ANA_MaskXCenter_RBV = ",
		recastCamera->mask_x_center.second);
}
void EPICSCameraInterface::update_ANA_MaskYCenter_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_y_center);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_MaskYCenter_RBV = ",
		recastCamera->mask_y_center.second);
}
void EPICSCameraInterface::update_ANA_MaskXRad_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_x_radius);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_MaskXRad_RBV = ",
		recastCamera->mask_x_radius.second);
}
void EPICSCameraInterface::update_ANA_MaskYRad_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_y_radius);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_MaskYRad_RBV = ",
		recastCamera->mask_y_radius.second);
}
void EPICSCameraInterface::update_ANA_CenterX_RBV(const struct event_handler_args args) 
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->x_center_pixel);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CenterX_RBV = ",
		recastCamera->x_center_pixel.second);
}
void EPICSCameraInterface::update_ANA_CenterY_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->y_center_pixel);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CenterY_RBV = ",
		recastCamera->y_center_pixel.second);
}
void EPICSCameraInterface::update_CAM_AcquireTime_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->acquire_time);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_CAM_AcquireTime_RBV = ",
		recastCamera->acquire_time.second);
}
void EPICSCameraInterface::update_CAM_AcquirePeriod_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->acquire_period);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_CAM_AcquirePeriod_RBV = ",
		recastCamera->acquire_period.second);
}
void EPICSCameraInterface::update_CAM_ArrayRate_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->array_rate);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_CAM_ArrayRate_RBV = ",
	//	recastCamera->array_rate.second);
}
void EPICSCameraInterface::update_CAM_Temperature_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->temperature);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_CAM_Temperature_RBV = ",
		recastCamera->temperature.second);
}
void EPICSCameraInterface::update_ANA_UseNPoint_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastCamera->use_npoint.first = tv->stamp;
	switch (tv->value)
	{
	case 0: recastCamera->use_npoint.second = STATE::NOT_USING_NPOINT; break;
	case 1:  recastCamera->use_npoint.second = STATE::USING_NPOINT; break;
	default:
		recastCamera->use_npoint.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseNPoint_RBV = ",
		ENUM_TO_STRING(recastCamera->use_npoint.second));
}
void EPICSCameraInterface::update_ANA_NewBkgrnd_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	//messenger.printDebugMessage("update_ANA_UseNPoint");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastCamera->set_new_background.first = tv->stamp;
	switch (tv->value)
	{
	case 0: recastCamera->set_new_background.second = STATE::YES; break;
	case 1: recastCamera->set_new_background.second = STATE::NO; break;
	default:
		recastCamera->set_new_background.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseBkgrnd = ",
		ENUM_TO_STRING(recastCamera->use_background.second));
}
void EPICSCameraInterface::update_ANA_UseBkgrnd_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	//messenger.printDebugMessage("update_ANA_UseNPoint");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastCamera->use_background.first = tv->stamp;
	switch (tv->value)
	{
	case 0: recastCamera->use_background.second = STATE::NOT_USING_BACKGROUND; break;
	case 1:  recastCamera->use_background.second = STATE::USING_BACKGROUND; break;
	default:
		recastCamera->use_background.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseBkgrnd = ",
		ENUM_TO_STRING(recastCamera->use_background.second));
}
void EPICSCameraInterface::update_HDF_FileNumber_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_FileNumber_RBV = ",
		recastCamera->save_filenumber.second);

}
void EPICSCameraInterface::update_HDF_FilePath_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_FilePath_RBV");
	const dbr_time_char* new_data = (const struct dbr_time_char*)args.dbr;
	recastCamera->save_filepath.first = new_data->stamp;
	const dbr_char_t* value;
	value = &new_data->value;
	//std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	// char array must have a const size (defined at compile time (or we use an array on the heap with new and delete)
	char dummy_char[256]; // MAGIC NUMBER
	for (int i = 0; i != 256; ++i)
	{
		dummy_char[i] = (char)value[i];
		//std::cout << (int)value[i] << std::endl;
	}
	std::string dummy_string(dummy_char);
	recastCamera->save_filepath.second = dummy_string;
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_FilePath_RBV = ",
		recastCamera->save_filepath.second);
}
void EPICSCameraInterface::update_HDF_FileName_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const dbr_time_char* new_data = (const struct dbr_time_char*)args.dbr;
	recastCamera->save_filename.first = new_data->stamp;
	const dbr_char_t* value;
	value = &new_data->value;
	// std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	// char array must have a const size (defined at compile time (or we use an array on the heap with new and delete)
	char dummy_char[256]; // MAGIC NUMBER
	for (int i = 0; i != 256; ++i)
	{
		dummy_char[i] = (char)value[i];
	}
	std::string dummy_string(dummy_char);
	recastCamera->save_filename.second = dummy_string;
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_FileName_RBV = ",
		recastCamera->save_filename.second);
}
//void EPICSCameraInterface::update_HDFB_Buffer_Trigger(const struct event_handler_args args)
//{
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	//std::pair<epicsTimeStamp, char> buffer_trigger;
//	// TODO
//
//}
void EPICSCameraInterface::update_HDFB_Buffer_FilePath_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDFB_Buffer_FilePath_RBV");
	const dbr_time_char* new_data = (const struct dbr_time_char*)args.dbr;
	recastCamera->buffer_filepath.first = new_data->stamp;
	const dbr_char_t* value;
	value = &new_data->value;
	//std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	// char array must have a const size (defined at compile time (or we use an array on the heap with new and delete)
	char dummy_char[256]; // MAGIC NUMBER
	for (int i = 0; i != 256; ++i)
	{
		dummy_char[i] = (char)value[i];
		//std::cout << (int)value[i] << std::endl;
	}
	std::string dummy_string(dummy_char);
	recastCamera->buffer_filepath.second = dummy_string;
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_HDFB_Buffer_FilePath_RBV = ",
	//	recastCamera->buffer_filepath.second);
}
void EPICSCameraInterface::update_HDFB_Buffer_FileName_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const dbr_time_char* new_data = (const struct dbr_time_char*)args.dbr;
	recastCamera->buffer_filename.first = new_data->stamp;
	const dbr_char_t* value;
	value = &new_data->value;
	// std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	// char array must have a const size (defined at compile time (or we use an array on the heap with new and delete)
	char dummy_char[256]; // MAGIC NUMBER
	for (int i = 0; i != 256; ++i)
	{
		dummy_char[i] = (char)value[i];
	}
	std::string dummy_string(dummy_char);
	recastCamera->buffer_filename.second = dummy_string;
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_FileName_RBV = ",
	//	recastCamera->buffer_filename.second);
}
void EPICSCameraInterface::update_HDFB_Buffer_FileNumber_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->buffer_filenumber);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_HDFB_Buffer_FileNumber_RBV = ",
	//	recastCamera->buffer_filenumber.second);
}
void EPICSCameraInterface::update_ROI1_MinX_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->roi_min_x);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ROI1_MinX_RBV = ",
		recastCamera->roi_min_x.second);

	recastCamera->roi_max_x = recastCamera->roi_min_x.second + recastCamera->roi_size_x.second;
	messenger.printDebugMessage(recastCamera->hardwareName, " ROI New Max X = ",
		recastCamera->roi_max_x);
}
void EPICSCameraInterface::update_ROI1_MinY_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, long> roi_min_y;
	updateTimeStampLongPair(args, recastCamera->roi_min_y);
	recastCamera->roi_max_y = recastCamera->roi_min_y.second + recastCamera->roi_size_y.second;
	// messenger.printDebugMessage(recastCamera->hardwareName, " ROI New Max Y = ",
		// recastCamera->roi_max_y);
	// messenger.printDebugMessage(recastCamera->hardwareName, " update_ROI1_MinY_RBV = ",
		// recastCamera->roi_min_y.second);

}
void EPICSCameraInterface::update_ROI1_SizeX_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->roi_size_x);
	//messenger.printDebugMessage(recastCamera->hardwareName, " roi_size_x = ",
	//	recastCamera->roi_size_x.second);
	messenger.printDebugMessage(recastCamera->hardwareName, " roi_size_x = ",
		recastCamera->roi_size_x.second);
	recastCamera->roi_max_x = recastCamera->roi_min_x.second + recastCamera->roi_size_x.second;
	messenger.printDebugMessage(recastCamera->hardwareName, " ROI New Max X = ",
		recastCamera->roi_max_x);
}
void EPICSCameraInterface::update_ROI1_SizeY_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->roi_size_y);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ROI1_SizeY_RBV = ",
	//	recastCamera->roi_size_y.second);
	recastCamera->roi_max_y = recastCamera->roi_min_y.second + recastCamera->roi_size_y.second;
	messenger.printDebugMessage(recastCamera->hardwareName, " ROI New Max Y = ",
		recastCamera->roi_max_y);
}
void EPICSCameraInterface::update_ROI1_ImageData_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	// TODO actually not doing it this way for now, only caget 
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ROI1_ImageData_RBV");
}
void EPICSCameraInterface::update_ANA_UseFloor_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastCamera->use_floor.first = tv->stamp;
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseFloor_RBV value = ", tv->value);
	switch (tv->value)
	{
	case 0: recastCamera->use_floor.second = STATE::NOT_USING_FLOOR; break;
	case 1: recastCamera->use_floor.second = STATE::USING_FLOOR; break;
	default:
		recastCamera->use_floor.second = STATE::ERR;
	}
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseFloor_RBV = ", ENUM_TO_STRING(recastCamera->use_floor.second));
}
void EPICSCameraInterface::update_ANA_FloorLevel_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->floor_level);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_FloorLevel_RBV = ",
	//	recastCamera->floor_level.second);
}
void EPICSCameraInterface::update_ANA_FlooredPoints_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->floored_pts_count);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_FlooredPoints_RBV = ",
	//	recastCamera->floored_pts_count.second);
}
void EPICSCameraInterface::update_ANA_FlooredPercent_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->floored_pts_percent);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_FlooredPercent_RBV = ",
	//	recastCamera->floored_pts_percent.second);
}
void EPICSCameraInterface::update_ANA_CPU_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->cpu_total);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CPU_RBV = ", 
	//	recastCamera->cpu_total.second);
}
void EPICSCameraInterface::update_ANA_CPU_CropSubMask_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->cpu_crop_sub_mask);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CPU_CropSubMask_RBV = ",
	//	recastCamera->cpu_crop_sub_mask.second);

}
void EPICSCameraInterface::update_ANA_CPU_Npoint_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, long> cpu_npoint;
	updateTimeStampLongPair(args, recastCamera->cpu_npoint);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CPU_Npoint_RBV = ",
	//	recastCamera->cpu_npoint.second);
}
void EPICSCameraInterface::update_ANA_CPU_Dot_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->cpu_dot);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_CPU_Dot_RBV = ",
	//	recastCamera->cpu_dot.second);
}
void EPICSCameraInterface::update_ANA_PixW_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->pixel_width);
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_PixW_RBV = ",
	//	recastCamera->pixel_width.second);
}
void EPICSCameraInterface::update_ANA_PixH_RBV(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, long> pixel_height;
	updateTimeStampLongPair(args, recastCamera->pixel_height);
	//messenger.printDebugMessage(recastCamera->hardwareName, " R = ",recastCamera->pixel_height.second);
}
//
//void EPICSCameraInterface::update_MAGICK_NumCaptured_RBV(const struct event_handler_args args)
//{
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_NumCaptured_RBV ");
//}
//void EPICSCameraInterface::update_MAGICK_WriteFile_RBV(const struct event_handler_args args)
//{
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_WriteFile_RBV ");
//}
//
//void EPICSCameraInterface::update_MAGICK_WriteStatus(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_WriteStatus ");
//}
//
//void EPICSCameraInterface::update_MAGICK_WriteMessage(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_WriteMessage ");
//}
//
//void EPICSCameraInterface::update_MAGICK_Capture_RBV(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_Capture_RBV ");
//}
//
//void EPICSCameraInterface::update_MAGICK_NumCapture_RBV(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_NumCapture_RBV ");
//}
void EPICSCameraInterface::update_OVERLAY_CROSS_HAIR(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->cross_hair_overlay.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->cross_hair_overlay.second = STATE::OFF; break;
	case GlobalConstants::one_ushort:  recastCamera->cross_hair_overlay.second = STATE::ON; break;
	default:
		recastCamera->cross_hair_overlay.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_OVERLAY_CROSS_HAIR = ",
		ENUM_TO_STRING(recastCamera->cross_hair_overlay.second));
}
void EPICSCameraInterface::update_OVERLAY_CENTRE_OF_MASS(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->center_of_mass_overlay.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->center_of_mass_overlay.second = STATE::OFF; break;
	case GlobalConstants::one_ushort:  recastCamera->center_of_mass_overlay.second = STATE::ON; break;
	default:
		recastCamera->center_of_mass_overlay.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_OVERLAY_CENTRE_OF_MASS = ",
		ENUM_TO_STRING(recastCamera->center_of_mass_overlay.second));
}
void EPICSCameraInterface::update_OVERLAY_MASK(const struct event_handler_args args)
{
	std::lock_guard<std::mutex> lg(cam_interface_mtx);  // This now locked your mutex mtx.lock();
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->analysis_mask_overlay.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->analysis_mask_overlay.second = STATE::OFF; break;
	case GlobalConstants::one_ushort:  recastCamera->analysis_mask_overlay.second = STATE::ON; break;
	default:
		recastCamera->analysis_mask_overlay.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_OVERLAY_MASK = ",
		ENUM_TO_STRING(recastCamera->analysis_mask_overlay.second));
}





//void EPICSCameraInterface::update_MAGICK_NumCaptured_RBV(const struct event_handler_args args)
//{
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_NumCaptured_RBV ");
//}
//void EPICSCameraInterface::update_MAGICK_WriteFile_RBV(const struct event_handler_args args)
//{
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_WriteFile_RBV ");
//}
//
//void EPICSCameraInterface::update_MAGICK_WriteStatus(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_WriteStatus ");
//}
//
//void EPICSCameraInterface::update_MAGICK_WriteMessage(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_WriteMessage ");
//}
//
//void EPICSCameraInterface::update_MAGICK_Capture_RBV(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_Capture_RBV ");
//}
//
//void EPICSCameraInterface::update_MAGICK_NumCapture_RBV(const struct event_handler_args args) {
//	Camera* recastCamera = static_cast<Camera*>(args.usr);
//	messenger.printDebugMessage("update_MAGICK_NumCapture_RBV ");
//}
