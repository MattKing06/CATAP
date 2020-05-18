#include "LLRFFactory.h"
#include "GlobalStateEnums.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"
#include "PythonTypeConversions.h"

LLRFFactory::LLRFFactory() : 
LLRFFactory(STATE::OFFLINE)
{
}

LLRFFactory::LLRFFactory(STATE mode) :
mode(mode), 
hasBeenSetup(false),
reader(ConfigReader("LLRF", mode)),
messenger(LoggingSystem(true, true)),
machineAreas(std::vector<TYPE>{TYPE::ALL_VELA_CLARA})
{
	messenger.printDebugMessage("LLRFFactory constructed");
}

LLRFFactory::LLRFFactory(const LLRFFactory& copyFactory) :
hasBeenSetup(copyFactory.hasBeenSetup),
mode(copyFactory.mode),
messenger(copyFactory.messenger),
reader(copyFactory.reader),
machineAreas(machineAreas)
{
}

LLRFFactory::~LLRFFactory()
{
}


bool LLRFFactory::setup()
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool LLRFFactory::setup(const std::string& version)
{
	return setup(version, machineAreas);
}
bool LLRFFactory::setup(TYPE machineArea)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool LLRFFactory::setup(const std::string& version, TYPE machineArea)
{
	return setup(GlobalConstants::nominal, std::vector<TYPE>{machineArea});
}
bool LLRFFactory::setup(const std::vector<TYPE>& machineAreas)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool LLRFFactory::setup(const boost::python::list& machineAreas)
{
	return setup(GlobalConstants::nominal, to_std_vector<TYPE>(machineAreas));
}
bool LLRFFactory::setup(const std::string& version, const boost::python::list& machineAreas)
{
	return setup(version, to_std_vector<TYPE>(machineAreas));
}
bool LLRFFactory::setup(const std::string& version, const std::vector<TYPE>& machineAreas)
{
	messenger.printDebugMessage("called LLRF Factory  setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup LLRF Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	// epics valve interface has been initialized in valve constructor
	// but we have a lot of PV information to retrieve from EPICS first
	// so we will cycle through the PV structs, and set up their values.
	populateLLRFMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}

	//setupChannels();
	//EPICSInterface::sendToEPICS();


	for (auto& llrf : LLRFMap)
	{
		// update aliases for valve in map
		updateAliasNameMap(llrf.second);
		std::map<std::string, pvStruct>& pvstruct = llrf.second.getPVStructs();
	//	for (auto& pv : pvstruct)
	//	{
	//		// sets the monitor state in the pvstruict to true or false
	//		if (ca_state(pv.second.CHID) == cs_conn)
	//		{
	//			retrieveMonitorStatus(pv.second);
	//			valve.second.epicsInterface->retrieveCHTYPE(pv.second);
	//			valve.second.epicsInterface->retrieveCOUNT(pv.second);
	//			valve.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
	//			//// not sure how to set the mask from EPICS yet.
	//			pv.second.MASK = DBE_VALUE;
	//			messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
	//				"write", std::to_string(ca_write_access(pv.second.CHID)),
	//				"state", std::to_string(ca_state(pv.second.CHID)));
	//			if (pv.second.monitor)
	//			{
	//				valve.second.epicsInterface->createSubscription(valve.second, pv.second);
	//				EPICSInterface::sendToEPICS();
	//			}
	//		}
	//		else
	//		{
	//			messenger.printMessage(valve.first, " CANNOT CONNECT TO EPICS");
	//		}
	//	}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}


void LLRFFactory::populateLLRFMap()
{
	messenger.printDebugMessage("LLRFFactory is populating the LLRF object map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, "
			"please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("LLRFFactory calling parseNextYamlFile");
		reader.parseNextYamlFile(LLRFMap);
	}
	messenger.printDebugMessage("LLRFFactory has finished populating "
		"the LLRF MAP, found ", LLRFMap.size(), " LLRF objects");
}

void LLRFFactory::updateAliasNameMap(const LLRF& llrf)
{
	// first add in the magnet full name
	std::string full_name = llrf.getHardwareName();
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
	std::vector<std::string> aliases = llrf.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", llrf.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}




void LLRFFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LLRF Factory - DEBUG On");
	//reader.debugMessagesOn();
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOn();
	}
}
void LLRFFactory::debugMessagesOff()
{
	messenger.printDebugMessage("LLRF Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOn();
	}
}
void LLRFFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("LLRF Factory - MESSAGES ON");
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOn();
	}
}
void LLRFFactory::messagesOff()
{
	messenger.printMessage("LLRF Factory - MESSAGES OFF");
	messenger.messagesOff();
	//reader.messagesOff();
	for (auto& llrf : LLRFMap)
	{
		llrf.second.messagesOff();
	}
}
bool LLRFFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool LLRFFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
