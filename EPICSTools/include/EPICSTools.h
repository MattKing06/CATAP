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
	void get(const std::string& pv);
	void get(std::vector<std::string> pvList);
	void get_Py(boost::python::list pvList);
	boost::python::object caget_Py(const std::string& pv);
	template <typename T>
	T caget(const std::string& pv);
	STATE mode;
	std::map<std::string, Listener> listenerMap;
	std::map<std::string, Getter> getterMap;
	std::map<std::string, Putter> putterMap;
};

template<typename T>
inline T EPICSTools::caget(const std::string& pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		return boost::get<T>(getterMap.at(pv).value);
	}
	else
	{
		get(pv);
		return boost::get<T>(getterMap.at(pv).value);
	}
}

#endif //EPICS_TOOLS_H