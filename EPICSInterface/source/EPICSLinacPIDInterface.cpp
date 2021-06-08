#include <EPICSLinacPIDInterface.h>

LoggingSystem EPICSLinacPIDInterface::static_messenger;

EPICSLinacPIDInterface::EPICSLinacPIDInterface() : EPICSInterface()
{
	static_messenger = LoggingSystem(false, false);
}

EPICSLinacPIDInterface::~EPICSLinacPIDInterface()
{
	static_messenger.printDebugMessage("EPICSLinacPIDInterface Destructor Called");
}

void EPICSLinacPIDInterface::retrieveUpdateFunctionForRecord(pvStruct& pvStruct) const
{

}

void EPICSLinacPIDInterface::debugMessagesOn()
{
	this->static_messenger.debugMessagesOn();
	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG On");
}

void EPICSLinacPIDInterface::debugMessagesOff()
{
	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG OFF");
	this->static_messenger.debugMessagesOff();
}

void EPICSLinacPIDInterface::messagesOn()
{
	this->static_messenger.messagesOn();
	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES On");
}

void EPICSLinacPIDInterface::messagesOff()
{
	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES OFF");
	this->static_messenger.messagesOff();
}

bool EPICSLinacPIDInterface::isMessagingOn()
{
	return this->static_messenger.isMessagingOn();
}

bool EPICSLinacPIDInterface::isDebugOn()
{
	return this->static_messenger.isDebugOn();
}