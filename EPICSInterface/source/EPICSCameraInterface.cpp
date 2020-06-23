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
	std::cout << "updateX_RBV" << std::endl;
	messenger.printDebugMessage("updateX_RBV");
	Camera* recastCam= static_cast<Camera*>(args.usr);
	//updateTimeStampDoublePair(args, recastCam->x_mm);
	messenger.printDebugMessage("updateX_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateY_RBV(const struct event_handler_args args)
{
	std::cout << "updateY_RBV" << std::endl;
	messenger.printDebugMessage("updateY_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateY_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateSigmaX_RBV(const struct event_handler_args args)
{

	messenger.printDebugMessage("updateSigmaX_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateSigmaX_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateSigmaY_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateSigmaY_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateSigmaY_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateCovXY_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateCovXY_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateCovXY_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateXPix_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateXPix_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateXPix_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateYPix_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateYPix_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateYPix_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateSigmaXPix_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateSigmaXPix_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateSigmaXPix_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateSigmaYPix_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateSigmaYPix_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateSigmaYPix_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateCovXYPix_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateCovXYPix_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateCovXYPix_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateAvgIntensity_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateAvgIntensity_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateAvgIntensity_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updateIntensity_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updateIntensity_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updateIntensity_RBV FOR: " + recastCam->getHardwareName());
}
void EPICSCameraInterface::updatePixMM_RBV(const struct event_handler_args args)
{
	messenger.printDebugMessage("updatePixMM_RBV");
	//Camera* recastCam = static_cast<Camera*>(args.usr);
	////updateTimeStampDoublePair(args, recastCam->x_mm);
	//messenger.printDebugMessage("updatePixMM_RBV FOR: " + recastCam->getHardwareName());
}
