#ifndef EPICS_STAGE_INTERFACE_H_
#define EPICS_STAGE_INTERFACE_H_
#include <EPICSInterface.h>
#include <StagePVRecords.h>
#include <Stage.h>

class EPICSStageInterface : public EPICSInterface
{
public:
	EPICSStageInterface();
	~EPICSStageInterface();
	void retrieveUpdateFunctionForRecord(pvStruct& pv);
	static void updateMABSS(const struct event_handler_args);
	static void updateRPOSS(const struct event_handler_args);
	bool setNewPosition(const pvStruct& pv, const double& newPosition);
	static LoggingSystem static_messenger;
	std::string ownerName;
};

#endif //EPICS_STAGE_INTERFACE_H_