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
	if (pvStruct.monitor)
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
}

void EPICSValveInterface::updateValveState(const struct event_handler_args args)
{
	Valve* recastValve = getHardwareFromArgs<Valve>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastValve->valveState.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
		std::cout << "UPDATE VALUE: " << pairToUpdate.second << std::endl;
	case GlobalConstants::zero_int: recastValve->setValveState(STATE::CLOSED); break;
	case GlobalConstants::one_int: recastValve->setValveState(STATE::OPEN); break;
	default:
		recastValve->setValveState(STATE::ERR);
	}
	messenger.printDebugMessage("VALVE STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->valveState.second));
}


bool EPICSValveInterface::setNewValveState(const STATE& state, const pvStruct& pv)
{
	putValue<unsigned short>(pv, GlobalConstants::EPICS_ACTIVATE);
	GlobalConstants::PAUSE_1000;
	putValue<unsigned short>(pv, GlobalConstants::EPICS_SEND);
	return true;
}
