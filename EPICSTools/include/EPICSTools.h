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
	STATE mode;
	std::map<std::string, Listener> listenerMap;
	std::map<std::string, Getter> getterMap;
	std::map<std::string, Putter> putterMap;
};


#endif //EPICS_TOOLS_H