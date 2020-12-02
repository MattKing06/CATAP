#include "EPICSCameraInterface.h"
#include "CameraPVRecords.h"

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
	if (pvStruct.pvRecord == HDF_WriteFile_RBV)
	{
	pvStruct.updateFunction = this->update_HDF_WriteFile_RBV;
	}
	else if (pvStruct.pvRecord == HDF_WriteStatus)
	{
		pvStruct.updateFunction = this->update_HDF_WriteStatus;
	}
	else if (pvStruct.pvRecord == HDF_WriteMessage)
	{
		pvStruct.updateFunction = this->update_HDF_WriteMessage;
	}
	else if (pvStruct.pvRecord == HDF_FileName_RBV)
	{
		pvStruct.updateFunction = this->update_HDF_FileName_RBV;
	}
	else if (pvStruct.pvRecord == HDF_FilePath_RBV)
	{
		pvStruct.updateFunction = this->update_HDF_FilePath_RBV;
	}
	else if (pvStruct.pvRecord == HDF_NumCaptured_RBV)
	{
		pvStruct.updateFunction = this->update_HDF_NumCaptured_RBV;
	}
	else if (pvStruct.pvRecord == HDF_Capture_RBV)
	{
		pvStruct.updateFunction = this->update_HDF_Capture_RBV;
	}
	else if (pvStruct.pvRecord == CAM_Acquire_RBV)
	{
		pvStruct.updateFunction = this->update_CAM_Acquire_RBV;
	}
	else if (pvStruct.pvRecord == HDF_NumCapture_RBV)
	{
		pvStruct.updateFunction = this->update_HDF_NumCapture_RBV;
	}
	else if (pvStruct.pvRecord == MAGICK_NumCaptured_RBV)
	{
		pvStruct.updateFunction = this->update_MAGICK_NumCaptured_RBV;
	}
	else if (pvStruct.pvRecord == MAGICK_WriteFile_RBV)
	{
		pvStruct.updateFunction = this->update_MAGICK_WriteFile_RBV;
	}
	else if (pvStruct.pvRecord == MAGICK_WriteStatus)
	{
		pvStruct.updateFunction = this->update_MAGICK_WriteStatus;
	}
	else if (pvStruct.pvRecord == MAGICK_WriteMessage)
	{
		pvStruct.updateFunction = this->update_MAGICK_WriteMessage;
	}
	else if (pvStruct.pvRecord == MAGICK_Capture_RBV)
	{
		pvStruct.updateFunction = this->update_MAGICK_Capture_RBV;
	}
	else if (pvStruct.pvRecord == MAGICK_NumCapture_RBV)
	{
		pvStruct.updateFunction = this->update_MAGICK_NumCapture_RBV;
	}
	else if (pvStruct.pvRecord == ANA_StepSize_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_StepSize_RBV;
	}
	else if (pvStruct.pvRecord == ANA_EnableCallbacks_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_EnableCallbacks_RBV;
	}
	else if (pvStruct.pvRecord == ANA_X_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_X_RBV;
	}
	else if (pvStruct.pvRecord == ANA_Y_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_Y_RBV;
	}
	else if (pvStruct.pvRecord == ANA_SigmaX_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_SigmaX_RBV;
	}
	else if (pvStruct.pvRecord == ANA_SigmaY_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_SigmaY_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CovXY_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_CovXY_RBV;
	}
	else if (pvStruct.pvRecord == ANA_AvgIntensity_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_AvgIntensity_RBV;
	}
	else if (pvStruct.pvRecord == ANA_Intensity_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_Intensity_RBV;
	}
	else if (pvStruct.pvRecord == ANA_XPix_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_XPix_RBV;
	}
	else if (pvStruct.pvRecord == ANA_YPix_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_YPix_RBV;
	}
	else if (pvStruct.pvRecord == ANA_SigmaXPix_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_SigmaXPix_RBV;
	}
	else if (pvStruct.pvRecord == ANA_SigmaYPix_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_SigmaYPix_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CovXYPix_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_CovXYPix_RBV;
	}
	else if (pvStruct.pvRecord == ANA_PixelResults_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_PixelResults_RBV;
	}
	else if (pvStruct.pvRecord == ANA_MaskXCenter_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_MaskXCenter_RBV;
	}
	else if (pvStruct.pvRecord == ANA_MaskYCenter_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_MaskYCenter_RBV;
	}
	else if (pvStruct.pvRecord == ANA_MaskXRad_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_MaskXRad_RBV;
	}
	else if (pvStruct.pvRecord == ANA_MaskYRad_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_MaskYRad_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CenterX_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_CenterX_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CenterY_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_CenterY_RBV;
	}
	else if (pvStruct.pvRecord == ANA_PixMM_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_PixMM_RBV;
	}
	else if (pvStruct.pvRecord == CAM_AcquireTime_RBV)
	{
		pvStruct.updateFunction = this->update_CAM_AcquireTime_RBV;
	}
	else if (pvStruct.pvRecord == CAM_AcquirePeriod_RBV)
	{
		pvStruct.updateFunction = this->update_CAM_AcquirePeriod_RBV;
	}
	else if (pvStruct.pvRecord == CAM_ArrayRate_RBV)
	{
		pvStruct.updateFunction = this->update_CAM_ArrayRate_RBV;
	}
	else if (pvStruct.pvRecord == CAM_Temperature_RBV)
	{
		pvStruct.updateFunction = this->update_CAM_Temperature_RBV;
	}
	else if (pvStruct.pvRecord == HDFB_Buffer_Trigger)
	{
	pvStruct.updateFunction = this->update_HDFB_Buffer_Trigger;
	}
	else if (pvStruct.pvRecord == HDFB_Buffer_FilePath_RBV)
	{
	pvStruct.updateFunction = this->update_HDFB_Buffer_FilePath_RBV;
	}
	else if (pvStruct.pvRecord == HDFB_Buffer_FileName_RBV)
	{
	pvStruct.updateFunction = this->update_HDFB_Buffer_FileName_RBV;
	}
	else if (pvStruct.pvRecord == HDFB_Buffer_FileNumber_RBV)
	{
	pvStruct.updateFunction = this->update_HDFB_Buffer_FileNumber_RBV;
	}
	else if (pvStruct.pvRecord == ROI1_MinX_RBV)
	{
	pvStruct.updateFunction = this->update_ROI1_MinX_RBV;
	}
	else if (pvStruct.pvRecord == ROI1_MinY_RBV)
	{
	pvStruct.updateFunction = this->update_ROI1_MinY_RBV;
	}
	else if (pvStruct.pvRecord == ROI1_SizeX_RBV)
	{
	pvStruct.updateFunction = this->update_ROI1_SizeX_RBV;
	}
	else if (pvStruct.pvRecord == ROI1_SizeY_RBV)
	{
	pvStruct.updateFunction = this->update_ROI1_SizeY_RBV;
	}
	else if (pvStruct.pvRecord == ROI1_ImageData_RBV)
	{
	pvStruct.updateFunction = this->update_ROI1_ImageData_RBV;
	}
	//else if (pvStruct.pvRecord == ROIandMask_SetX)
	//{
	//pvStruct.updateFunction = this->update_ROIandMask_SetX;
	//}
	//else if (pvStruct.pvRecord == ROIandMask_SetY)
	//{
	//pvStruct.updateFunction = this->update_ROIandMask_SetY;
	//}
	//else if (pvStruct.pvRecord == ROIandMask_SetXrad)
	//{
	//pvStruct.updateFunction = this->update_ROIandMask_SetXrad;
	//}
	//else if (pvStruct.pvRecord == ROIandMask_SetYrad)
	//{
	//pvStruct.updateFunction = this->update_ROIandMask_SetYrad;
	//}
	else if (pvStruct.pvRecord == ANA_UseFloor_RBV) // TODO ERROR?
	{
		pvStruct.updateFunction = this->update_ANA_UseFloor_RBV;
	}
	else if (pvStruct.pvRecord == ANA_FloorLevel_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_FloorLevel_RBV;
	}
	else if (pvStruct.pvRecord == ANA_FlooredPoints_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_FlooredPoints_RBV;
	}
	else if (pvStruct.pvRecord == ANA_FlooredPercent_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_FlooredPercent_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CPU_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_CPU_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CPU_CropSubMask_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_CPU_CropSubMask_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CPU_Npoint_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_CPU_Npoint_RBV;
	}
	else if (pvStruct.pvRecord == ANA_CPU_Dot_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_CPU_Dot_RBV;
	}
	else if (pvStruct.pvRecord == ANA_PixW_RBV)
	{
	pvStruct.updateFunction = this->update_ANA_PixW_RBV;
	}
	else if (pvStruct.pvRecord == ANA_PixH_RBV)
	{
		pvStruct.updateFunction = this->update_ANA_PixH_RBV;
	}
	else if (pvStruct.pvRecord == ANA_UseBkgrnd)
	{
		pvStruct.updateFunction = this->update_ANA_UseBkgrnd;
	}
	else if (pvStruct.pvRecord == ANA_UseNPoint)
	{
		pvStruct.updateFunction = this->update_ANA_UseNPoint;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}
