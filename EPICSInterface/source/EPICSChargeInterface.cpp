#include "EPICSChargeInterface.h"

LoggingSystem EPICSChargeInterface::messenger;

EPICSBPMInterface::EPICSChargeInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSBPMInterface::~EPICSChargeInterface()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage("EPICSChargeInterface Destructor Called");
}
void EPICSBPMInterface::retrieveUpdateFunctionForRecord(pvStruct &pvStruct) const
{
	
	if (pvStruct.pvRecord == "Q")
	{
		pvStruct.updateFunction = this->updateQ;
	}
}

void EPICSBPMInterface::updateQ(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Charge* recastCharge = static_cast<Charge*>(args.usr);
		recastCharge->setQ(*(double*)(args.dbr));
	}
	else if (args.type == DBR_TIME_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Charge* recastCharge = static_cast<Charge*>(args.usr);
		const struct dbr_time_double* pTD = (const struct dbr_time_double*)(args.dbr);
		recastCharge->pvStructs.at("Q").time = pTD->stamp;
		recastCharge->setQ(pTD->value);
		messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastCharge->pvStructs.at("Q").time),
			"CHARGE VALUE FOR: " + recastCharge->getHardwareName() + ": "
			+ std::to_string(pTD->value));
	}
	messenger.printDebugMessage(" CALLED UPDATE Q ");
}