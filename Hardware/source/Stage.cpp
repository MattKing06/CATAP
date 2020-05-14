#include <Stage.h>

Stage::Stage()
{
}

Stage::Stage(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

Stage::Stage(const Stage& copyStage)
{
}

Stage::~Stage()
{
}

void Stage::setPVStructs()
{
}

void Stage::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Stage::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Stage::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Stage::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}
