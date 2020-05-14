#include <RFHeartbeat.h>

RFHeartbeat::RFHeartbeat()
{
}

RFHeartbeat::RFHeartbeat(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

RFHeartbeat::RFHeartbeat(const RFHeartbeat& copyRFHeartbeat)
{
}

RFHeartbeat::~RFHeartbeat()
{
}

void RFHeartbeat::setPVStructs()
{
}

void RFHeartbeat::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void RFHeartbeat::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void RFHeartbeat::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void RFHeartbeat::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

