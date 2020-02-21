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
	void setup(std::vector<std::string> pvListToMonitor);
	bool hasBeenSetup;
	std::map<std::string, Stage> StageMap;
};

#endif // STAGE_FACTORY_H_