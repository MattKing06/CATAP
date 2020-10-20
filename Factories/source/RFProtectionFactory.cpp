#include <RFProtectionFactory.h>
#include <RFProtectionPVRecords.h>

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


void RFProtectionFactory::setupChannels()
{
	for (auto& nameAndProtectionPair : RFProtectionMap)
	{
		std::map<std::string, pvStruct>& pvStructs = nameAndProtectionPair.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			nameAndProtectionPair.second.epicsInterface->retrieveCHID(pv.second);
		}
	}

}

void RFProtectionFactory::setMonitorStatus(pvStruct& pv)
{

		if(pv.pvRecord==RFProtectionRecords::Sta 
			|| pv.pvRecord == RFProtectionRecords::Cmi)
		{
			pv.monitor = true;
		}
		
}

bool RFProtectionFactory::setup(const std::string& version)
{
	if (hasBeenSetup)
	{messenger.printMessage("RF Protection Factory has already been setup"); return true;}
	populateRFProtectionMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		// Could not find any YAML files for configuration..
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto& nameAndProtectionPair : RFProtectionMap)
	{
		//updateNameAliasMap()
		RFProtection rfProtection = nameAndProtectionPair.second;
		std::map<std::string, pvStruct> pvStructs = rfProtection.getPVStructs();
		for (auto& pv : pvStructs)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				setMonitorStatus(pv.second);
				rfProtection.epicsInterface->retrieveCOUNT(pv.second);
				rfProtection.epicsInterface->retrieveCHTYPE(pv.second);
				rfProtection.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				pv.second.MASK = DBE_VALUE;
				if (pv.second.monitor)
				{
					rfProtection.epicsInterface->createSubscription(rfProtection, pv.second);
				}
			}
			else
			{
				messenger.printMessage(nameAndProtectionPair.first, ":", pv.first, " COULD NOT CONNECT TO EPICS.");
			}
		}
		EPICSInterface::sendToEPICS();
	}
	hasBeenSetup = true;
	return true;
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
