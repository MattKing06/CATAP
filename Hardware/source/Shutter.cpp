#include <Shutter.h>

Shutter::Shutter()
{
}

Shutter::Shutter(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

Shutter::Shutter(const Shutter& copyShutter)
{
}

Shutter::~Shutter()
{
}

void Shutter::setPVStructs()
{
}

void Shutter::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Shutter::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Shutter::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Shutter::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}
