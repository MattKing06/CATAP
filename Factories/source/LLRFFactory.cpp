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


	//for (auto& valve : valveMap)
	//{
	//	// update aliases for valve in map
	//	updateAliasNameMap(valve.second);
	//	std::map<std::string, pvStruct>& valvePVStructs = valve.second.getPVStructs();
	//	for (auto& pv : valvePVStructs)
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
	//}
	hasBeenSetup = true;
	return hasBeenSetup;
}


void LLRFFactory::populateLLRFMap()
{
	messenger.printDebugMessage("LLRFFactory is populating the magnet map");
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
		"the LLRF MAP, found ", LLRFMap.size(), " magnets objects");
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
