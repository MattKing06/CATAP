#include "EPICSLaserEnergyMeterInterface.h"

LoggingSystem EPICSLaserEnergyMeterInterface::messenger;

EPICSLaserEnergyMeterInterface::EPICSLaserEnergyMeterInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSLaserEnergyMeterInterface::~EPICSLaserEnergyMeterInterface()
{
	messenger.printDebugMessage("EPICSLaserEnergyMeterInterface Destructor Called");
}
void EPICSLaserEnergyMeterInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == LaserEnergyMeterRecords::OVERRANGE)
	{
		pvStruct.updateFunction = this->updateOverRange;
	}
	else if (pvStruct.pvRecord == LaserEnergyMeterRecords::RUNSP)
	{
		pvStruct.updateFunction = this->updateStart;
	}
	else if (pvStruct.pvRecord == LaserEnergyMeterRecords::RANGESP)
	{
		pvStruct.updateFunction = this->updateRange;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSLaserEnergyMeterInterface::updateOverRange(const struct event_handler_args args)
{
	LaserEnergyMeter* recastLaser = getHardwareFromArgs<LaserEnergyMeter>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastLaser->overrange.first = pairToUpdate.first;
	recastLaser->overrange.second = pairToUpdate.second;	
	messenger.printDebugMessage("OVERRANGE VALUE FOR: " + recastLaser->getHardwareName() + ": "
		+ std::to_string(recastLaser->overrange.second));
}

void EPICSLaserEnergyMeterInterface::updateStart(const struct event_handler_args args)
{
	LaserEnergyMeter* recastLaser = getHardwareFromArgs<LaserEnergyMeter>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastLaser->start.first= pairToUpdate.first;
	recastLaser->start.second = pairToUpdate.second;
	switch (recastLaser->start.second)
	{
	case 0:
	{
		recastLaser->acquiring = false;
	}
	case 1:
	{
		recastLaser->acquiring = true;
	}
	}
	messenger.printDebugMessage("START VALUE FOR: " + recastLaser->getHardwareName() + ": "
		+ std::to_string(recastLaser->start.second));
}

void EPICSLaserEnergyMeterInterface::updateRange(const struct event_handler_args args)
{
	LaserEnergyMeter* recastLaser = getHardwareFromArgs<LaserEnergyMeter>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastLaser->range.first = pairToUpdate.first;
	recastLaser->range.second = (int)pairToUpdate.second;		
	messenger.printDebugMessage("RANGE VALUE FOR: " + recastLaser->getHardwareName() + ": "
		+ std::to_string(recastLaser->range.second));
}

void EPICSLaserEnergyMeterInterface::setRANGE(const int& value, const pvStruct& pv)
{
	putValue2(pv, value);
}

void EPICSLaserEnergyMeterInterface::setSTART(const int& value, const pvStruct& pv)
{
	putValue2(pv, value);
}