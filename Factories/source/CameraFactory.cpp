#include <CameraFactory.h>

CameraFactory::CameraFactory()
{
}

CameraFactory::CameraFactory(STATE mode)
{
}

CameraFactory::CameraFactory(const CameraFactory& copyFactory)
{
}

CameraFactory::~CameraFactory()
{
}

void CameraFactory::setup(std::string version)
{
}
void CameraFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("CameraFactory Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& rfHeartbeat : CameraMap)
	{
		rfHeartbeat.second.debugMessagesOn();
	}
}
void CameraFactory::debugMessagesOff()
{
	messenger.printDebugMessage("CameraFactory Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& rfHeartbeat : CameraMap)
	{
		rfHeartbeat.second.debugMessagesOff();
	}
}
void CameraFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("CameraFactory Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& rfHeartbeat : CameraMap)
	{
		rfHeartbeat.second.messagesOn();
	}
}
void CameraFactory::messagesOff()
{
	messenger.printMessage("CameraFactory Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& rfHeartbeat : CameraMap)
	{
		rfHeartbeat.second.messagesOff();
	}
}
bool CameraFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool CameraFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}