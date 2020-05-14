#include <GeneralMonitorFactory.h>

GeneralMonitorFactory::GeneralMonitorFactory()
{
}

GeneralMonitorFactory::GeneralMonitorFactory(STATE mode)
{
}

GeneralMonitorFactory::GeneralMonitorFactory(const GeneralMonitorFactory& copyFactory)
{
}

GeneralMonitorFactory::~GeneralMonitorFactory()
{
}

void GeneralMonitorFactory::setup(std::vector<std::string> pvListToMonitor)
{
}

void GeneralMonitorFactory::debugMessagesOn() 
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("General Monitor Factory - DEBUG ON");
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.debugMessagesOn();
	}
}
void GeneralMonitorFactory::debugMessagesOff() 
{
	messenger.printDebugMessage("General Monitor Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.debugMessagesOff();
	}
}
void GeneralMonitorFactory::messagesOn() 
{
	messenger.messagesOn();
	messenger.printMessage("General Monitor Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.messagesOn();
	}
}
void GeneralMonitorFactory::messagesOff()
{
	messenger.printMessage("General Monitor Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto generalMonitor : generalMonitorMap)
	{
		generalMonitor.second.messagesOff();
	}
}

bool GeneralMonitorFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool GeneralMonitorFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}