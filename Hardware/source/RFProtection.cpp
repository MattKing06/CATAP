#include <RFProtection.h>

RFProtection::RFProtection()
{
}

RFProtection::RFProtection(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

RFProtection::RFProtection(const RFProtection& copyRFProtection)
{
}

RFProtection::~RFProtection()
{
}

void RFProtection::setPVStructs()
{
}

void RFProtection::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void RFProtection::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void RFProtection::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void RFProtection::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}
