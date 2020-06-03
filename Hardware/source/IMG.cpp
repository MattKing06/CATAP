#include <IMG.h>

IMG::IMG()
{
}

IMG::IMG(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

IMG::IMG(const IMG& copyIMG)
{
}

IMG::~IMG()
{
}

void IMG::setPVStructs()
{
}

void IMG::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void IMG::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void IMG::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void IMG::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}
