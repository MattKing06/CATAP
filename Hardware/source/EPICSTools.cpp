#include <EPICSTools.h>
#include <vector>

EPICSTools::EPICSTools() : Hardware()
{
}

EPICSTools::EPICSTools(STATE mode, std::string pv) : Hardware(mode)
{
	pvToMonitor = pv;
	epicsInterface = boost::make_shared<EPICSToolsInterface>();
	setPVStruct(pvToMonitor);
}

EPICSTools::EPICSTools(const EPICSTools& copyEPICSTools) :
	Hardware(copyEPICSTools),
	epicsInterface(copyEPICSTools.epicsInterface)
{
}

boost::any EPICSTools::getValue()
{
	return value;
}

void EPICSTools::setPVStruct(const std::string& pv)
{
	//look at magnet SetPVStructs and see what to do.
	pvStructs[pv] = pvStruct();
	if (mode == STATE::VIRTUAL)
	{
		pvStructs[pv].fullPVName = "VM-" + pv;
	}
	else
	{
		pvStructs[pv].fullPVName = pv;
	}

	// find the last colon and take the string after it as the pvRecord
	size_t recordPosition = pv.find_last_of(":");
	pvStructs[pv].pvRecord = pv.substr(recordPosition+1);
	std::cout << pvStructs[pv] << std::endl;
	epicsInterface->retrieveCHID(pvStructs[pv]);
	if (ca_state(pvStructs[pv].CHID) == cs_conn)
	{
		epicsInterface->retrieveCHTYPE(pvStructs[pv]);
		epicsInterface->retrieveCOUNT(pvStructs[pv]);
		EPICSInterface::sendToEPICS();
	}
	else
	{
		messenger.printMessage(pvToMonitor, " COULD NOT CONNECT TO EPICS.");
	}
}

EPICSTools::~EPICSTools()
{
}

void EPICSTools::monitor()
{
	epicsInterface->retrieveUpdateFunction(pvStructs[pvToMonitor]);
	EPICSInterface::sendToEPICS();
	epicsInterface->createSubscription(*(this), pvStructs[pvToMonitor]);
	EPICSInterface::sendToEPICS();

}

void EPICSTools::setValue(boost::any valueToSet)
{
	value = valueToSet;
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
