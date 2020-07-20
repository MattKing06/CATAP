#ifndef EPICS_TOOLS_H
#define EPICS_TOOLS_H

#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>
#include <Listener.h>
#include <Getter.h>
#include <Putter.h>



class EPICSTools
{
public:
	EPICSTools();
	EPICSTools(STATE mode);
	EPICSTools(const EPICSTools& copyEPICSTools);
	void monitor(const std::string& pv);
	void monitor(std::vector<std::string> pvList);
	void monitor_Py(boost::python::list pvList);
	Listener& getMonitor(std::string pv);
	std::map<std::string, Listener&> getMonitors(std::vector<std::string> names);
	std::vector<std::string> getAllMonitorNames();
	boost::python::list getAllMonitorNames_Py();
	template <typename T>
	T get(const std::string& pv);
	template <typename T>
	std::map<std::string, T> get(const std::vector<std::string>& pvList);
	boost::python::object get_Py(const std::string& pv);
	boost::python::dict get_Py(boost::python::list pvList);
	STATE mode;
	std::map<std::string, Listener> listenerMap;
	std::map<std::string, Getter> getterMap;
	std::map<std::string, Putter> putterMap;
};

template<typename T>
inline T EPICSTools::get(const std::string & pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		return getterMap[pv].getValue<T>();
	}
	else
	{
		getterMap[pv] = Getter(pv);
		EPICSInterface::sendToEPICS();
		std::cout << "GETTER VALUE: " << getterMap[pv].value << std::endl;
		return getterMap[pv].getValue<T>();
	}

	
}

template<typename T>
inline std::map<std::string, T> EPICSTools::get(const std::vector<std::string>& pvList)
{
	std::map<std::string, T> nameAndValueMap();
	for (auto& pv : pvList)
	{
		nameAndValueMap[pv] = get(pv);
	}
	return nameAndValueMap;
}


#endif //EPICS_TOOLS_H