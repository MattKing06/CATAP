#include "EPICSChargeInterface.h"

LoggingSystem EPICSChargeInterface::messenger;

EPICSChargeInterface::EPICSChargeInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSChargeInterface::~EPICSChargeInterface()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage("EPICSChargeInterface Destructor Called");
}
void EPICSChargeInterface::retrieveUpdateFunctionForRecord(pvStruct &pvStruct) const
{
	if (pvStruct.pvRecord == "Q")
	{
		pvStruct.updateFunction = this->updateQ;
	}
}

void EPICSChargeInterface::updateQ(const struct event_handler_args args)
{
	Charge* recastCharge = getHardwareFromArgs<Charge>(args);
	setPVTimeStampFromArgs(recastCharge->pvStructs.at("Q"), args);
	double value = returnValueFromArgsAsDouble(args);
	recastCharge->setQ(value);
	messenger.printDebugMessage("Q VALUE FOR: " + recastCharge->getHardwareName() + ": "
		+ std::to_string(value));
	messenger.printDebugMessage(" CALLED UPDATE Q ");
}