#include <LLRF.h>

LLRF::LLRF()
{
}

LLRF::LLRF(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
// calls copy constructor and destroys 
epicsInterface(boost::make_shared<EPICSLLRFInterface>(EPICSLLRFInterface()))
machineArea(boost::make_shared<EPICSLLRFInterface>(EPICSLLRFInterface()))
{
}

LLRF::LLRF(const LLRF& copyLLRF)
{
}

LLRF::~LLRF()
{
}

void LLRF::setPVStructs()
{
}

void LLRF::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LLRF::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void LLRF::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void LLRF::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}
