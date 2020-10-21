#include <ShutterFactory.h>
#include <ShutterPVRecords.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>

ShutterFactory::ShutterFactory()
{
}

ShutterFactory::ShutterFactory(STATE mode):
	messenger(LoggingSystem(true, true)),
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Shutter", mode))
{
}

ShutterFactory::ShutterFactory(const ShutterFactory& copyFactory)
{
}

ShutterFactory::~ShutterFactory()
{

}

bool ShutterFactory::setup(const std::string version)
{
	messenger.printDebugMessage("called ShutterFactory setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup ShutterFactory : it has been setup");
		return true;
	}
	//std::cout << "populateMagnetMap()" << std::endl;
	populateShutterMap();
	//std::cout << "populateMagnetMap() fin" << std::endl;
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	// 
	//convertConfigStringsToGlobalTypeEnums();
	setupChannels();
	EPICSInterface::sendToEPICS();
	messenger.printMessage("All Shutter CHIDs setup, getting alias names and creating subscriptions");
	/*
		LOOP OVER ALL MAGNETS AGAIN TO SET MORE EPICS INFO.
	*/
	for (auto& shutter : shutterMap)
	{
		updateAliasNameMap(shutter.second);
		messenger.printMessage("Set up EPICS suscriptions for " + shutter.second.getHardwareName());
		
		//std::cout << "shutter.first = " << shutter.first << std::endl;
		/*
			NOW CHANNELS HAVE BEEN SENT TO EPICS, SET UP EVERYTHING ELSE
		*/
		std::map<std::string, pvStruct>& shutterPVStructs = shutter.second.getPVStructs();
		for (auto& pv : shutterPVStructs)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				//messenger.printMessage("Connected!, getting some channel data (COUNT, CHTYPE, ... )");

				shutter.second.epicsInterface->retrieveCHTYPE(pv.second);
				shutter.second.epicsInterface->retrieveCOUNT(pv.second);
				shutter.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;

				setMonitorStatus(pv.second);
				if (pv.second.monitor)
				{
					messenger.printMessage(shutter.first, ":", pv.second.pvRecord, " should be monitored");
					shutter.second.epicsInterface->createSubscription(shutter.second, pv.second);
				}
				messenger.printDebugMessage(pv.second.pvRecord, ": read/write/state = ",
					std::to_string(ca_read_access(pv.second.CHID)),
					std::to_string(ca_write_access(pv.second.CHID)),
					std::to_string(ca_state(pv.second.CHID)));
			}
			else
			{
				messenger.printMessage(shutter.first, ":", pv.second.pvRecord, " CANNOT CONNECT TO EPICS");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
		}
		EPICSInterface::sendToEPICS();
	}

	hasBeenSetup = true;
	//std::cout << "hasBeenSetup = true " << std::endl;
	return hasBeenSetup;
}

std::string ShutterFactory::getFullName(const std::string& name_to_check) const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return "UNKNOWN_NAME";
}

bool ShutterFactory::open(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).open();
	}
	return false;
}
bool ShutterFactory::close(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).close();
	}
	return false;
}
bool ShutterFactory::isOpen(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isOpen();
	}
	return false;
}
bool ShutterFactory::isClosed(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isClosed();
	}
	return false;

}
STATE ShutterFactory::getState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).getState();
	}
	return STATE::UNKNOWN;
}

int ShutterFactory::getCMI(const  std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isClosed();
	}
	return false;
}
std::map<std::string, STATE> ShutterFactory::getCMIBitMap(const  std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).getCMIBitMap();
	}
	std::map<std::string, STATE> dummy;
	return dummy;
}

boost::python::dict ShutterFactory::getCMIBitMap_Py(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).getCMIBitMap_Py();
	}
	boost::python::dict dummy;
	return dummy;
}
// GET MAGNET OBJECT  FUNCTIONS
//
Shutter& ShutterFactory::getShutter(const std::string& shutter_name)
{
	std::string full_name = getFullName(shutter_name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name);
	}
	return dummy_shutter;
}
std::vector<std::string> ShutterFactory::getAllShutterNames() const
{
	std::vector<std::string> r;
	for (auto&& shutter : shutterMap)
	{
		r.push_back(shutter.first);
	}
	return r;
}
boost::python::list ShutterFactory::getAllShutterNames_Py() const
{
	return to_py_list<std::string>(getAllShutterNames());
}

