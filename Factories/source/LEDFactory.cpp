#include <LEDFactory.h>

LEDFactory::LEDFactory()
{
}

LEDFactory::LEDFactory(STATE mode)
{
}

LEDFactory::LEDFactory(const LEDFactory& copyFactory)
{
}

LEDFactory::~LEDFactory()
{
}

void LEDFactory::setup(std::string version)
{
}

void LEDFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LED Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& LED : LEDMap)
	{
		LED.second.debugMessagesOn();
	}
}
void LEDFactory::debugMessagesOff()
{
	messenger.printDebugMessage("LED Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& LED : LEDMap)
	{
		LED.second.debugMessagesOff();
	}
}
void LEDFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("LED Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& LED : LEDMap)
	{
		LED.second.messagesOn();
	}
}
void LEDFactory::messagesOff()
{
	messenger.printMessage("LED Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& LED : LEDMap)
	{
		LED.second.messagesOff();
	}
}
bool LEDFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool LEDFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}