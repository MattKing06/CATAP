#include <EPICSValveInterface.h>


LoggingSystem EPICSValveInterface::static_messenger;

EPICSValveInterface::EPICSValveInterface() : EPICSInterface()
{
	static_messenger = LoggingSystem(false,false);
}

EPICSValveInterface::~EPICSValveInterface()
{
	static_messenger.printDebugMessage("EPICSValveInterface Destructor Called");
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
			static_messenger.printDebugMessage("!! WARNING !! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
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
	case GlobalConstants::zero_int: 
	{
		recastValve->setValveState(STATE::CLOSED);
		recastValve->currentState.update(ValveRecords::Sta, STATE::CLOSED);
		break;
	}
	case GlobalConstants::one_int:
	{
		recastValve->setValveState(STATE::OPEN);
		recastValve->currentState.update(ValveRecords::Sta, STATE::OPEN);
		break;
	}
	default:
	{
		recastValve->setValveState(STATE::ERR);
		recastValve->currentState.update(ValveRecords::Sta, STATE::ERR);
		break;
	}
	}
	static_messenger.printDebugMessage("VALVE STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->valveState.second));
}


bool EPICSValveInterface::setNewValveState(const STATE& state, const pvStruct& pv)
{
	putValue<unsigned short>(pv, GlobalConstants::EPICS_ACTIVATE);
	GlobalConstants::PAUSE_1000;
	putValue<unsigned short>(pv, GlobalConstants::EPICS_SEND);
	return true;
}


void EPICSValveInterface::debugMessagesOn()
{
	this->static_messenger.debugMessagesOn();
	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG On");
}

void EPICSValveInterface::debugMessagesOff()
{
	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG OFF");
	this->static_messenger.debugMessagesOff();
}

void EPICSValveInterface::messagesOn()
{
	this->static_messenger.messagesOn();
	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES On");
}

void EPICSValveInterface::messagesOff()
{
	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES OFF");
	this->static_messenger.messagesOff();
}

bool EPICSValveInterface::isMessagingOn()
{
	return this->static_messenger.isMessagingOn();
}

bool EPICSValveInterface::isDebugOn()
{
	return this->static_messenger.isDebugOn();
}