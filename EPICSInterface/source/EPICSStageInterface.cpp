#include <EPICSStageInterface.h>

LoggingSystem EPICSStageInterface::static_messenger(true, true);

EPICSStageInterface::EPICSStageInterface()
{
}

EPICSStageInterface::~EPICSStageInterface()
{
}

void EPICSStageInterface::retrieveUpdateFunctionForRecord(pvStruct& pv)
{
	if (pv.monitor)
	{
		if (pv.pvRecord == StageRecords::MABSS)
		{
			pv.updateFunction = updateMABSS;
		}
		if (pv.pvRecord == StageRecords::RPOSS)
		{
			pv.updateFunction = updateRPOSS;
		}
	}
}

void EPICSStageInterface::updateMABSS(const struct event_handler_args args)
{
	Stage* recastStage = getHardwareFromArgs<Stage>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastStage->setPositionSetpoint(pairToUpdate);
	static_messenger.printDebugMessage(recastStage->hardwareName,
		":MABSS received updated value: ", pairToUpdate.second);
}

void EPICSStageInterface::updateRPOSS(const struct event_handler_args args)
{
	Stage* recastStage = getHardwareFromArgs<Stage>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastStage->setCurrentPosition(pairToUpdate);
	static_messenger.printDebugMessage(recastStage->hardwareName,
		":RPOSS received updated value: ", pairToUpdate.second);
	recastStage->moving = !recastStage->isReadPositionEqualToSetPosition();
}

bool EPICSStageInterface::setNewPosition(const pvStruct& pv, const double& newPosition)
{
	bool status = putValue2<double>(pv, newPosition);
	EPICSInterface::sendToEPICS();
	return status;
}


void EPICSStageInterface::debugMessagesOn()
{
	static_messenger.debugMessagesOn();
	static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG On");
	messenger.debugMessagesOn();
	messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG On");
}

void EPICSStageInterface::debugMessagesOff()
{
	static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG OFF");
	static_messenger.debugMessagesOff();	
	messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG OFF");
	messenger.debugMessagesOff();
}

void EPICSStageInterface::messagesOn()
{
	static_messenger.messagesOn();
	static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES On");
	messenger.messagesOn();
	messenger.printMessage(ownerName, " EPICS Interface - MESSAGES On");
}

void EPICSStageInterface::messagesOff()
{
	static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES OFF");
	static_messenger.messagesOff();
	messenger.printMessage(ownerName, " EPICS Interface - MESSAGES OFF");
	messenger.messagesOff();
}

bool EPICSStageInterface::isMessagingOn()
{
	return static_messenger.isMessagingOn();
}

bool EPICSStageInterface::isDebugOn()
{
	return static_messenger.isDebugOn();
}