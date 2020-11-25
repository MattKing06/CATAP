#include "EPICSLaserHWPInterface.h"

LoggingSystem EPICSLaserHWPInterface::messenger;

EPICSLaserHWPInterface::EPICSLaserHWPInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSLaserHWPInterface::~EPICSLaserHWPInterface()
{
	messenger.printDebugMessage("EPICSLaserHWPInterface Destructor Called");
}
void EPICSLaserHWPInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == LaserHWPRecords::RPOS)
	{
		pvStruct.updateFunction = this->updateHWPGet;
	}
	else if (pvStruct.pvRecord == LaserHWPRecords::MABS)
	{
		pvStruct.updateFunction = this->updateHWPSet;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSLaserHWPInterface::updateHWPGet(const struct event_handler_args args)
{
	LaserHWP* recastLaser = getHardwareFromArgs<LaserHWP>(args);
	updateTimeStampDoublePair(args, recastLaser->hwpread);
	messenger.printDebugMessage("RPOS VALUE FOR: " + recastLaser->getHardwareName() + ": "
		+ std::to_string(recastLaser->hwpread.second));
}

void EPICSLaserHWPInterface::updateHWPSet(const struct event_handler_args args)
{
	LaserHWP* recastLaser = getHardwareFromArgs<LaserHWP>(args);
	updateTimeStampDoublePair(args, recastLaser->hwpset);
	messenger.printDebugMessage("MABS VALUE FOR: " + recastLaser->getHardwareName() + ": "
		+ std::to_string(recastLaser->hwpset.second));
}

void EPICSLaserHWPInterface::setMABS(const double& value, const pvStruct& pv)
{
	putValue2(pv, value);
}
