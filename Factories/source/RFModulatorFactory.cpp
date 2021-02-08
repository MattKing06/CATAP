#include <RFModulatorFactory.h>
#include <RFModulatorPVRecords.h>
#include <GlobalFunctions.h>

RFModulatorFactory::RFModulatorFactory()
{
}

RFModulatorFactory::RFModulatorFactory(STATE mode):
	messenger(LoggingSystem(true, true)),
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("RFModulator", mode))
{
}

RFModulatorFactory::RFModulatorFactory(const RFModulatorFactory& copyFactory)
{
}

RFModulatorFactory::~RFModulatorFactory()
{
}

void RFModulatorFactory::populateRFModulatorMap()
{
	messenger.printDebugMessage("RFModulatorFactory is populating the RFModulator map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("RFModulatorFactory calling parseNextYamlFile");
		reader.parseNextYamlFile<RFModulator>(RFModulatorMap);
	}
}

std::vector<std::string> RFModulatorFactory::getAllRFModulatorNames()
{
	std::vector<std::string> rfProtNames;
	for (auto& rfProt : RFModulatorMap)
	{
		rfProtNames.push_back(rfProt.first);
	}
	return rfProtNames;
}

bool RFModulatorFactory::setup(std::string version)
{
	if (hasBeenSetup)
	{
		messenger.printMessage("RF Modulator Factory has already been setup");
		return true;
	}
	messenger.printMessage("setup populateRFModulatorMap");
	populateRFModulatorMap();
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
	for (auto& prot : RFModulatorMap)
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
void RFModulatorFactory::updateAliasNameMap(const RFModulator& mod)
{
	// first add in the magnet full name
	std::string full_name = mod.getHardwareName();
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
	std::vector<std::string> aliases = mod.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", mod.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}
void RFModulatorFactory::setupChannels()
{
	for (auto& device : RFModulatorMap)
	{
		std::map<std::string, pvStruct>& pvStructs = device.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printMessage(device.first, ", retrieveCHID ", pv.first);
			device.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}
void RFModulatorFactory::setMonitorStatus(pvStruct& pvStruct)
{
	messenger.printMessage("setMonitorStatus checking ", pvStruct.pvRecord);
	if (std::find(RFModulatorRecords::rf_mod_monitor_records_list.begin(),
		RFModulatorRecords::rf_mod_records_list.end(), pvStruct.pvRecord) != RFModulatorRecords::rf_mod_records_list.end())
	{
		pvStruct.monitor = true;
		messenger.printMessage("setMonitorStatus ", pvStruct.pvRecord, ", status = true");
	}
	else
	{
		pvStruct.monitor = false;
		messenger.printMessage("setMonitorStatus ", pvStruct.pvRecord, ", status = false ");
	}
}


void RFModulatorFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("RF Modulator Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.debugMessagesOn();
	}
}
void RFModulatorFactory::debugMessagesOff()
{
	messenger.printDebugMessage("RF Modulator Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.debugMessagesOff();
	}
}
void RFModulatorFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("RF Modulator Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.messagesOn();
	}
}
void RFModulatorFactory::messagesOff()
{
	messenger.printMessage("RF Modulator Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.messagesOff();
	}
}
bool RFModulatorFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool RFModulatorFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
