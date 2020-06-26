#include <EPICSCameraInterface.h>
#include <iostream>


LoggingSystem EPICSCameraInterface::messenger;
EPICSCameraInterface::EPICSCameraInterface()
	: EPICSInterface()
{
	messenger = LoggingSystem(true, true);
}

EPICSCameraInterface::EPICSCameraInterface(const EPICSCameraInterface& copyInterface)
{
	messenger.printDebugMessage("EPICSCameraInterface Copy Constructor Called");
}

EPICSCameraInterface::~EPICSCameraInterface()
{
}

void EPICSCameraInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == CameraRecords::ANA_X_RBV)
	{
		pvStruct.updateFunction = this->updateX_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_Y_RBV)
	{
		pvStruct.updateFunction = this->updateY_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_SigmaX_RBV)
	{
		pvStruct.updateFunction = this->updateSigmaX_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_SigmaY_RBV)
	{
		pvStruct.updateFunction = this->updateSigmaY_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_CovXY_RBV)
	{
		pvStruct.updateFunction = this->updateCovXY_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_XPix_RBV)
	{
		pvStruct.updateFunction = this->updateXPix_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_YPix_RBV)
	{
		pvStruct.updateFunction = this->updateYPix_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_SigmaXPix_RBV)
	{
		pvStruct.updateFunction = this->updateSigmaXPix_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_SigmaYPix_RBV)
	{
		pvStruct.updateFunction = this->updateSigmaYPix_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_CovXYPix_RBV)
	{
		pvStruct.updateFunction = this->updateCovXYPix_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_AvgIntensity_RBV)
	{
		pvStruct.updateFunction = this->updateAvgIntensity_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_Intensity_RBV)
	{
		pvStruct.updateFunction = this->updateIntensity_RBV;
	}
	else if (pvStruct.pvRecord == CameraRecords::ANA_PixMM_RBV)
	{
		pvStruct.updateFunction = this->updatePixMM_RBV;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSCameraInterface::updateX_RBV(const struct event_handler_args args)
{
	Camera* recastCam= static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateX_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->x_mm.second);
}
void EPICSCameraInterface::updateY_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->y_mm);
	//messenger.printDebugMessage("updateY_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->y_mm.second);
}
void EPICSCameraInterface::updateSigmaX_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->sigma_x_mm);
	//messenger.printDebugMessage("updateSigmaX_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->sigma_x_mm.second);
}
void EPICSCameraInterface::updateSigmaY_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->sigma_y_mm);
	//messenger.printDebugMessage("updateSigmaY_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->sigma_y_mm.second);
}
void EPICSCameraInterface::updateCovXY_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->sigma_xy_mm);
	//messenger.printDebugMessage("updateCovXY_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->sigma_xy_mm.second);
}
void EPICSCameraInterface::updateXPix_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->x_pix);
	//messenger.printDebugMessage("updateXPix_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->x_pix.second);
}
void EPICSCameraInterface::updateYPix_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->y_pix);
	//messenger.printDebugMessage("updateYPix_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->y_pix.second);
}
void EPICSCameraInterface::updateSigmaXPix_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->sigma_x_pix);
	//messenger.printDebugMessage("updateSigmaXPix_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->sigma_x_pix.second);
}
void EPICSCameraInterface::updateSigmaYPix_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->sigma_y_pix);
	//messenger.printDebugMessage("updateSigmaYPix_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->sigma_y_pix.second);
}
void EPICSCameraInterface::updateCovXYPix_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->sigma_xy_pix);
	//messenger.printDebugMessage("updateCovXYPix_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->sigma_xy_mm.second);
}
void EPICSCameraInterface::updateAvgIntensity_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->avg_intensity);
	//messenger.printDebugMessage("updateAvgIntensity_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->avg_intensity.second);
}
void EPICSCameraInterface::updateIntensity_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->sum_intensity);
	//messenger.printDebugMessage("updateIntensity_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->sum_intensity.second);
}
void EPICSCameraInterface::updatePixMM_RBV(const struct event_handler_args args)
{
	Camera* recastCam = static_cast<Camera*>(args.usr);
	updateTimeStampDoublePair(args, recastCam->pix2mm);
	//messenger.printDebugMessage("updatePixMM_RBV FOR: " + recastCam->getHardwareName(), " ", recastCam->pix2mm.second);
}
