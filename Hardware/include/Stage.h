#ifndef STAGE_H_
#define STAGE_H_

#include <Hardware.h>
#include <EPICSStageInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

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
};


#endif //STAGE_H_