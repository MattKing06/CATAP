#ifndef GENERAL_MONITOR_FACTORY_H_
#define GENERAL_MONITOR_FACTORY_H_

#include <ConfigReader.h>
#include <EPICSTools.h>
#include <GlobalStateEnums.h>
#include <LoggingSystem.h>
#include <vector>
class EPICSToolsFactory
{
public:
	EPICSToolsFactory();
	EPICSToolsFactory(STATE mode);
	EPICSToolsFactory(const EPICSToolsFactory& copyFactory);
	~EPICSToolsFactory();
	STATE mode;
	LoggingSystem messenger;
	ConfigReader reader;
	void monitor(std::vector<std::string> pvListToMonitor);
	void monitor_Py(boost::python::list pvListToMonitor);
	bool hasBeenSetup;
	std::map<std::string, EPICSTools> generalMonitorMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // GENERAL_MONITOR_FACTORY_H_