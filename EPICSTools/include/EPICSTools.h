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
	std::map<std::string, Listener> getMonitors(std::vector<std::string> names);
	std::vector<std::string> getAllMonitorNames();
	boost::python::list getAllMonitorNames_Py();
	template <typename T>
	T get(const std::string& pv);
	template <typename T>
	void put(const std::string& pv, T value);
	boost::python::object get_Py(const std::string& pv);
	boost::python::dict get_Py(boost::python::list pvList);
	STATE mode;
	std::map<std::string, Listener> listenerMap;
	std::map<std::string, Getter> getterMap;
	std::map<std::string, Putter> putterMap;
};



template<typename T>
inline T EPICSTools::get(const std::string& pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		return getterMap[pv].getValue<T>();
	}
	else
	{
		getterMap[pv] = Getter(pv);
		return getterMap[pv].getValue<T>();
	}
}

template<typename T>
inline void EPICSTools::put(const std::string& pv, T value)
{
	if (GlobalFunctions::entryExists(putterMap, pv))
	{
		putterMap[pv].put<T>(value);
	}
	else
	{
		putterMap[pv] = Putter(pv);
		putterMap[pv].put<T>(value);
	}
}

#endif //EPICS_TOOLS_H
