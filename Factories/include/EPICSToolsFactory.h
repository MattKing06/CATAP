#ifndef GENERAL_MONITOR_FACTORY_H_
#define GENERAL_MONITOR_FACTORY_H_

#include <ConfigReader.h>
#include <EPICSTools.h>
#include <GlobalStateEnums.h>
#include <GlobalFunctions.h>
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
	void monitor(std::string pv);
	void monitor(const std::vector<std::string>& pvListToMonitor);
	void monitor_Py(const boost::python::list& pvListToMonitor);
	void get(const std::string& pv);
	void get(const std::vector<std::string>& pvListToGet);
	void get(const boost::python::list& pvListToGet);
	template<typename valueType>
	void put(const std::string& pv, valueType value);
	template<typename valueType>
	void put(const std::map<std::string, valueType> pvValueMap);
	void put(const boost::python::dict pvValueMap);
	void info(const std::string& pv);
	void info(const std::vector<std::string> pvList);
	void info(const boost::python::list pvList);
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

template<typename valueType>
inline void EPICSToolsFactory::put(const std::string& pv, valueType value)
{
}

template<typename valueType>
inline void EPICSToolsFactory::put(const std::map<std::string, valueType> pvValueMap)
{
}
