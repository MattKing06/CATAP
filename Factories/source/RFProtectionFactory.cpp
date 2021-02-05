#include <RFProtectionFactory.h>
#include <RFProtectionPVRecords.h>
#include <GlobalFunctions.h>
#include <GlobalFunctions.h>
#include "PythonTypeConversions.h"

RFProtectionFactory::RFProtectionFactory()
{
}

RFProtectionFactory::RFProtectionFactory(STATE mode) :
	messenger(LoggingSystem(true, true)),
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("RFProtection", mode))
{
}

RFProtectionFactory::RFProtectionFactory(const RFProtectionFactory& copyFactory)
{
}

RFProtectionFactory::~RFProtectionFactory()
{
}

void RFProtectionFactory::populateRFProtectionMap()
{
	messenger.printDebugMessage("RF Protection Factory is populating the RFProtection map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("RF Protection Factory calling parseNextYamlFile");
		reader.parseNextYamlFile<RFProtection>(RFProtectionMap);
	}
}

std::vector<std::string> RFProtectionFactory::getAllRFProtectionNames()
{
	std::vector<std::string> rfProtNames;
	for (auto& rfProt : RFProtectionMap)
	{
		rfProtNames.push_back(rfProt.first);
	}
	return rfProtNames;
}


void RFProtectionFactory::setupChannels()
{
	for (auto& nameAndProtectionPair : RFProtectionMap)
	{
		std::map<std::string, pvStruct>& pvStructs = nameAndProtectionPair.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printMessage(nameAndProtectionPair.first, ", retrieveCHID ", pv.first);
			nameAndProtectionPair.second.epicsInterface->retrieveCHID(pv.second);
		}
	}

}

void RFProtectionFactory::setMonitorStatus(pvStruct& pv)
{
	// only 2 PVs are monitored
	if(pv.pvRecord==RFProtectionRecords::Sta 
		|| pv.pvRecord == RFProtectionRecords::Cmi)
	{
		pv.monitor = true;
	}
}

bool RFProtectionFactory::setup(const std::string& version)
{
	if(hasBeenSetup)
	{
		messenger.printMessage("RF Protection Factory has already been setup"); 
		return true;
	}
	messenger.printMessage("setup populateRFProtectionMap");
	populateRFProtectionMap();
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
	for (auto& prot : RFProtectionMap)
	{
		updateNameAliasMap(prot.second);
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
				messenger.printMessage(pv.second.fullPVName , " COULD NOT CONNECT TO EPICS.");
			}
		}
		EPICSInterface::sendToEPICS();
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}


bool RFProtectionFactory::resetGun()
{
	return false;
}
bool RFProtectionFactory::resetL01()
{
	return false;
}

bool RFProtectionFactory::enableGun()
{
	return false;
}

bool RFProtectionFactory::enableL01()
{
	return false;
}

bool RFProtectionFactory::disableGun()
{
	return false;
}
bool RFProtectionFactory::disableL01()
{
	return false;
}


STATE RFProtectionFactory::getStatus(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if(GlobalFunctions::entryExists(RFProtectionMap,full_name))
	{
		return STATE::UNKNOWN_NAME;

	}
	return STATE::UNKNOWN_NAME;
}
std::vector<int> RFProtectionFactory::getKeyBits(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFProtectionMap, full_name))
	{
		return RFProtectionMap.at(full_name).getKeyBits();
	}
	return std::vector<int>{};
}
boost::python::list RFProtectionFactory::getKeyBits_Py(const std::string& name)const
{
	return to_py_list<int>(getKeyBits(name));
}
std::vector<bool> RFProtectionFactory::getKeyBitValues(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(RFProtectionMap, full_name))
	{
		return RFProtectionMap.at(full_name).getKeyBitValues();
	}
	return std::vector<bool>{};
}
boost::python::list RFProtectionFactory::getKeyBitValues_Py(const std::string& name)const
{
	return to_py_list<bool>(getKeyBitValues(name));
}



void RFProtectionFactory::updateNameAliasMap(const RFProtection& prot)
{
	// first add in the magnet full name
	std::string full_name = prot.getHardwareName();
	//messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		// Not necessarily an error, screen_name goes in the alias map too 
		messenger.printMessage("!!ERROR!! ", full_name, " RFProtection name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases
	std::vector<std::string> aliases = prot.getAliases();

	std::ostringstream vts;
	if (!aliases.empty())
	{
		// Convert all but the last element to avoid a trailing "," 
		std::copy(aliases.begin(), aliases.end() - 1,
			std::ostream_iterator<std::string>(vts, ", "));

		// Now add the last element with no delimiter 
		vts << aliases.back();
	}

	messenger.printMessage("Got Aliases: ", vts.str(), " for " + full_name);


	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", prot.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}

}
std::string RFProtectionFactory::getFullName(const std::string& name_to_check) const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return dummy_prot.getHardwareName();
}


void RFProtectionFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("RF Protection Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& rfProtection : RFProtectionMap)
	{
		rfProtection.second.debugMessagesOn();
	}
}
void RFProtectionFactory::debugMessagesOff()
{
	messenger.printDebugMessage("RF Protection Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& rfProtection : RFProtectionMap)
	{
		rfProtection.second.debugMessagesOff();
	}
}
void RFProtectionFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("RF Protection Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& rfProtection : RFProtectionMap)
	{
		rfProtection.second.messagesOn();
	}
}
void RFProtectionFactory::messagesOff()
{
	messenger.printMessage("RF Protection Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& rfProtection : RFProtectionMap)
	{
		rfProtection.second.messagesOff();
	}
}
bool RFProtectionFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool RFProtectionFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
