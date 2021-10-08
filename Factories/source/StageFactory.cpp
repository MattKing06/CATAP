#include <StageFactory.h>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptors.hpp>
StageFactory::StageFactory()
{
}

StageFactory::StageFactory(STATE mode) :
StageFactory(mode, MASTER_LATTICE_LOCATION)
{
}

StageFactory::StageFactory(STATE mode, const std::string& primeLatticeLocation) :
	mode(mode),
	reader(ConfigReader("Stage", mode, primeLatticeLocation)),
	hasBeenSetup(false),
	aliasesAndFullNames(std::map<std::string, std::string>())
{
}

StageFactory::StageFactory(const StageFactory& copyFactory)
{
	stageMap.insert(copyFactory.stageMap.begin(), copyFactory.stageMap.end());
}

StageFactory::~StageFactory()
{
}


void StageFactory::populateStageMap()
{
	messenger.printDebugMessage("StageFactory is populating Stage Map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support.");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Stage Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(stageMap);
	}
	messenger.printDebugMessage("Stage Factory has finished populating Stage Map");
}

void StageFactory::setupChannels()
{
	for (auto&& stage : stageMap)
	{
		std::map<std::string, pvStruct>& pvStructs = stage.second.getPVStructs();
		
		for (auto&& pv : pvStructs)
		{
			std::cout << pv.second.fullPVName << std::endl;
			stage.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

std::map<std::string, bool> StageFactory::clearAllForBeam()
{
	std::map<std::string, bool> nameAndStatus;
	for (auto&& stage : stageMap)
	{
		nameAndStatus[stage.first] = stage.second.clearForBeam();
	}
	return nameAndStatus;
}


std::vector<std::string> StageFactory::getAllStageNames()
{
	std::vector<std::string> names;
	for (auto&& stage : stageMap)
	{
		names.push_back(stage.first);
	}
	return names;
}
boost::python::list StageFactory::getAllStageNames_Py()
{
	return to_py_list(getAllStageNames());
}

std::vector<std::string> StageFactory::getDevices(const std::string& name)
{
	if (GlobalFunctions::entryExists(aliasesAndFullNames, name))
	{
		std::string fullName = aliasesAndFullNames.at(name);
		return stageMap.at(fullName).getDevices();
	}
	else if (GlobalFunctions::entryExists(stageMap, name))
	{
		return stageMap.at(name).getDevices();
	}
}

boost::python::list StageFactory::getDevices_Py(const std::string& name)
{
	return to_py_list(getDevices(name));
}

std::map<std::string, std::vector<std::string> > StageFactory::getAllDevices()
{
	std::map<std::string, std::vector<std::string> > nameAndDevices;
	for (auto&& stage : stageMap)
	{
		nameAndDevices[stage.first] = stage.second.getDevices();
	}
	return nameAndDevices;
}

boost::python::dict StageFactory::getAllDevices_Py()
{
	return to_py_dict(getAllDevices());
}


boost::python::dict StageFactory::clearAllForBeam_Py()
{
	return to_py_dict(clearAllForBeam());
}

bool StageFactory::moveStageToDevice(const std::string& stageName, const std::string& deviceName)
{
	if (GlobalFunctions::entryExists(aliasesAndFullNames, stageName))
	{
		std::string fullName = aliasesAndFullNames.at(stageName);
		return stageMap.at(fullName).moveToDevice(deviceName);
	}
	else if (GlobalFunctions::entryExists(stageMap, stageName))
	{
		return stageMap.at(stageName).moveToDevice(deviceName);
	}
	else
	{
		std::vector<std::string> names;
		boost::copy(stageMap | boost::adaptors::map_keys, std::back_inserter(names));
		std::string namesList = GlobalFunctions::toString(names);
		messenger.printDebugMessage("Stage: ", stageName, " does not exist. Please choose another stage: ",
									namesList);
		return false;
	}
}

void StageFactory::updateAliases()
{
	for (auto&& stage : stageMap)
	{
		for (auto&& alias : stage.second.getAliases())
		{
			aliasesAndFullNames[alias] = stage.first;
			messenger.printDebugMessage("STAGE: ", stage.first, " ALIAS: ", alias);
		}
	}
}

void StageFactory::retrieveMonitorStatus(pvStruct& pv)
{
	if (pv.pvRecord == StageRecords::MABSS)
	{
		pv.monitor = true;
	}
	if (pv.pvRecord == StageRecords::RPOSS)
	{
		pv.monitor = true;
	}
}

bool StageFactory::isReadPositionEqualToSetPosition(const std::string& name)
{
	if (GlobalFunctions::entryExists(aliasesAndFullNames, name))
	{
		std::string fullName = aliasesAndFullNames.at(name);
		return stageMap.at(fullName).isReadPositionEqualToSetPosition();
	}
	else if (GlobalFunctions::entryExists(stageMap, name))
	{
		return stageMap.at(name).isReadPositionEqualToSetPosition();
	}
}

Stage& StageFactory::getStage(const std::string& name)
{
	if (GlobalFunctions::entryExists(aliasesAndFullNames, name))
	{
		std::string fullName = aliasesAndFullNames.at(name);
		return stageMap.at(fullName);
	}
	else if (GlobalFunctions::entryExists(stageMap, name))
	{
		return stageMap.at(name);
	}
	else
	{
		messenger.printMessage("Could not find: ", name, " in stages. Please use a valid stage name.");
	}
}

std::string StageFactory::getFullName(const std::string& alias)
{
	if (GlobalFunctions::entryExists(aliasesAndFullNames, alias))
	{
		return aliasesAndFullNames.at(alias);
	}
	else
	{
		messenger.printMessage("Alias: ", alias, " could not be found in configs.");
	}
}

bool StageFactory::setup(std::string version)
{
	if (hasBeenSetup)
	{
		return true;
	}
	populateStageMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto&& stage : stageMap)
	{
		std::map<std::string, pvStruct>& pvStructs = stage.second.getPVStructs();
		for (auto&& pv : pvStructs)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrieveMonitorStatus(pv.second);
				stage.second.epicsInterface->retrieveCHTYPE(pv.second);
				stage.second.epicsInterface->retrieveCOUNT(pv.second);
				stage.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					stage.second.epicsInterface->createSubscription(stage.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(stage.first, " CANNOT CONNECT TO EPICS");
			}
			EPICSInterface::sendToEPICS();
		}
	}
	updateAliases();
	hasBeenSetup = true;
	return hasBeenSetup;
}

void StageFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Stage Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& stage : stageMap)
	{
		stage.second.debugMessagesOn();
	}
}
void StageFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Stage Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& stage : stageMap)
	{
		stage.second.debugMessagesOff();
	}
}
void StageFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Stage Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& stage : stageMap)
	{
		stage.second.messagesOn();
	}
}
void StageFactory::messagesOff()
{
	messenger.printMessage("Stage Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& stage : stageMap)
	{
		stage.second.messagesOff();
	}
}
bool StageFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool StageFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}