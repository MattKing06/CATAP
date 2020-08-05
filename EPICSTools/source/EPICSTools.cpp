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

std::map<std::string, Listener> EPICSTools::getMonitors(std::vector<std::string> names)
{
	std::map<std::string, Listener> listenerMapToReturn;
	for (auto& name : names)
	{
		listenerMapToReturn[name] = getMonitor(name);
	}
	return listenerMapToReturn;
}

std::vector<std::string> EPICSTools::getAllMonitorNames()
{
	std::vector<std::string> monitorNames;
	for (auto& listener : listenerMap)
	{
		monitorNames.push_back(listener.first);
	}
	return monitorNames;
}

boost::python::list EPICSTools::getAllMonitorNames_Py()
{
	boost::python::list listenerNames = to_py_list(getAllMonitorNames());
	return listenerNames;
}

boost::python::list EPICSTools::getBuffer_Py(const std::string& pv)
{
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		return listenerMap[pv].getBuffer_Py();
	}
	else
	{
		listenerMap[pv] = Listener(pv);
		return listenerMap[pv].getBuffer_Py();
	}
}

boost::python::dict EPICSTools::getBuffer_Py(boost::python::list pvList)
{
	std::vector<std::string> namesVec = to_std_vector<std::string>(pvList);
	std::map<std::string, boost::python::list> pvBufferMap;
	for (auto& pv : namesVec)
	{
		pvBufferMap[pv] = getBuffer_Py(pv);
	}
	return to_py_dict(pvBufferMap);
}

double EPICSTools::getBufferAverage_Py(const std::string& pv)
{
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		return listenerMap[pv].getBufferAverage();
	}
	else
	{
		listenerMap[pv] = Listener(pv);
		return listenerMap[pv].getBufferAverage();
	}
}

boost::python::dict EPICSTools::getBufferAverage_Py(boost::python::list pvList)
{
	std::vector<std::string> namesVec = to_std_vector<std::string>(pvList);
	std::map<std::string, double> pvBufferAverageMap;
	for (auto& pv : namesVec)
	{
		pvBufferAverageMap[pv] = getBufferAverage_Py(pv);
	}
	return to_py_dict(pvBufferAverageMap);
}

void EPICSTools::monitor(const std::string& pv)
{
	listenerMap[pv] = Listener(pv);
	if (ca_state(listenerMap[pv].pv.CHID) == cs_conn)
	{
		listenerMap[pv].epicsInterface->createSubscription(listenerMap[pv], listenerMap[pv].pv);
		EPICSInterface::sendToEPICS();
	}
	else
	{
		std::cout << pv << " COULD NOT CONNECT TO EPICS " << std::endl;
	}
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



boost::python::dict EPICSTools::get_Py(boost::python::list pvList)
{
	std::vector<std::string> pvVec = to_std_vector<std::string>(pvList);
	std::map<std::string, boost::python::object> nameAndValueMap;
	for (auto pv : pvVec)
	{
		if (GlobalFunctions::entryExists(getterMap, pv))
		{
			nameAndValueMap[pv] = getterMap[pv].getValue_Py();
		}
		else
		{
			getterMap[pv] = Getter(pv);
			EPICSInterface::sendToEPICS();
			nameAndValueMap[pv] = getterMap[pv].getValue_Py();
		}
	}
	return to_py_dict(nameAndValueMap);
}

void EPICSTools::put_Py(boost::python::dict pvAndValueDict)
{
	std::map<std::string, boost::python::object> pvAndValueMap = to_std_map<std::string, boost::python::object>(pvAndValueDict);
	for (auto& entry : pvAndValueMap)
	{
		if (GlobalFunctions::entryExists(putterMap, entry.first))
		{
			putterMap[entry.first].put_Py(entry.second);
		}
		else
		{
			putterMap[entry.first] = Putter(entry.first);
			putterMap[entry.first].put_Py(entry.second);
		}
	}

}

void EPICSTools::put_Py(const std::string& pv, boost::python::object value)
{
	if (GlobalFunctions::entryExists(putterMap, pv))
	{
		putterMap[pv].put_Py(value);
	}
	else
	{
		putterMap[pv] = Putter(pv);
		EPICSInterface::sendToEPICS();
		return putterMap[pv].put_Py(value);
	}

}

boost::python::object EPICSTools::get_Py(const std::string& pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		return getterMap[pv].getValue_Py();
	}
	else
	{
		getterMap[pv] = Getter(pv);
		EPICSInterface::sendToEPICS();
		return getterMap[pv].getValue_Py();
	}

}