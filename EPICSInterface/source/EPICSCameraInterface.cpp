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
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteFile_RBV = " + ENUM_TO_STRING(recastCamera->write_state_check.second));
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
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteStatus = " + ENUM_TO_STRING(recastCamera->write_state.second));
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
	messenger.printDebugMessage(recastCamera->hardwareName, " update_HDF_WriteMessage (write_error_message) = ", recastCamera->write_error_message.second);
}
void EPICSCameraInterface::update_HDF_NumCaptured_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_HDF_NumCaptured_RBV ");
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
	messenger.printDebugMessage(recastCamera->getHardwareName()," update_HDF_Capture_RBV = " + ENUM_TO_STRING(recastCamera->capture_state.second));
}
void EPICSCameraInterface::update_CAM_Acquire_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->acquire_status.first = new_value.first;
	switch(new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->acquire_status.second = STATE::NOT_ACQUIRING; break;
	case GlobalConstants::one_ushort:  recastCamera->acquire_status.second = STATE::ACQUIRING; break;
	default:
		recastCamera->acquire_status.second = STATE::ERR;
	}
	std::cout << recastCamera->getHardwareName() << " new acquire state = " << ENUM_TO_STRING(recastCamera->acquire_status.second) << std::endl;
}
void EPICSCameraInterface::update_HDF_NumCapture_RBV(const struct event_handler_args args)
{
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	messenger.printDebugMessage("update_HDF_NumCapture_RBV ");
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
void EPICSCameraInterface::update_ANA_StepSize_RBV(const struct event_handler_args args) {

	Camera* recastCamera = static_cast<Camera*>(args.usr);

	//messenger.printDebugMessage("update_ANA_StepSize_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_EnableCallbacks_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_EnableCallbacks_RBV ");
	std::cout << "update_ANA_EnableCallbacks_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastCamera->analysis_status.first = new_value.first;
	switch (new_value.second)
	{
	case GlobalConstants::zero_ushort: recastCamera->analysis_status.second = STATE::NOT_ANALYSING; break;
	case GlobalConstants::one_ushort:  recastCamera->analysis_status.second = STATE::ANALYSING; break;
	default:
		recastCamera->analysis_status.second = STATE::ERR;
	}
}
void EPICSCameraInterface::update_ANA_X_RBV(const struct event_handler_args args)
{
	//messenger.printDebugMessage("update_ANA_X_RBV ");

	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->x_mm);
	recastCamera->x_mm_rs.Push(recastCamera->x_mm.second);
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

	///*! latest Sum Intensity (sum of pixel values). Value and epicstimestamp.	*/
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
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	Camera* recastCamera = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCamera->sigma_xy_pix);
}
void EPICSCameraInterface::update_ANA_PixelResults_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_PixelResults_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_MaskXCenter_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_MaskXCenter_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_MaskYCenter_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_MaskYCenter_RBV ");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_MaskXRad_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_MaskXRad_RBV");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_MaskYRad_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_MaskYRad_RBV");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_CenterX_RBV(const struct event_handler_args args) {
	//messenger.printDebugMessage("update_ANA_CenterX_RBV");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_CenterY_RBV(const struct event_handler_args args)
{
	//messenger.printDebugMessage("update_ANA_CenterY_RBV");
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
}
void EPICSCameraInterface::update_ANA_PixMM_RBV(const struct event_handler_args args)
{
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	//messenger.printDebugMessage("update_ANA_PixMM_RBV");
}
void EPICSCameraInterface::update_CAM_AcquireTime_RBV(const struct event_handler_args args)
{
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	//messenger.printDebugMessage("update_CAM_AcquireTime_RBV");
}
void EPICSCameraInterface::update_CAM_AcquirePeriod_RBV(const struct event_handler_args args)
{
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	//messenger.printDebugMessage("update_CAM_AcquirePeriod_RBV");
}
void EPICSCameraInterface::update_CAM_ArrayRate_RBV(const struct event_handler_args args)
{
	//std::cout << "update_CAM_ArrayRate_RBV????" << std::endl;
	//messenger.printDebugMessage("update_CAM_ArrayRate_RBV");
}
void EPICSCameraInterface::update_CAM_Temperature_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CAM_Temperature_RBV");
}
void EPICSCameraInterface::update_ANA_UseNPoint(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_ANA_UseNPoint");
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
	//std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
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