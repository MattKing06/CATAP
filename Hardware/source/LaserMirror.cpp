#include <LaserMirror.h>

LaserMirror::LaserMirror()
{
}

LaserMirror::LaserMirror(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

LaserMirror::LaserMirror(const LaserMirror& copyLaserMirror)
{
}

LaserMirror::~LaserMirror()
{
}

void LaserMirror::setPVStructs()
{
}

void LaserMirror::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void LaserMirror::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LaserMirror::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGING ON");
	epicsInterface->messagesOn();
}

void LaserMirror::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGING OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();

}
