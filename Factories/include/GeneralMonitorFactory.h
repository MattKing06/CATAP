#ifndef GENERAL_MONITOR_FACTORY_H_
#define GENERAL_MONITOR_FACTORY_H_

#include <ConfigReader.h>
#include <GeneralMonitor.h>
#include <GlobalStateEnums.h>
#include <vector>
class GeneralMonitorFactory
{
public:
	GeneralMonitorFactory();
	GeneralMonitorFactory(STATE mode);
	GeneralMonitorFactory(const GeneralMonitorFactory& copyFactory);
	~GeneralMonitorFactory();
	void setup(std::vector<std::string> pvListToMonitor);
	bool hasBeenSetup;
	std::map<std::string, GeneralMonitor> generalMonitorMap;
};

#endif // GENERAL_MONITOR_FACTORY_H_