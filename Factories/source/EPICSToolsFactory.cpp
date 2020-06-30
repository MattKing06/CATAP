#include <EPICSToolsFactory.h>

EPICSToolsFactory::EPICSToolsFactory()
{
}

EPICSToolsFactory::EPICSToolsFactory(STATE mode) :
	mode(mode)
{
}

EPICSToolsFactory::EPICSToolsFactory(const EPICSToolsFactory& copyFactory)
{
}

EPICSToolsFactory::~EPICSToolsFactory()
{
}

void EPICSToolsFactory::monitor(std::string pv)
{
	if (GlobalFunctions::entryExists(generalMonitorMap, pv))
	{
		generalMonitorMap[pv].monitor();
	}
	else
	{
		generalMonitorMap[pv] = EPICSTools(mode, pv);
		generalMonitorMap[pv].monitor();
	}
}

void EPICSToolsFactory::monitor(const std::vector<std::string>& pvListToMonitor)
{
	// should we pass the pv's straight through to EPICSTools object??
	for (auto& pv : pvListToMonitor)
	{
		monitor(pv);
	}
}

void EPICSToolsFactory::monitor_Py(const boost::python::list& pvListToMonitor)
{
	std::vector<std::string> pvVec = to_std_vector<std::string>(pvListToMonitor);
	monitor(pvVec);
}

void EPICSToolsFactory::get(const std::string& pv)
{
}

void EPICSToolsFactory::get(const std::vector<std::string>& pvListToGet)
{
}

void EPICSToolsFactory::get(const boost::python::list& pvListToGet)
{
}

void EPICSToolsFactory::put(const boost::python::dict pvValueMap)
{
}

void EPICSToolsFactory::info(const std::string& pv)
{
}

void EPICSToolsFactory::info(const std::vector<std::string> pvList)
{
}

void EPICSToolsFactory::info(const boost::python::list pvList)
{
}

void EPICSToolsFactory::debugMessagesOn() 
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("General Monitor Factory - DEBUG ON");
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.debugMessagesOn();
	}
}
void EPICSToolsFactory::debugMessagesOff() 
{
	messenger.printDebugMessage("General Monitor Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.debugMessagesOff();
	}
}
void EPICSToolsFactory::messagesOn() 
{
	messenger.messagesOn();
	messenger.printMessage("General Monitor Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.messagesOn();
	}
}
void EPICSToolsFactory::messagesOff()
{
	messenger.printMessage("General Monitor Factory - MESSAGES OFF");
	messenger.messagesOff();
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.messagesOff();
	}
}

bool EPICSToolsFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool EPICSToolsFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}