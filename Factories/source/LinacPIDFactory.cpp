#include <LinacPIDFactory.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>

LinacPIDFactory::LinacPIDFactory() :
	LinacPIDFactory(STATE::OFFLINE)
{
}

LinacPIDFactory::LinacPIDFactory(STATE mode) :
	mode(mode), hasBeenSetup(false),
	reader(ConfigReader("LinacPID", mode)),
	messenger(LoggingSystem(true, true))
{

}

LinacPIDFactory::LinacPIDFactory(const LinacPIDFactory& copyLinacPIDFactory) :
	hasBeenSetup(copyLinacPIDFactory.hasBeenSetup),
	mode(copyLinacPIDFactory.mode),
	messenger(copyLinacPIDFactory.messenger),
	reader(copyLinacPIDFactory.reader)
{
	messenger.printDebugMessage("LinacPIDFactory copy constructor called");
	linacPIDMap.insert(copyLinacPIDFactory.linacPIDMap.begin(), copyLinacPIDFactory.linacPIDMap.end());
}

LinacPIDFactory::~LinacPIDFactory()
{
	messenger.printDebugMessage("LinacPIDFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& linacPID : linacPIDMap)
		{
			auto pvStructsList = linacPID.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					if (pvStruct.second.EVID)
					{
						linacPID.second.epicsInterface->removeSubscription(pvStruct.second);
						ca_flush_io();
					}
				}
				if (pvStruct.second.CHID)
				{
					linacPID.second.epicsInterface->removeChannel(pvStruct.second);
					ca_pend_io(CA_PEND_IO_TIMEOUT);
				}
			}
		}
	}
}

void LinacPIDFactory::setupChannels()
{
	for (auto& linacPID : linacPIDMap)
	{
		std::map<std::string, pvStruct>& pvStructs = linacPID.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			linacPID.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

void LinacPIDFactory::populateLinacPIDMap()
{
	messenger.printDebugMessage("LinacPIDFactory is populating LinacPID Map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support.");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("LinacPID Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(linacPIDMap);
	}
	messenger.printDebugMessage("LinacPID Factory has finished populating LinacPID Map");
}

bool LinacPIDFactory::setup(const std::string& VERSION)
{
	messenger.printDebugMessage("called LinacPID Factory  setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup LinacPID Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics linacPID interface has been initialized in linacPID constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateLinacPIDMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}


	setupChannels();
	EPICSInterface::sendToEPICS();


	for (auto& linacPID : linacPIDMap)
	{
		// update aliases for linacPID in map
		updateAliasNameMap(linacPID.second);
		std::map<std::string, pvStruct>& linacPIDPVStructs = linacPID.second.getPVStructs();
		for (auto& pv : linacPIDPVStructs)
		{
			// sets the monitor state in the pvstruict to true or false
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrieveMonitorStatus(pv.second);
				linacPID.second.epicsInterface->retrieveCHTYPE(pv.second);
				linacPID.second.epicsInterface->retrieveCOUNT(pv.second);
				linacPID.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					linacPID.second.epicsInterface->createSubscription(linacPID.second, pv.second);
					EPICSInterface::sendToEPICS();
				}
			}
			else
			{
				messenger.printMessage(linacPID.first, " CANNOT CONNECT TO EPICS");
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

void LinacPIDFactory::retrieveMonitorStatus(pvStruct& pvStruct) const
{

}

LinacPID& LinacPIDFactory::getLinacPID(const std::string& fullLinacPIDName)
{
	std::string fullName = getFullName(fullLinacPIDName);
	if (GlobalFunctions::entryExists(linacPIDMap, fullName))
	{
		return linacPIDMap.at(fullName);
	}
}

std::string LinacPIDFactory::getFullName(const std::string& nameToCheck) const
{
	if (GlobalFunctions::entryExists(aliasNameMap, nameToCheck))
	{
		return aliasNameMap.at(nameToCheck);
	}
	else
	{
		return ENUM_TO_STRING(STATE::UNKNOWN);
	}
}

std::vector<std::string> LinacPIDFactory::getAllLinacPIDNames() const
{
	std::vector<std::string> returnNames;
	for (auto&& item : linacPIDMap)
	{
		returnNames.push_back(item.first);
	}
	return returnNames;
}

boost::python::list LinacPIDFactory::getAllLinacPIDNames_Py() const
{
	std::vector<std::string> returnNames = getAllLinacPIDNames();
	return to_py_list(returnNames);
}

void LinacPIDFactory::updateAliasNameMap(const LinacPID& linacPID)
{
	// first add in the linacPID full name
	std::string fullName = linacPID.getHardwareName();
	messenger.printMessage("updateAliasNameMap ", fullName);
	if (GlobalFunctions::entryExists(aliasNameMap, fullName))
	{
		messenger.printMessage("!!ERROR!! ", fullName, " linacPID name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", fullName, " added to alias_name_map");
		aliasNameMap[fullName] = fullName;
	}

	// now add in the aliases
	std::vector<std::string> aliases = linacPID.getAliases();
	for (auto&& nextAlias : aliases)
	{
		if (GlobalFunctions::entryExists(aliasNameMap, nextAlias))
		{
			messenger.printMessage("!!ERROR!! ", linacPID.getHardwareName(), " alias = ", nextAlias, " already exists");
		}
		else
		{
			aliasNameMap[nextAlias] = fullName;
			messenger.printMessage("Added alias " + nextAlias + " for " + fullName);
		}
	}
}

void LinacPIDFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LinacPID-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.debugMessagesOn();
	}
}
void LinacPIDFactory::debugMessagesOff()
{
	messenger.printDebugMessage("LinacPID-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.debugMessagesOff();
	}
}
void LinacPIDFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("LinacPID-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.messagesOn();
	}
}
void LinacPIDFactory::messagesOff()
{
	messenger.printMessage("LinacPID-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& linacPID : linacPIDMap)
	{
		linacPID.second.messagesOff();
	}
}
bool LinacPIDFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool LinacPIDFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}


