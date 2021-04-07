#include "EPICSChargeInterface.h"

LoggingSystem EPICSChargeInterface::messenger;

EPICSChargeInterface::EPICSChargeInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSChargeInterface::~EPICSChargeInterface()
{
	messenger.printDebugMessage("EPICSChargeInterface Destructor Called");
}
void EPICSChargeInterface::retrieveupdateFunctionForRecord(pvStruct &pvStruct) const
{
	if (pvStruct.pvRecord == ChargeRecords::Q)
	{
		pvStruct.updateFunction = this->updateQ;
	}
}

void EPICSChargeInterface::updateQ(const struct event_handler_args args)
{
	Charge* recastCharge = getHardwareFromArgs<Charge>(args);
	updateTimeStampDoublePair(args, recastCharge->q);
	recastCharge->setQ(recastCharge->q.second);
	messenger.printDebugMessage("Q VALUE FOR: " + recastCharge->getHardwareName() + ": "
		+ std::to_string(recastCharge->q.second));
}

void EPICSChargeInterface::setQ(const double& value, const pvStruct& pv)
{
	putValue2(pv, value);
}
