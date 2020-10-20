#include <EPICSRFProtectionInterface.h>
#include <RFProtectionPVRecords.h>

LoggingSystem EPICSRFProtectionInterface::messenger;

EPICSRFProtectionInterface::EPICSRFProtectionInterface() : EPICSInterface()
{
	messenger = LoggingSystem(true, true);
}

EPICSRFProtectionInterface::EPICSRFProtectionInterface(const EPICSRFProtectionInterface& copyInterface)
{
	messenger.printDebugMessage("EPICSRFProtectionInterface Copy Constructor Called");
}

EPICSRFProtectionInterface::~EPICSRFProtectionInterface()
{
	messenger.printDebugMessage("EPICSRFProtectionInterface Destructor Called");
}

void EPICSRFProtectionInterface::retrieveUpdateFunctionForRecord(const pvStruct& pv)
{
	if (pv.pvRecord == RFProtectionRecords::Sta)
	{
		pv.updateFunction = updateProtectionStatus;
	}
	if (pv.pvRecord == RFProtectionRecords::Cmi)
	{
		pv.updateFunction = updateProtectionCmi;
	}
}

void EPICSRFProtectionInterface::updateProtectionStatus(const struct event_handler_args args)
{
	RFProtection* protection = getHardwareFromArgs<RFProtection>(args);
	messenger.printMessage(protection->getHardwareName());
}

void EPICSRFProtectionInterface::updateProtectionCmi(const struct event_handler_args args)
{
	RFProtection* protection = getHardwareFromArgs<RFProtection>(args);
	messenger.printMessage(protection->getHardwareName());
}

bool EPICSRFProtectionInterface::setProtectionOn(const pvStruct& pv)
{
	return false;
}

bool EPICSRFProtectionInterface::setProtectionOff(const pvStruct& pv)
{
	return false;
}

bool EPICSRFProtectionInterface::resetProtection(const pvStruct& pv)
{
	return false;
}
