#ifndef STAGE_FACTORY_H_
#define STAGE_FACTORY_H_

#include <ConfigReader.h>
#include <Stage.h>
#include <GlobalStateEnums.h>
#include <vector>
class StageFactory
{
public:
	StageFactory();
	StageFactory(STATE mode);
	StageFactory(const StageFactory& copyFactory);
	~StageFactory();
	LoggingSystem messenger;
	STATE mode;
	ConfigReader reader;
	bool setup(std::string version);
	void populateStageMap();
	bool hasBeenSetup;
	std::map<std::string, Stage> stageMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // STAGE_FACTORY_H_