void EPICSCameraInterface::update_HDF_WriteFile_RBV(const struct event_handler_args args)
{
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
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteFile_RBV = " + ENUM_TO_STRING(recastCamera->write_state_check.second));
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
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteStatus = " + ENUM_TO_STRING(recastCamera->write_state.second));
}
void EPICSCameraInterface::update_HDF_WriteMessage(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const dbr_time_char* new_data = (const struct dbr_time_char*)args.dbr;
	recastCamera->write_error_message.first = new_data->stamp;
	const dbr_char_t* value;
	value = &new_data->value;
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	// char array must have a const size (defined at compile time (or we use an array on the heap with new and delete)
	char dummy_char[256]; // MAGIC NUMBER
	for(int i = 0; i != 256; ++i)
	{
		dummy_char[i] = value[i];
		//std::cout << (int)value[i] << std::endl;
	}
	std::string dummy_string(dummy_char);
	recastCamera->write_error_message.second = dummy_string;
	//messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteMessage (write_error_message) = ", recastCamera->write_error_message.second);
}
void EPICSCameraInterface::update_HDF_NumCaptured_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//messenger.printDebugMessage("update_HDF_NumCaptured_RBV ");
}
void EPICSCameraInterface::update_HDF_Capture_RBV(const struct event_handler_args args)
{
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
	//messenger.printDebugMessage(recastCamera->getHardwareName()," update_HDF_Capture_RBV = " + ENUM_TO_STRING(recastCamera->capture_state.second));
}
void EPICSCameraInterface::update_CAM_Acquire_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->acquire_state.first = new_value.first;
	switch(new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->acquire_state.second = STATE::NOT_ACQUIRING; break;
	case GlobalConstants::one_ushort:  recastCamera->acquire_state.second = STATE::ACQUIRING; break;
	default:
		recastCamera->acquire_state.second = STATE::ERR;
	}
	std::cout << recastCamera->getHardwareName() << " new acquire state = " << ENUM_TO_STRING(recastCamera->acquire_state.second) << std::endl;
}
void EPICSCameraInterface::update_LED_Sta(const struct event_handler_args args)
{
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
}
void EPICSCameraInterface::update_ANA_PixMM_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->pixel_to_mm);
}
void EPICSCameraInterface::update_HDF_NumCapture_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_HDF_NumCapture_RBV ");
}
void EPICSCameraInterface::update_ANA_StepSize_RBV(const struct event_handler_args args) 
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->step_size);
}
void EPICSCameraInterface::update_ANA_EnableCallbacks_RBV(const struct event_handler_args args) 
{
	//messenger.printDebugMessage("update_ANA_EnableCallbacks_RBV ");
	//std::cout << "update_ANA_EnableCallbacks_RBV????" << std::endl;
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
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_EnableCallbacks_RBV = ", ENUM_TO_STRING(recastCamera->analysis_state.second));
}
void EPICSCameraInterface::update_ANA_X_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->x_mm);
	recastCamera->x_mm_rs.Push(recastCamera->x_mm.second);
	//messenger.printDebugMessage("update_ANA_X_RBV, val =  ", recastCamera->x_mm.second);

	//std::pair<epicsTimeStamp, double > x_pix;
	///*! latest vertical position (expected value) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > y_pix;
	///*! latest horizontal rms (width) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > sigma_x_pix;
	///*! latest vertical rms (width) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > sigma_y_pix;
	///*! latest tilt rms (width) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > sigma_xy_pix;
	///*! latest horizontal position (expected value) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > x_mm;
	///*! latest vertical position (expected value) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > y_mm;
	///*! latest horizontal rms (width) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > sigma_x_mm;
	///*! latest vertical rms (width) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > sigma_y_mm;
	///*! latest tilt rms (width) in pixels. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > sigma_xy_mm;
	//*! latest Sum Intensity (sum of pixel values). Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > sum_intensity;
	///*! latest Average Intensity (mean of pixel values). Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, double > avg_intensity;

}
void EPICSCameraInterface::update_ANA_Y_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_Y_RBV ");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->y_mm);
	//std::cout << "update_ANA_Y_RBV " << recastCamera->getHardwareName() << std::endl;
	recastCamera->y_mm_rs.Push(recastCamera->y_mm.second);
}
void EPICSCameraInterface::update_ANA_SigmaX_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_SigmaX_RBV ");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_x_mm);
	//std::cout << "update_ANA_Y_RBV " << recastCamera->getHardwareName() << std::endl;
	recastCamera->sigma_x_mm_rs.Push(recastCamera->sigma_x_mm.second);
}
void EPICSCameraInterface::update_ANA_SigmaY_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_SigmaY_RBV ");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_y_mm);
	//std::cout << "update_ANA_Y_RBV " << recastCamera->getHardwareName() << std::endl;
	recastCamera->sigma_y_mm_rs.Push(recastCamera->sigma_y_mm.second);
}
void EPICSCameraInterface::update_ANA_CovXY_RBV(const struct event_handler_args args)
{
	//messenger.printDebugMessage("update_ANA_CovXY_RBV ");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_xy_mm);
	//std::cout << "update_ANA_Y_RBV " << recastCamera->getHardwareName() << std::endl;
}
void EPICSCameraInterface::update_ANA_AvgIntensity_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_AvgIntensity_RBV ");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->avg_intensity);
	//std::cout << "update_ANA_AvgIntensity_RBV " << recastCamera->getHardwareName() << std::endl;
}
void EPICSCameraInterface::update_ANA_Intensity_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_Intensity_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sum_intensity);
	//std::cout << "update_ANA_AvgIntensity_RBV " << recastCamera->getHardwareName() << std::endl;
}
void EPICSCameraInterface::update_ANA_XPix_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_XPix_RBV ");
	//std::cout << "update_ANA_XPix_RBV" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->x_pix);
	//std::cout << "update_ANA_AvgIntensity_RBV " << recastCamera->getHardwareName() << std::endl;
}
void EPICSCameraInterface::update_ANA_YPix_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_YPix_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->y_pix);
}
void EPICSCameraInterface::update_ANA_SigmaXPix_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_SigmaXPix_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_x_pix);
}
void EPICSCameraInterface::update_ANA_SigmaYPix_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_SigmaYPix_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_y_pix);
}
void EPICSCameraInterface::update_ANA_CovXYPix_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_CovXYPix_RBV ");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_xy_pix);
}
void EPICSCameraInterface::update_ANA_PixelResults_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_PixelResults_RBV ");
}
void EPICSCameraInterface::update_ANA_MaskXCenter_RBV(const struct event_handler_args args) {
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_x_center);
	messenger.printDebugMessage("update_ANA_MaskXCenter_RBV ", recastCamera->getHardwareName(),
	" new_val = ", recastCamera->mask_x_center.second);
}
void EPICSCameraInterface::update_ANA_MaskYCenter_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_MaskYCenter_RBV ");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_y_center);
}
void EPICSCameraInterface::update_ANA_MaskXRad_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_MaskXRad_RBV");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_x_radius);
}
void EPICSCameraInterface::update_ANA_MaskYRad_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_MaskYRad_RBV");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->mask_y_radius);
}
void EPICSCameraInterface::update_ANA_CenterX_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_CenterX_RBV");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->x_center);
}
void EPICSCameraInterface::update_ANA_CenterY_RBV(const struct event_handler_args args)
{
	//messenger.printDebugMessage("update_ANA_CenterY_RBV");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->y_center);
}
void EPICSCameraInterface::update_CAM_AcquireTime_RBV(const struct event_handler_args args)
{
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	//messenger.printDebugMessage("update_CAM_AcquireTime_RBV");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->acquire_time);
}
void EPICSCameraInterface::update_CAM_AcquirePeriod_RBV(const struct event_handler_args args)
{
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	//messenger.printDebugMessage("update_CAM_AcquirePeriod_RBV");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->acquire_period);
}
void EPICSCameraInterface::update_CAM_ArrayRate_RBV(const struct event_handler_args args)
{
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	//messenger.printDebugMessage("update_CAM_ArrayRate_RBV");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->array_rate);
}
void EPICSCameraInterface::update_CAM_Temperature_RBV(const struct event_handler_args args)
{
	//messenger.printDebugMessage("update_CAM_Temperature_RBV");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->temperature);
}
void EPICSCameraInterface::update_ANA_UseNPoint(const struct event_handler_args args)
{
	//messenger.printDebugMessage("update_ANA_UseNPoint");
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, unsigned short> r;
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastCamera->use_npoint.first = tv->stamp;
	switch (tv->value)
	{
	case 0: recastCamera->use_npoint.second = STATE::NOT_USING_NPOINT; break;
	case 1:  recastCamera->use_npoint.second = STATE::USING_NPOINT; break;
	default:
		recastCamera->use_npoint.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseNPoint = ", ENUM_TO_STRING(recastCamera->use_npoint.second));

}
void EPICSCameraInterface::update_ANA_UseBkgrnd(const struct event_handler_args args)
{
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
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseBkgrnd = ", ENUM_TO_STRING(recastCamera->use_background.second));
}
void EPICSCameraInterface::update_HDF_FilePath_RBV(const struct event_handler_args args)
{
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
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_FilePath_RBV = ", recastCamera->save_filepath.second);
}
void EPICSCameraInterface::update_HDF_FileName_RBV(const struct event_handler_args args)
{
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
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_FileName_RBV = ", recastCamera->save_filename.second);
}
void EPICSCameraInterface::update_HDFB_Buffer_Trigger(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, char> buffer_trigger;
	// TODO
}
void EPICSCameraInterface::update_HDFB_Buffer_FilePath_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, std::string> buffer_filepath;
	// TODO
}
void EPICSCameraInterface::update_HDFB_Buffer_FileName_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, std::string> buffer_filename;
	// TODO
}
void EPICSCameraInterface::update_HDFB_Buffer_FileNumber_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->buffer_filenumber);
}
void EPICSCameraInterface::update_ROI1_MinX_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->roi_min_x);
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ROI1_MinX_RBV = ", recastCamera->roi_min_x.second);

}
void EPICSCameraInterface::update_ROI1_MinY_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, long> roi_min_y;
	updateTimeStampLongPair(args, recastCamera->roi_min_y);
}
void EPICSCameraInterface::update_ROI1_SizeX_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->roi_size_x);
}
void EPICSCameraInterface::update_ROI1_SizeY_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->roi_size_y);
}
void EPICSCameraInterface::update_ROI1_ImageData_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	// TODO 
}

 void EPICSCameraInterface::update_ANA_UseFloor_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastCamera->use_floor.first = tv->stamp;
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseFloor_RBV value = ", tv->value);
	switch (tv->value)
	{
	case 0: recastCamera->use_floor.second = STATE::NOT_USING_FLOOR; break;
	case 1:  recastCamera->use_floor.second = STATE::USING_FLOOR; break;
	default:
		recastCamera->use_floor.second = STATE::ERR;
	}
	messenger.printDebugMessage(recastCamera->hardwareName, " update_ANA_UseFloor_RBV = ", ENUM_TO_STRING(recastCamera->use_floor.second));

}
void EPICSCameraInterface::update_ANA_FloorLevel_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->floor_level);
}
void EPICSCameraInterface::update_ANA_FlooredPoints_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->floored_pts_count);
}
void EPICSCameraInterface::update_ANA_FlooredPercent_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->floored_pts_percent);
}
void EPICSCameraInterface::update_ANA_CPU_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, long> cpu_total;
	updateTimeStampLongPair(args, recastCamera->cpu_total);
}
void EPICSCameraInterface::update_ANA_CPU_CropSubMask_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, long> cpu_crop_sub_mask;
	updateTimeStampLongPair(args, recastCamera->cpu_crop_sub_mask);
}
void EPICSCameraInterface::update_ANA_CPU_Npoint_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, long> cpu_npoint;
	updateTimeStampLongPair(args, recastCamera->cpu_npoint);
}
void EPICSCameraInterface::update_ANA_CPU_Dot_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, long> cpu_dot;
	updateTimeStampLongPair(args, recastCamera->cpu_dot);
}
void EPICSCameraInterface::update_ANA_PixW_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampLongPair(args, recastCamera->pixel_width);
}
void EPICSCameraInterface::update_ANA_PixH_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	//std::pair<epicsTimeStamp, long> pixel_height;
	updateTimeStampLongPair(args, recastCamera->pixel_height);
}
















void EPICSCameraInterface::update_MAGICK_NumCaptured_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_MAGICK_NumCaptured_RBV ");
}
void EPICSCameraInterface::update_MAGICK_WriteFile_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_MAGICK_WriteFile_RBV ");
}

void EPICSCameraInterface::update_MAGICK_WriteStatus(const struct event_handler_args args) {
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_MAGICK_WriteStatus ");
}

void EPICSCameraInterface::update_MAGICK_WriteMessage(const struct event_handler_args args) {
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_MAGICK_WriteMessage ");
}

void EPICSCameraInterface::update_MAGICK_Capture_RBV(const struct event_handler_args args) {
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_MAGICK_Capture_RBV ");
}

void EPICSCameraInterface::update_MAGICK_NumCapture_RBV(const struct event_handler_args args) {
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_MAGICK_NumCapture_RBV ");
}
