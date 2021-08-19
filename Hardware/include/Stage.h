#ifndef STAGE_H_
#define STAGE_H_

#include <Hardware.h>
#include <EPICSStageInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <GlobalFunctions.h>
#include <boost/make_shared.hpp>
#include <StagePVRecords.h>

class EPICSStageInterface;

typedef boost::shared_ptr<EPICSStageInterface> EPICSStageInterface_sptr;

class Stage : public Hardware
{
public:
	Stage();
	Stage(const std::map<std::string, std::string>& paramMap, STATE mode);
	Stage(const Stage& copyStage);
	~Stage();
	void setPVStructs();
	EPICSStageInterface_sptr epicsInterface;
	std::map<std::string, std::string> StageParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	std::pair<epicsTimeStamp, double> getCurrentPosition();
	void setCurrentPosition(std::pair<epicsTimeStamp, double> newValue);
	std::pair<epicsTimeStamp, double> getPositionSetpoint();
	void setPositionSetpoint(std::pair<epicsTimeStamp, double> newValue);
	void setNewPosition(double newPosition);
	double getMinPosition();
	double getMaxPosition();
	double getInPosition();
	double getOutPosition();
	size_t getStageNumber();
	bool doesHaveYag();
	size_t getPrecision();
private:
	std::pair<epicsTimeStamp, double> currentPosition;
	std::pair<epicsTimeStamp, double> positionSetpoint;
	double minPosition;
	double maxPosition;
	double inPosition;
	double outPosition;
	size_t stageNumber;
	bool hasYag;
	size_t precision;
	std::map<std::string, double> deviceAndPositionMap;
};


#endif //STAGE_H_