#include <ShutterFactory.h>

ShutterFactory::ShutterFactory()
{
}

ShutterFactory::ShutterFactory(STATE mode)
{
}

ShutterFactory::ShutterFactory(const ShutterFactory& copyFactory)
{
}

ShutterFactory::~ShutterFactory()
{
}

void ShutterFactory::setup(std::string version)
{
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