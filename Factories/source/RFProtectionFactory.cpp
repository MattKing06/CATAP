#include <RFProtectionFactory.h>

RFProtectionFactory::RFProtectionFactory()
{
}

RFProtectionFactory::RFProtectionFactory(STATE mode)
{
}

RFProtectionFactory::RFProtectionFactory(const RFProtectionFactory& copyFactory)
{
}

RFProtectionFactory::~RFProtectionFactory()
{
}

void RFProtectionFactory::setup(std::string version)
{
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
