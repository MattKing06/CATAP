#include <RFProtectionFactory.h>

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
	messenger.printDebugMessage("MagnetFactory is populating the magnet map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Magnet Factory calling parseNextYamlFile");
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
