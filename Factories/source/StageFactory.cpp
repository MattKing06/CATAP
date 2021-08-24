#include <StageFactory.h>

StageFactory::StageFactory()
{
}

StageFactory::StageFactory(STATE mode) :
	StageFactory(mode, MASTER_LATTICE_LOCATION)
{
}

StageFactory::StageFactory(STATE mode, const std::string& primeLatticeLocation) :
	mode(mode),
	reader(ConfigReader("Stage", mode, primeLatticeLocation))
{
}

StageFactory::StageFactory(const StageFactory& copyFactory)
{
}

StageFactory::~StageFactory()
{
}

void StageFactory::setup(std::string version)
{
}

void StageFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Stage Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& stage : stageMap)
	{
		stage.second.debugMessagesOn();
	}
}
void StageFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Stage Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& stage : stageMap)
	{
		stage.second.debugMessagesOff();
	}
}
void StageFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Stage Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& stage : stageMap)
	{
		stage.second.messagesOn();
	}
}
void StageFactory::messagesOff()
{
	messenger.printMessage("Stage Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& stage : stageMap)
	{
		stage.second.messagesOff();
	}
}
bool StageFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool StageFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}