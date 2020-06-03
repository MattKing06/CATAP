#include <IMGFactory.h>

IMGFactory::IMGFactory()
{
}

IMGFactory::IMGFactory(STATE mode)
{
}

IMGFactory::IMGFactory(const IMGFactory& copyFactory)
{
}

IMGFactory::~IMGFactory()
{
}

void IMGFactory::setup(std::string version)
{
}

void IMGFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("IMG FACTORY - DEBUG ON");
	// reader.debugMessagesOn();
	for (auto& IMG : IMGMap)
	{
		IMG.second.debugMessagesOn();
	}
}

void IMGFactory::debugMessagesOff()
{
	messenger.printDebugMessage("IMG FACTORY - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& IMG : IMGMap)
	{
		IMG.second.debugMessagesOff();
	}
}

void IMGFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("IMG FACTORY - MESSAGES ON");
	// reader.messagesOn();
	for (auto& IMG : IMGMap)
	{
		IMG.second.messagesOn();
	}
}

void IMGFactory::messagesOff()
{
	messenger.printMessage("IMG FACTORY - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& IMG : IMGMap)
	{
		IMG.second.messagesOff();
	}
}

bool IMGFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool IMGFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
