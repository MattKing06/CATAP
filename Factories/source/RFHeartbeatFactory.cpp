#include <RFHeartbeatFactory.h>

RFHeartbeatFactory::RFHeartbeatFactory()
{
}

RFHeartbeatFactory::RFHeartbeatFactory(STATE mode)
{
}

RFHeartbeatFactory::RFHeartbeatFactory(const RFHeartbeatFactory& copyFactory)
{
}

RFHeartbeatFactory::~RFHeartbeatFactory()
{
}

void RFHeartbeatFactory::setup(std::string version)
{
}
void RFHeartbeatFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("RFHeartbeat Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.debugMessagesOn();
	}
}
void RFHeartbeatFactory::debugMessagesOff()
{
	messenger.printDebugMessage("RFHeartbeat Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.debugMessagesOff();
	}
}
void RFHeartbeatFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("RFHeartbeat Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.messagesOn();
	}
}
void RFHeartbeatFactory::messagesOff()
{
	messenger.printMessage("RFHeartbeat Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& rfHeartbeat : RFHeartbeatMap)
	{
		rfHeartbeat.second.messagesOff();
	}
}
bool RFHeartbeatFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool RFHeartbeatFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}