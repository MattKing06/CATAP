#include <GeneralMonitor.h>

GeneralMonitor::GeneralMonitor()
{
}

GeneralMonitor::GeneralMonitor(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

GeneralMonitor::GeneralMonitor(const GeneralMonitor& copyGeneralMonitor)
{
}

GeneralMonitor::~GeneralMonitor()
{
}

void GeneralMonitor::setPVStructs()
{
}

void GeneralMonitor::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void GeneralMonitor::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void GeneralMonitor::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void GeneralMonitor::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}
