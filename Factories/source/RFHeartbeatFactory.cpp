#include <RFHeartbeatFactory.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>

RFHeartbeatFactory::RFHeartbeatFactory()
{
}

RFHeartbeatFactory::RFHeartbeatFactory(STATE mode) :
messenger(LoggingSystem(true, true)),
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("RFHeartbeat", mode))
{
}

RFHeartbeatFactory::RFHeartbeatFactory(const RFHeartbeatFactory& copyFactory)
{
}

RFHeartbeatFactory::~RFHeartbeatFactory()
{
}

bool RFHeartbeatFactory::setup(std::string version)
{
	if (hasBeenSetup)
	{
		messenger.printMessage("RF Heartbeat Factory has already been setup");
		return hasBeenSetup;
	}
	messenger.printMessage("setup populateRFHeartbeatMap");
	populateRFHeartbeatMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		// Could not find any YAML files for configuration..
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	messenger.printMessage("call setupChannels");
	setupChannels();
	messenger.printMessage("sendToEPICS");
	EPICSInterface::sendToEPICS();
	messenger.printMessage("setup connections");
	for (auto& hb : RFHeartbeatMap)
	{
		updateAliasNameMap(hb.second);
		std::map<std::string, pvStruct>& pvStructs = hb.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printDebugMessage("Setup ", pv.first);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				setMonitorStatus(pv.second);
				hb.second.epicsInterface->retrieveCOUNT(pv.second);
				hb.second.epicsInterface->retrieveCHTYPE(pv.second);
				hb.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.first, " ", pv.second.pvRecord, " r, w, s = ", std::to_string(ca_read_access(pv.second.CHID)),
					std::to_string(ca_write_access(pv.second.CHID)), std::to_string(ca_state(pv.second.CHID)));

				if (pv.second.monitor)
				{
					hb.second.epicsInterface->createSubscription(hb.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(pv.second.fullPVName, " COULD NOT CONNECT TO EPICS.");
			}
		}
		EPICSInterface::sendToEPICS();
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}
void RFHeartbeatFactory::populateRFHeartbeatMap()
{
	messenger.printDebugMessage("RFHeartbeatFactory is populating the rfHeartbeatMap map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("RFModulatorFactory calling parseNextYamlFile");
		reader.parseNextYamlFile<RFHeartbeat>(RFHeartbeatMap);
	}
	//size_t start_size = rfHeartbeatMap.size();
}
void RFHeartbeatFactory::setMonitorStatus(pvStruct& pvStruct)
{	// only 1 PVs are monitored
	if (pvStruct.pvRecord == RFHeartbeatRecords::KEEP_ALIVE)
	{
		pvStruct.monitor = true;
	}
}
void RFHeartbeatFactory::updateAliasNameMap(const RFHeartbeat& rf_hb)
{
	// first add in the full name
	std::string full_name = rf_hb.getHardwareName();
	//messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		//messenger.printMessage("!!ERROR!! ", full_name, " name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases
	std::vector<std::string> aliases = rf_hb.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", rf_hb.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}
void RFHeartbeatFactory::setupChannels()
{
	for (auto& device : RFHeartbeatMap)
	{
		std::map<std::string, pvStruct>& pvStructs = device.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printMessage("retrieveCHID ", pv.first, " = [", pv.second.fullPVName,"]");
			device.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}


RFHeartbeat& RFHeartbeatFactory::getRFHeartbeat(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFHeartbeatMap, full_name))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return RFHeartbeatMap.at(full_name);
	}
	return dummy_rfHeartbeat;
}

std::vector<std::string> RFHeartbeatFactory::getHeartbeatNames()const
{
	std::vector<std::string> r;
	for (auto&& it : RFHeartbeatMap)
	{
		r.push_back(it.first);
	}
	return r;
}
boost::python::list RFHeartbeatFactory::getHeartbeatNames_Py()const
{
	return to_py_list<std::string>(getHeartbeatNames());
}
std::string RFHeartbeatFactory::getFullName(const std::string& name_to_check) const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return dummy_rfHeartbeat.getHardwareName();
}



double RFHeartbeatFactory::getValue(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFHeartbeatMap, full_name))
	{
		return RFHeartbeatMap.at(full_name).getValue();
	}
}
void RFHeartbeatFactory::setValue(const std::string& name, double v)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFHeartbeatMap, full_name))
	{
		return RFHeartbeatMap.at(full_name).setValue(v);
	}
}
void RFHeartbeatFactory::pulse(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFHeartbeatMap, full_name))
	{
		return RFHeartbeatMap.at(full_name).pulse();
	}
}


void RFHeartbeatFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("RFHeartbeat Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.debugMessagesOn();
	}
}
void RFHeartbeatFactory::debugMessagesOff()
{
	messenger.printDebugMessage("RFHeartbeat Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.debugMessagesOff();
	}
}
void RFHeartbeatFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("RFHeartbeat Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.messagesOn();
	}
}
void RFHeartbeatFactory::messagesOff()
{
	messenger.printMessage("RFHeartbeat Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.messagesOff();
	}
}
bool RFHeartbeatFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool RFHeartbeatFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}