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
	else if (pvStruct.pvRecord == LaserHWPRecords::ENABLE)
	{
		pvStruct.updateFunction = this->updateENABLE;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSLaserHWPInterface::updateHWPGet(const struct event_handler_args args)
{
	LaserHWP* recastLaserHWP = getHardwareFromArgs<LaserHWP>(args);
	updateTimeStampDoublePair(args, recastLaserHWP->hwpread);
	messenger.printDebugMessage("RPOS VALUE FOR: " + recastLaserHWP->getHardwareName() + ": "
		+ std::to_string(recastLaserHWP->hwpread.second));
}

void EPICSLaserHWPInterface::updateHWPSet(const struct event_handler_args args)
{
	LaserHWP* recastLaserHWP = getHardwareFromArgs<LaserHWP>(args);
	updateTimeStampDoublePair(args, recastLaserHWP->hwpset);
	messenger.printDebugMessage("MABS VALUE FOR: " + recastLaserHWP->getHardwareName() + ": "
		+ std::to_string(recastLaserHWP->hwpset.second));
}

void EPICSLaserHWPInterface::updateENABLE(const struct event_handler_args args)
{
	LaserHWP* recastLaserHWP = getHardwareFromArgs<LaserHWP>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastLaserHWP->hwp_enable.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: recastLaserHWP->hwp_enable.second = STATE::DISABLED; break;
	case GlobalConstants::one_int:  recastLaserHWP->hwp_enable.second = STATE::ENABLED; break;
	default:
		recastLaserHWP->hwp_enable.second = STATE::ERR;
	}
	messenger.printDebugMessage("MABS VALUE FOR: " + recastLaserHWP->getHardwareName() + ": "
		+ std::to_string(recastLaserHWP->hwpset.second));
}



void EPICSLaserHWPInterface::setMABS(const double& value, const pvStruct& pv)
{
	putValue2(pv, value);
}
