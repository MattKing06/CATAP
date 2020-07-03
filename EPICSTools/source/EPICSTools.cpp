#include <EPICSTools.h>

EPICSTools::EPICSTools() :
	listenerMap(std::map<std::string,Listener>()),
	getterMap(std::map<std::string, Getter>()),
	putterMap(std::map<std::string, Putter>())
{
	//setup the ca_client_context to actually connect
	// to EPICS server.., may need to be passed through
	// to Listeners, Getters, and Putters.
}

EPICSTools::EPICSTools(STATE mode) :
	mode(mode),
	listenerMap(std::map<std::string, Listener>()),
	getterMap(std::map<std::string, Getter>()),
	putterMap(std::map<std::string, Putter>())
{

}

EPICSTools::EPICSTools(const EPICSTools& copyEPICSTools)
{
	listenerMap.insert(copyEPICSTools.listenerMap.begin(), copyEPICSTools.listenerMap.end());
	getterMap.insert(copyEPICSTools.getterMap.begin(), copyEPICSTools.getterMap.end());
	putterMap.insert(copyEPICSTools.putterMap.begin(), copyEPICSTools.putterMap.end());
}

Listener& EPICSTools::getMonitor(std::string name)
{
	if (GlobalFunctions::entryExists(listenerMap, name))
	{
		return listenerMap[name];
	}
	else
	{
		std::cout << "NO MONITOR SETUP FOR " << name << std::endl;
	}
}

void EPICSTools::monitor(std::string pv)
{
	listenerMap[pv] = Listener(pv);
	listenerMap[pv].epicsInterface->createSubscription(listenerMap[pv], listenerMap[pv].pv);
	EPICSInterface::sendToEPICS();


}

void EPICSTools::monitor(std::vector<std::string> pvList)
{
	for (auto& pv : pvList)
	{
		monitor(pv);
	}
}

void EPICSTools::monitor_Py(boost::python::list pvList)
{
	std::vector<std::string> pvVec = to_std_vector<std::string>(pvList);
	monitor(pvVec);
}

