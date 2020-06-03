#include <RFModulator.h>

RFModulator::RFModulator()
{
}

RFModulator::RFModulator(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

RFModulator::RFModulator(const RFModulator& copyRFModulator)
{
}

RFModulator::~RFModulator()
{
}

void RFModulator::setPVStructs()
{
}

void RFModulator::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void RFModulator::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void RFModulator::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void RFModulator::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}