#include <RFModulatorFactory.h>

RFModulatorFactory::RFModulatorFactory()
{
}

RFModulatorFactory::RFModulatorFactory(STATE mode)
{
}

RFModulatorFactory::RFModulatorFactory(const RFModulatorFactory& copyFactory)
{
}

RFModulatorFactory::~RFModulatorFactory()
{
}

void RFModulatorFactory::setup(std::string version)
{
}

void RFModulatorFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("RF Modulator Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.debugMessagesOn();
	}
}
void RFModulatorFactory::debugMessagesOff()
{
	messenger.printDebugMessage("RF Modulator Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.debugMessagesOff();
	}
}
void RFModulatorFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("RF Modulator Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.messagesOn();
	}
}
void RFModulatorFactory::messagesOff()
{
	messenger.printMessage("RF Modulator Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& rfModulator : RFModulatorMap)
	{
		rfModulator.second.messagesOff();
	}
}
bool RFModulatorFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool RFModulatorFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
