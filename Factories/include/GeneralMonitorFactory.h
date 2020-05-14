#ifndef GENERAL_MONITOR_FACTORY_H_
#define GENERAL_MONITOR_FACTORY_H_

#include <ConfigReader.h>
#include <GeneralMonitor.h>
#include <GlobalStateEnums.h>
#include <LoggingSystem.h>
#include <vector>
class GeneralMonitorFactory
{
public:
	GeneralMonitorFactory();
	GeneralMonitorFactory(STATE mode);
	GeneralMonitorFactory(const GeneralMonitorFactory& copyFactory);
	~GeneralMonitorFactory();
	LoggingSystem messenger;
	ConfigReader reader;
	void setup(std::vector<std::string> pvListToMonitor);
	bool hasBeenSetup;
	std::map<std::string, GeneralMonitor> generalMonitorMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // GENERAL_MONITOR_FACTORY_H_