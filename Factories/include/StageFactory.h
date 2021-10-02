#ifndef STAGE_FACTORY_H_
#define STAGE_FACTORY_H_

#include <ConfigReader.h>
#include <Stage.h>
#include <GlobalStateEnums.h>
#include <vector>
#include <boost/python.hpp>
class StageFactory
{
public:
	StageFactory();
	StageFactory(STATE mode);
	StageFactory(STATE mode, const std::string& primeLatticeLocation);
	StageFactory(const StageFactory& copyFactory);
	~StageFactory();
	LoggingSystem messenger;
	STATE mode;
	ConfigReader reader;
	bool setup(std::string version);
	void populateStageMap();
	void setupChannels();
	void retrieveMonitorStatus(pvStruct& pv);
	bool isReadPositionEqualToSetPosition(const std::string& name);
	Stage& getStage(const std::string& name);
	std::string getFullName(const std::string& alias);
	bool moveStageToDevice(const std::string& stageName, const std::string& deviceName);
	std::map<std::string, bool> clearAllForBeam();
	boost::python::dict clearAllForBeam_Py();
	void updateAliases();
	bool hasBeenSetup;
	std::map<std::string, std::string> aliasesAndFullNames;
	std::map<std::string, Stage> stageMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // STAGE_FACTORY_H_