#include <EPICSLaserMirrorInterface.h>

EPICSLaserMirrorInterface::EPICSLaserMirrorInterface() : EPICSInterface()
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

bool EPICSLaserMirrorInterface::setNewHorizontalPosition(const double value, const pvStruct& pv)
{
	std::cout << "setNewHorizontalPosition " << value << std::endl;

	int status = ca_put(pv.CHTYPE, pv.CHID, &value);
	MY_SEVCHK(status);
	status = ca_pend_io(CA_PEND_IO_TIMEOUT);
	return true;
}

bool EPICSLaserMirrorInterface::setNewVerticalPosition(const double value, const pvStruct& pv)
{
	std::cout << "setNewVerticalPosition " << value << std::endl;
	int status = ca_put(pv.CHTYPE, pv.CHID, &value);
	MY_SEVCHK(status);
	status = ca_pend_io(CA_PEND_IO_TIMEOUT);
	return true;
}

