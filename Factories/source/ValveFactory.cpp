#include <ValveFactory.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>
ValveFactory::ValveFactory() :
	ValveFactory(STATE::OFFLINE)
{
}

ValveFactory::ValveFactory(STATE mode) : 
	mode(mode), hasBeenSetup(false),
	reader(ConfigReader("Valve", mode)),
	messenger(LoggingSystem(true,true))
{
}

ValveFactory::ValveFactory(const ValveFactory& copyValveFactory) :
	hasBeenSetup(copyValveFactory.hasBeenSetup),
	mode(copyValveFactory.mode),
	messenger(copyValveFactory.messenger),
	reader(copyValveFactory.reader),
	dummyValve(copyValveFactory.dummyValve)
{
	messenger.printDebugMessage("ValveFactory copy constructor called");
	valveMap.insert(copyValveFactory.valveMap.begin(), copyValveFactory.valveMap.end());
}

ValveFactory::~ValveFactory()
{
	messenger.printDebugMessage("ValveFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& valve : valveMap)
		{
			auto pvStructsList = valve.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					if (pvStruct.second.EVID)
					{
						valve.second.epicsInterface->removeSubscription(pvStruct.second);
						ca_flush_io();
					}
				}
				if (pvStruct.second.CHID)
				{
					valve.second.epicsInterface->removeChannel(pvStruct.second);
					ca_pend_io(CA_PEND_IO_TIMEOUT);
				}
			}
		}
	}
}

void ValveFactory::populateValveMap()
{
	messenger.printDebugMessage("ValveFactory is populating Valve Map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support.");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Valve Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(valveMap);
	}
	messenger.printDebugMessage("Valve Factory has finished populating Valve Map");
}

void ValveFactory::setupChannels()
{
	for (auto& valve : valveMap)
	{
		std::map<std::string, pvStruct>& pvStructs = valve.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			valve.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

bool ValveFactory::setup(const std::string& VERSION)
{
	messenger.printDebugMessage("called Valve Factory  setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Valve Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics valve interface has been initialized in valve constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateValveMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}


	setupChannels();
	EPICSInterface::sendToEPICS();


	for (auto& valve : valveMap)
	{
		// update aliases for valve in map
		updateAliasNameMap(valve.second);
		std::map<std::string, pvStruct>& valvePVStructs = valve.second.getPVStructs();
		for (auto& pv : valvePVStructs)
		{
			// sets the monitor state in the pvstruict to true or false
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrieveMonitorStatus(pv.second);
				valve.second.epicsInterface->retrieveCHTYPE(pv.second);
				valve.second.epicsInterface->retrieveCOUNT(pv.second);
				valve.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					valve.second.epicsInterface->createSubscription(valve.second, pv.second);
					EPICSInterface::sendToEPICS();
				}
			}
			else
			{
				messenger.printMessage(valve.first, " CANNOT CONNECT TO EPICS");
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

void ValveFactory::retrieveMonitorStatus(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == ValveRecords::Sta)
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
}

Valve& ValveFactory::getValve(const std::string& fullValveName)
{
	std::string fullName = getFullName(fullValveName);
	if (GlobalFunctions::entryExists(valveMap, fullName)) 
	{
		return valveMap.at(fullName);
	}
	return dummyValve;
}

std::string ValveFactory::getFullName(const std::string& nameToCheck) const
{
	if (GlobalFunctions::entryExists(aliasNameMap, nameToCheck))
	{
		return aliasNameMap.at(nameToCheck);
	}
	return dummyValve.getHardwareName();
}

std::vector<std::string> ValveFactory::getAllValveNames() const
{
	std::vector<std::string> returnNames;
	for (auto&& item : valveMap)
	{
		returnNames.push_back(item.first);
	}
	return returnNames;
}

boost::python::list ValveFactory::getAllValveNames_Py() const
{
	return to_py_list(getAllValveNames());
}

STATE ValveFactory::getValveState(const std::string& name) const
{
	std::string fullName = getFullName(name);
	return valveMap.at(fullName).getValveState();
}

std::map<std::string, STATE> ValveFactory::getValveStates(const std::vector<std::string>& names) const
{
	std::map<std::string, STATE> stateMap;
	for (auto& name : names)
	{
		stateMap[name] = getValveState(name);
	}
	
	return stateMap;
}

boost::python::dict ValveFactory::getValveStates_Py(const boost::python::list& names) const
{
	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
	return to_py_dict(getValveStates(namesVector));
}

std::map<std::string, STATE> ValveFactory::getAllValveStates() const
{
	return getValveStates(getAllValveNames());
}

boost::python::dict ValveFactory::getAllValveStates_Py() const
{
	return to_py_dict(getAllValveStates());
}

void ValveFactory::setValveState(const std::string& name, const STATE& state)
{
	std::string fullName = getFullName(name);
	valveMap.at(fullName).setValveState(state);
}

void ValveFactory::close(const std::string& name)
{
	setValveState(name, STATE::CLOSED);
}

void ValveFactory::close(const std::vector<std::string>& names)
{
	for (auto& name : names)
	{
		setValveState(name, STATE::CLOSED);
	}
}

void ValveFactory::close_Py(const boost::python::list& names)
{
	close(to_std_vector<std::string>(names));
}

void ValveFactory::closeAllValves()
{
	close(getAllValveNames());
}

void ValveFactory::open(const std::string& name)
{
	setValveState(name, STATE::OPEN);
}

void ValveFactory::open(const std::vector<std::string>& names)
{
	for (auto& name : names)
	{
		setValveState(name, STATE::OPEN);
	}
}

void ValveFactory::open_Py(const boost::python::list& names)
{
	open(to_std_vector<std::string>(names));
}

void ValveFactory::openAllValves()
{
	open(getAllValveNames());
}

bool ValveFactory::isOpen(std::string name) const
{
	std::string fullName = getFullName(name);
	return valveMap.at(fullName).isOpen();
}

bool ValveFactory::isClosed(std::string name) const
{
	std::string fullName = getFullName(name);
	return valveMap.at(fullName).isClosed();
}

void ValveFactory::updateAliasNameMap(const Valve& valve)
{
	// first add in the valve full name
	std::string fullName = valve.getHardwareName();
	messenger.printMessage("updateAliasNameMap ", fullName);
	if (GlobalFunctions::entryExists(aliasNameMap, fullName))
	{
		messenger.printMessage("!!ERROR!! ", fullName, " valve name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", fullName, " added to alias_name_map");
		aliasNameMap[fullName] = fullName;
	}

	// now add in the aliases
	std::vector<std::string> aliases = valve.getAliases();
	for (auto&& nextAlias : aliases)
	{
		if (GlobalFunctions::entryExists(aliasNameMap, nextAlias))
		{
			messenger.printMessage("!!ERROR!! ", valve.getHardwareName(), " alias = ", nextAlias, " already exists");
		}
		else
		{
			aliasNameMap[nextAlias] = fullName;
			messenger.printMessage("Added alias " + nextAlias + " for " + fullName);
		}
	}
}


void ValveFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("VALV-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& valve : valveMap)
	{
		valve.second.debugMessagesOn();
	}
}
void ValveFactory::debugMessagesOff()
{
	messenger.printDebugMessage("VALV-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& valve : valveMap)
	{
		valve.second.debugMessagesOff();
	}
}
void ValveFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("VALV-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& valve : valveMap)
	{
		valve.second.messagesOn();
	}
}
void ValveFactory::messagesOff()
{
	messenger.printMessage("VALV-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& valve : valveMap)
	{
		valve.second.messagesOff();
	}
}
bool ValveFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool ValveFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}