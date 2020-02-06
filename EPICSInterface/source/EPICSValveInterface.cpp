#include <EPICSValveInterface.h>


LoggingSystem EPICSValveInterface::messenger;

EPICSValveInterface::EPICSValveInterface() : EPICSInterface()
{
	messenger = LoggingSystem(true, true);
}

EPICSValveInterface::~EPICSValveInterface()
{
	messenger.printDebugMessage("EPICSValveInterface Destructor Called");
}

void EPICSValveInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == ValveRecords::Sta)
	{
		pvStruct.updateFunction = updateValveState;
	}
	else
	{
		messenger.printDebugMessage("!! WARNING !! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSValveInterface::updateValveState(const struct event_handler_args args)
{
	Valve* recastValve = getHardwareFromArgs<Valve>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastValve->valveState.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: recastValve->valveState.second = STATE::CLOSED; break;
	case GlobalConstants::one_int: recastValve->valveState.second = STATE::OPEN; break;
	default:
		recastValve->valveState.second = STATE::ERR;
	}
	messenger.printDebugMessage("VALVE STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->valveState.second));
}


bool EPICSValveInterface::setNewValveState(const STATE& state, const pvStruct& pv)
{
	int epicsValue = GlobalConstants::zero_int;
	switch (state) 
	{
	case STATE::OPEN : epicsValue = GlobalConstants::one_int; break;
	case STATE::CLOSED : epicsValue = GlobalConstants::zero_int; break;
	}
	
	return putValue2(pv, epicsValue);
}
