#include <EPICSTools.h>

EPICSTools::EPICSTools() :
EPICSTools(STATE::VIRTUAL)
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
{}

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
		listenerMap[pv] = Listener(pv, mode);
		return listenerMap[pv].getBuffer_Py();
	}
}

boost::python::list EPICSTools::getArrayBuffer_Py(const std::string& pv)
{
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		return listenerMap[pv].getArrayBuffer_Py();
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

double EPICSTools::getBufferStdDeviation(const std::string& pv)
{
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		return listenerMap[pv].getBufferStdDeviation();
	}
	else
	{
		listenerMap[pv] = Listener(pv);
		return listenerMap[pv].getBufferStdDeviation();
	}
}

boost::python::dict EPICSTools::getBufferStdDeviation_Py(boost::python::list pvList)
{
	std::vector<std::string> namesVec = to_std_vector<std::string>(pvList);
	std::map<std::string, double> pvStdDeviationMap;
	for (auto& pv : namesVec)
	{
		if (GlobalFunctions::entryExists(listenerMap, pv))
		{
			pvStdDeviationMap[pv] = getBufferStdDeviation(pv);
		}
		else
		{
			pvStdDeviationMap[pv] = std::numeric_limits<double>::min();
		}
	}
	return to_py_dict(pvStdDeviationMap);
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

void EPICSTools::stopMonitoring(const std::string& pv)
{
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		listenerMap[pv].stopListening();
		std::cout << "removed monitor for " << pv << std::endl;
	}
	else
	{
		std::cout << "no monitor set up for " << pv << std::endl;
	}
}

void EPICSTools::restartMonitoring(const std::string& pv)
{
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		listenerMap[pv].startListening();
		if (ca_state(listenerMap[pv].pv.CHID) == cs_conn)
		{
			listenerMap[pv].epicsInterface->createSubscription(listenerMap[pv], listenerMap[pv].pv);
			EPICSInterface::sendToEPICS();
		}
		std::cout << "restarted monitor for " << pv << std::endl;
	}
	else
	{
		std::cout << "no monitor set up for " << pv << std::endl;
	}
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

void EPICSTools::putArray_Py(const std::string& pv, boost::python::list py_Array)
{
	if (GlobalFunctions::entryExists(putterMap, pv))
	{
		putterMap[pv].putArray_Py(py_Array);
	}
	else
	{
		putterMap[pv] = Putter(pv);
		EPICSInterface::sendToEPICS();
		return putterMap[pv].putArray_Py(py_Array);
	}
}

int EPICSTools::getCount(const std::string& pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		return int(getterMap[pv].pv.COUNT);
	}
	else if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		return int(listenerMap[pv].pv.COUNT);
	}
	else if (GlobalFunctions::entryExists(putterMap, pv))
	{
		return int(putterMap[pv].pv.COUNT);
	}
}

std::string EPICSTools::getType(const std::string& pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		switch (getterMap[pv].pv.CHTYPE)
		{
			case(DBR_DOUBLE):
				return std::string("DBR_DOUBLE");
				break;
			case(DBR_INT):
				return std::string("DBR_INT");
				break;
			case(DBR_FLOAT):
				return std::string("DBR_FLOAT");
				break;
			case(DBR_LONG):
				return std::string("DBR_LONG");
				break;
			case(DBR_ENUM):
				return std::string("DBR_ENUM");
				break;
			case(DBR_CHAR):
				return std::string("DBR_CHAR");
				break;
			case(DBR_STRING):
				return std::string("DBR_STRING");
				break;
			default:
				return std::string("UNKNOWN TYPE");
				break;
		}
	}
	else if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		switch (listenerMap[pv].pv.CHTYPE)
		{
		case(DBR_DOUBLE):
			return std::string("DBR_DOUBLE");
			break;
		case(DBR_INT):
			return std::string("DBR_INT");
			break;
		case(DBR_FLOAT):
			return std::string("DBR_FLOAT");
			break;
		case(DBR_LONG):
			return std::string("DBR_LONG");
			break;
		case(DBR_ENUM):
			return std::string("DBR_ENUM");
			break;
		case(DBR_CHAR):
			return std::string("DBR_CHAR");
			break;
		case(DBR_STRING):
			return std::string("DBR_STRING");
			break;
		default:
			return std::string("UNKNOWN TYPE");
			break;
		}
	}
	else if (GlobalFunctions::entryExists(putterMap, pv))
	{
		switch (putterMap[pv].pv.CHTYPE)
		{
		case(DBR_DOUBLE):
			return std::string("DBR_DOUBLE");
			break;
		case(DBR_INT):
			return std::string("DBR_INT");
			break;
		case(DBR_FLOAT):
			return std::string("DBR_FLOAT");
			break;
		case(DBR_LONG):
			return std::string("DBR_LONG");
			break;
		case(DBR_ENUM):
			return std::string("DBR_ENUM");
			break;
		case(DBR_CHAR):
			return std::string("DBR_CHAR");
			break;
		case(DBR_STRING):
			return std::string("DBR_STRING");
			break;
		default:
			return std::string("UNKNOWN TYPE");
			break;
		}
	}
}

std::string EPICSTools::getMonitorType(const std::string& pv)
{
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		switch (listenerMap[pv].pv.monitorCHTYPE)
		{
		case(DBR_TIME_DOUBLE):
			return std::string("DBR_TIME_DOUBLE");
			break;
		case(DBR_TIME_INT):
			return std::string("DBR_TIME_INT");
			break;
		case(DBR_TIME_FLOAT):
			return std::string("DBR_TIME_FLOAT");
			break;
		case(DBR_TIME_LONG):
			return std::string("DBR_TIME_LONG");
			break;
		case(DBR_TIME_ENUM):
			return std::string("DBR_TIME_ENUM");
			break;
		case(DBR_TIME_CHAR):
			return std::string("DBR_TIME_CHAR");
			break;
		case(DBR_TIME_STRING):
			return std::string("DBR_TIME_STRING");
			break;
		default:
			return std::string("UNKNOWN TYPE");
			break;
		}
	}
}

boost::python::dict EPICSTools::getEPICSInfo(const std::string& pv)
{
	boost::python::dict returnDict;
	returnDict["NAME"] = pv;
	returnDict["COUNT"] = getCount(pv);
	if (GlobalFunctions::entryExists(listenerMap, pv))
	{
		returnDict["MONITOR_TYPE"] = getMonitorType(pv);
	}
	else
	{
		returnDict["TYPE"] = getType(pv);
	}
	return returnDict;
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

boost::python::list EPICSTools::getArray_Py(const std::string& pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		return getterMap[pv].getArray_Py();
	}
	else
	{
		std::cout << "creating getter for: " << pv << std::endl;
		getterMap[pv] = Getter(pv);
		std::cout << "created getter" << std::endl;
		return getterMap[pv].getArray_Py();
	}
}
