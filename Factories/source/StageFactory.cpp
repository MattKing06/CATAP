#include <StageFactory.h>

StageFactory::StageFactory()
{
}

StageFactory::StageFactory(STATE mode) :
	mode(mode), hasBeenSetup(false),
	reader(ConfigReader("Stage", mode)),
	messenger(LoggingSystem(true, true))
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

Stage& StageFactory::getStage(const std::string& name)
{
	if (GlobalFunctions::entryExists(stageMap, name))
	{
		return stageMap.at(name);
	}
	else
	{
		messenger.printMessage("Could not find: ", name, " in stages. Please use a valid stage name.");
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