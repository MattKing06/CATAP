#include <Stage.h>
Stage::Stage()
{
}

Stage::Stage(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
currentPosition(std::pair<epicsTimeStamp, double>(epicsTimeStamp(), GlobalConstants::double_min)),
positionSetpoint(std::pair<epicsTimeStamp, double>(epicsTimeStamp(), GlobalConstants::double_min))
{
	if (GlobalFunctions::entryExists(paramMap, "min_pos"))
	{minPosition = std::stod(paramMap.at("min_pos"));}
	if (GlobalFunctions::entryExists(paramMap, "max_pos"))
	{maxPosition = std::stod(paramMap.at("max_pos"));}
	if (GlobalFunctions::entryExists(paramMap, "stage_number"))
	{stageNumber = std::stoi(paramMap.at("stage_number"));}
	if (GlobalFunctions::entryExists(paramMap, "has_yag"))
	{std::istringstream(paramMap.at("has_yag")) >> std::boolalpha >> hasYag;}
	if (GlobalFunctions::entryExists(paramMap, "precision"))
	{precision = std::stoi(paramMap.at("precision"));}
	if (GlobalFunctions::entryExists(paramMap, "in_pos"))
	{inPosition = std::stod(paramMap.at("in_pos"));}
	if (GlobalFunctions::entryExists(paramMap, "out_pos"))
	{outPosition = std::stod(paramMap.at("out_pos"));}
}

double Stage::getMinPosition()
{
	return minPosition;
}

bool Stage::doesHaveYag()
{
	return hasYag;
}

double Stage::getMaxPosition()
{
	return maxPosition;
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
