#include <Lighting.h>

Lighting::Lighting()
{
}

Lighting::Lighting(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

Lighting::Lighting(const Lighting& copyLighting)
{
}

Lighting::~Lighting()
{
}

void Lighting::setPVStructs()
{
}

void Lighting::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Lighting::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Lighting::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Lighting::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}