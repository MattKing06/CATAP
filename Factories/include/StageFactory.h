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
	double getCurrentPosition(const std::string& name);
	double getPositionSetpoint(const std::string& name);
	std::pair<epicsTimeStamp, double> getCurrentPositionTimestamped(const std::string& name);
	std::pair<epicsTimeStamp, double> getPositionSetpointTimestamped(const std::string& name);
	std::map<std::string, double> getCurrentPositions(const std::vector<std::string>& name);
	std::map<std::string, double> getPositionSetpoints(const std::vector<std::string>& name);
	boost::python::dict getCurrentPositions_Py(boost::python::list names);
	boost::python::dict getPositionSetpoints_Py(boost::python::list names);
	std::map<std::string, std::pair<epicsTimeStamp, double>> getCurrentPositionsTimestamped(const std::vector<std::string>& name);
	boost::python::dict  getCurrentPositionsTimestamped_Py(const std::vector<std::string>& name);
	std::map<std::string, std::pair<epicsTimeStamp, double>> getPositionSetpointsTimestamped(const std::vector<std::string>& name);
	boost::python::dict  getPositionSetpointsTimestamped_Py(const std::vector<std::string>& name);
	std::map<std::string, double> getCurrentPositions();
	boost::python::dict getCurrentPositions_Py();
	std::map<std::string, double> getPositionSetpoints();
	boost::python::dict getPositionSetpoints_Py();
	std::map<std::string, std::pair<epicsTimeStamp, double>> getCurrentPositionsTimestamped();
	std::map<std::string, std::pair<epicsTimeStamp, double>> getPositionSetpointsTimestamped();
	Stage& getStage(const std::string& name);
	std::string getFullName(const std::string& alias);
	bool moveStageToDevice(const std::string& stageName, const std::string& deviceName);
	std::map<std::string, bool> clearAllForBeam();
	boost::python::dict clearAllForBeam_Py();
	void updateAliases();
	std::vector<std::string> getAllStageNames();
	boost::python::list getAllStageNames_Py();
	std::vector<std::string> getDevices(const std::string& name);
	boost::python::list getDevices_Py(const std::string& name);
	std::map<std::string, std::vector<std::string>> getAllDevices();
	bool isMoving(const std::string& name);
	bool isAtDevice(const std::string& name, const std::string& device);
	boost::python::dict getAllDevices_Py();
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