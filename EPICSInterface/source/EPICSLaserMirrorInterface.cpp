#include <EPICSLaserMirrorInterface.h>

EPICSLaserMirrorInterface::EPICSLaserMirrorInterface()
{
}

EPICSLaserMirrorInterface::~EPICSLaserMirrorInterface()
{
}

void EPICSLaserMirrorInterface::retrieveUpdateFunctionForRecord(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == LaserMirrorRecords::H_POS)
	{
		pvStruct.updateFunction = this->updateHorizontalPosition;
	}
	if (pvStruct.pvRecord == LaserMirrorRecords::V_POS)
	{
		pvStruct.updateFunction = this->updateVerticalPosition;
	}
	else
	{
		messenger.printMessage("!! WARNING !! NO UPDATE FUNCTION FOR: ", pvStruct.fullPVName);
	}
}

void EPICSLaserMirrorInterface::updateHorizontalPosition(const struct event_handler_args args)
{
	LaserMirror* recastMirror = getHardwareFromArgs<LaserMirror>(args);
	updateTimeStampDoublePair(args, recastMirror->currentHorizontalPosition);
	std::cout << "UPDATED HORIZONTAL POSITION" << std::endl;
}

void EPICSLaserMirrorInterface::updateVerticalPosition(const struct event_handler_args args)
{
	LaserMirror* recastMirror = getHardwareFromArgs<LaserMirror>(args);
	updateTimeStampDoublePair(args, recastMirror->currentVerticalPosition);
	std::cout << "UPDATED VERTICAL POSITION" << std::endl;
}

void EPICSLaserMirrorInterface::moveHorizontal(const double& value, const pvStruct& pv)
{
	putValue(pv, value);
}

void EPICSLaserMirrorInterface::moveVertical(const double& value, const pvStruct& pv)
{
	putValue(pv, value);
}
