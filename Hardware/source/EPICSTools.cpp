#include <EPICSTools.h>
#include <vector>

EPICSTools::EPICSTools() : Hardware()
{
}

EPICSTools::EPICSTools(STATE mode, std::string pv) : Hardware(mode)
{
	pvToMonitor = pv;
	epicsInterface = boost::make_shared<EPICSToolsInterface>();
}

EPICSTools::EPICSTools(const EPICSTools& copyEPICSTools) :
	Hardware(copyEPICSTools),
	epicsInterface(copyEPICSTools.epicsInterface)
{
}

EPICSTools::~EPICSTools()
{
}

void EPICSTools::monitor()
{
	pvStructs[pvToMonitor] = pvStruct();
	if (mode == STATE::VIRTUAL)
	{
		pvStructs[pvToMonitor].fullPVName = "VM-" + pvToMonitor;
	}
	else
	{
		pvStructs[pvToMonitor].fullPVName = pvToMonitor;
	}
	std::cout << pvStructs[pvToMonitor].fullPVName << std::endl;
}

void EPICSTools::setPVStructs()
{
}

void EPICSTools::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	//epicsInterface->debugMessagesOn();
}

void EPICSTools::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	//epicsInterface->debugMessagesOff();
}

void EPICSTools::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	//epicsInterface->messagesOn();
}

void EPICSTools::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	//epicsInterface->messagesOff();
}
