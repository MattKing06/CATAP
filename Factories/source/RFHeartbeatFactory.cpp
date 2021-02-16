#include <RFHeartbeatFactory.h>
#include <GlobalFunctions.h>

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
	messenger.printMessage("setup setupChannels");
	setupChannels();
	EPICSInterface::sendToEPICS();
	messenger.printMessage("setup connections");
	for (auto& prot : RFHeartbeatMap)
	{
		updateAliasNameMap(prot.second);
		std::map<std::string, pvStruct>& pvStructs = prot.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printDebugMessage("Setup ", pv.first);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				setMonitorStatus(pv.second);
				prot.second.epicsInterface->retrieveCOUNT(pv.second);
				prot.second.epicsInterface->retrieveCHTYPE(pv.second);
				prot.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.first, " ", pv.second.pvRecord, " r, w, s = ", std::to_string(ca_read_access(pv.second.CHID)),
					std::to_string(ca_write_access(pv.second.CHID)), std::to_string(ca_state(pv.second.CHID)));

				if (pv.second.monitor)
				{
					prot.second.epicsInterface->createSubscription(prot.second, pv.second);
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
void RFHeartbeatFactory::retrieveUpdateFunctionForRecord()
{

}
void RFHeartbeatFactory::setMonitorStatus(pvStruct& pvStruct)
{

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
			messenger.printMessage(device.first, ", retrieveCHID ", pv.first);
			device.second.epicsInterface->retrieveCHID(pv.second);
		}
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