ShutterState ShutterFactory::getShutterState(const std::string& shutter_name)const
{
	std::string full_name = getFullName(shutter_name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).getShutterState();
	}
	return ShutterState();
}
//ShutterState ShutterFactory::getShutterState_Py(const std::string& shutter_name)const
//{
//	std::string full_name = getFullName(shutter_name);
//	if (GlobalFunctions::entryExists(shutterMap, full_name))
//	{
//		return shutterMap.at(full_name).getShutterState_Py();
//	}
//	return ShutterState();
//}


std::map<std::string, ShutterState> ShutterFactory::getShutterStates()const
{
	std::map<std::string, ShutterState> r;
	for (auto&& shutter : shutterMap)
	{
		r[shutter.first] = shutter.second.getShutterState();
	}
	return r;
}

boost::python::dict ShutterFactory::getShutterStates_Py()const
{
	return to_py_dict<std::string, ShutterState>(getShutterStates());
}


boost::python::dict ShutterFactory::getShutterStateDictionary(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).getShutterStateDictionary();
	}
	boost::python::dict r;
	return r;
}
boost::python::dict ShutterFactory::getShutterStateDictionaries()const
{
	boost::python::dict r;
	for (auto&& shutter : shutterMap)
	{
		r[shutter.first] = shutter.second.getShutterStateDictionary();
	}
	return r;
}


void ShutterFactory::updateAliasNameMap(const Shutter& shutter)
{
	// first add in the magnet full name
	std::string full_name = shutter.getHardwareName();
	//messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		//messenger.printMessage("!!ERROR!! ", full_name, " magnet name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases
	std::vector<std::string> aliases = shutter.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", shutter.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}

void ShutterFactory::setMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == ShutterRecords::State||
		pvStruct.pvRecord == ShutterRecords::Cmi)
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
}


bool ShutterFactory::isEnergyInterlockGood(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isEnergyInterlockBad();
	}
	return false;
}

bool ShutterFactory::isEnergyInterlockBad(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isEnergyInterlockBad();
	}
	return false;
}
bool ShutterFactory::isChargeInterlockGood(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isChargeInterlockGood();
	}
	return false;
}
bool ShutterFactory::isChargeInterlockBad(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isChargeInterlockBad();
	}
	return false;
}
bool ShutterFactory::isPSInterlockGood(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isPSInterlockGood();
	}
	return false;;
}
bool ShutterFactory::isPSInterlockBad(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, full_name))
	{
		return shutterMap.at(full_name).isPSInterlockBad();
	}
	return false;
}


void ShutterFactory::populateShutterMap()
{
	messenger.printDebugMessage("ShutterFactory is populating Valve Map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support.");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Shutter Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(shutterMap);
	}
	messenger.printDebugMessage("ShutterFactory has finished populating Shutter Map");
}

void ShutterFactory::setupChannels()
{
	for (auto& shutter : shutterMap)
	{
		std::map<std::string, pvStruct>& pvStructs = shutter.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			shutter.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

std::vector<std::string> ShutterFactory::getAliases(const std::string& name) const
{
	std::string fullName = getFullName(name);
	if (GlobalFunctions::entryExists(shutterMap, fullName))
	{
		return shutterMap.at(fullName).getAliases();
	}
	std::vector<std::string> dummy;
	return dummy;
}

void ShutterFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Shutter Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& shutter : shutterMap)
	{
		shutter.second.debugMessagesOn();
	}
}
void ShutterFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Shutter Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& shutter : shutterMap)
	{
		shutter.second.debugMessagesOff();
	}
}
void ShutterFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Shutter Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& shutter : shutterMap)
	{
		shutter.second.messagesOn();
	}
}
void ShutterFactory::messagesOff()
{
	messenger.printMessage("Shutter Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& shutter : shutterMap)
	{
		shutter.second.messagesOff();
	}
}
bool ShutterFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool ShutterFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}