#include "EPICSMagnetInterface.h"

LoggingSystem EPICSMagnetInterface::messenger;

EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSMagnetInterface::~EPICSMagnetInterface()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage("EPICSMagnetInterface Destructor Called");
}
void EPICSMagnetInterface::retrieveUpdateFunctionForRecord(pvStruct &pvStruct) const
{
	
	if (pvStruct.pvRecord == "GETSETI")
	{
		pvStruct.updateFunction = this->updateCurrent;
	}
	if (pvStruct.pvRecord == "RPOWER")
	{
		pvStruct.updateFunction = this->updatePSUState;
	}
	if (pvStruct.pvRecord == "READI")
	{
		pvStruct.updateFunction = this->updateREADI;
	}
	if (pvStruct.pvRecord == "RILK")
	{
		pvStruct.updateFunction = this->updateRILK;
	}
}

void EPICSMagnetInterface::updateCurrent(const struct event_handler_args args)
{
	Magnet* recastMagnet = getHardwareFromArgs<Magnet>(args);
	setPVTimeStampFromArgs(recastMagnet->pvStructs.at("GETSETI"), args);
	double value = returnValueFromArgsAs<double>(args);
	recastMagnet->setCurrent(value);
	messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("GETSETI").time),
														"GETSETI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ std::to_string(value));
}

void EPICSMagnetInterface::updatePSUState(const struct event_handler_args args)
{
	Magnet* recastMagnet = getHardwareFromArgs<Magnet>(args);
	setPVTimeStampFromArgs(recastMagnet->pvStructs.at("RPOWER"), args);
	STATE value = returnValueFromArgsAs<STATE>(args);
	recastMagnet->setPSUState(value);
	messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("RPOWER").time),
														"RPOWER VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ std::to_string(value));
}

void EPICSMagnetInterface::updateREADI(const struct event_handler_args args)
{

	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	setPVTimeStampFromArgs(recastMagnet->pvStructs.at("READI"), args);
	double value = returnValueFromArgsAs<double>(args);
	recastMagnet->setRICurrent(value);
	messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("READI").time),
														"READI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ std::to_string(value));

}

void EPICSMagnetInterface::updateRILK(const struct event_handler_args args)
{

	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	setPVTimeStampFromArgs(recastMagnet->pvStructs.at("RILK"), args);
	STATE value = returnValueFromArgsAs<STATE>(args);
	recastMagnet->setILKState(value);
	messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("RILK").time),
		"RILK VALUE FOR: " + recastMagnet->getHardwareName() + ": "
		+ std::to_string(value));
}

void EPICSMagnetInterface::setNewCurrent(const double &value, const pvStruct &pv) const
{
	//we have checked that pvRecord is SETI before reaching here.
	putValue(pv, value);
	messenger.printDebugMessage("SENT CURRENT " + std::to_string(value) + " FOR " + pv.fullPVName + " TO EPICS ");
}

void EPICSMagnetInterface::setNewPSUState(const STATE& value, const pvStruct& pv) const
{
	putValue(pv, static_cast<int>(value));
	messenger.printDebugMessage("SENT POWER " + std::to_string(value) + " FOR " + pv.fullPVName + " TO EPICS");
}
