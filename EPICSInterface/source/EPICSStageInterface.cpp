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
}

bool EPICSStageInterface::setNewPosition(const pvStruct& pv, const double& newPosition)
{
	bool status = putValue2<double>(pv, newPosition);
	return status;
}
