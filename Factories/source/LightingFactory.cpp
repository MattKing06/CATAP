#include <LightingFactory.h>

LightingFactory::LightingFactory()
{
}

LightingFactory::LightingFactory(STATE mode)
{
}

LightingFactory::LightingFactory(STATE mode, const std::string& primeLatticeLocation) :
reader(ConfigReader("Lighting", mode, primeLatticeLocation))
{
}

LightingFactory::LightingFactory(const LightingFactory& copyFactory)
{
}

LightingFactory::~LightingFactory()
{
}

void LightingFactory::setup(std::string version)
{
}

void LightingFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Lighting Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& lighting : lightingMap)
	{
		lighting.second.debugMessagesOn();
	}
}
void LightingFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Lighting Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& lighting : lightingMap)
	{
		lighting.second.debugMessagesOff();
	}
}
void LightingFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Lighting Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& lighting : lightingMap)
	{
		lighting.second.messagesOn();
	}
}
void LightingFactory::messagesOff()
{
	messenger.printMessage("Lighting Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& lighting : lightingMap)
	{
		lighting.second.messagesOff();
	}
}
bool LightingFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool LightingFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}