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
	/*! turns debug messaging on for this EPICSInterface instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this EPICSInterface instance*/
	void debugMessagesOff();
	/*! turns messaging on for this EPICSInterface instance*/
	void messagesOn();
	/*! turns messaging off for this EPICSInterface instance*/
	void messagesOff();
	/*! Checks if messaging flag is on
	* @param[out] bool : returns true if messenger messagesOn flag is true, false otherwise.*/
	bool isMessagingOn();
	/*! Checks if debug flag is on
	* @param[out] bool : returns true if messenger debugOn flag is true, false otherwise.*/
	bool isDebugOn();
};

#endif //EPICS_STAGE_INTERFACE_H_