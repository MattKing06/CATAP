#include "EPICSMagnetInterface.h"

LoggingSystem EPICSMagnetInterface::messenger;

EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{
	messenger = LoggingSystem(true, true);
}

EPICSMagnetInterface::~EPICSMagnetInterface()
{
	messenger.printDebugMessage("EPICSMagnetInterface Destructor Called");
}
void EPICSMagnetInterface::retrieveupdateFunctionForRecord(pvStruct &pvStruct) const
{
	if (pvStruct.pvRecord == MagnetRecords::GETSETI)
	{
		pvStruct.updateFunction = this->updateGETSETI;
	}
	else if (pvStruct.pvRecord == MagnetRecords::RPOWER)
	{
		pvStruct.updateFunction = this->updatePSUState;
	}
	else if (pvStruct.pvRecord == MagnetRecords::READI)
	{
		pvStruct.updateFunction = this->updateREADI;
	}
	else if (pvStruct.pvRecord == MagnetRecords::RILK)
	{
		pvStruct.updateFunction = this->updateRILK;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord );
	}
}


// TODO rename 
//void EPICSMagnetInterface::updateCurrent(const struct event_handler_args args)
void EPICSMagnetInterface::updateGETSETI(const struct event_handler_args args)
{
	Magnet* recastMagnet = getHardwareFromArgs<Magnet>(args);
	
	updateTimeStampDoublePair(args, recastMagnet->GETSETI);
	
	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("GETSETI"), args);
	//double value = returnValueFromArgsAsDouble(args);
	//recastMagnet->offlineSETI(value);
	messenger.printDebugMessage("GETSETI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ std::to_string(recastMagnet->GETSETI.second));
}

// TODO rename 
void EPICSMagnetInterface::updatePSUState(const struct event_handler_args args)
{
	Magnet* recastMagnet = getHardwareFromArgs<Magnet>(args);
	std::pair<epicsTimeStamp, short> pairToUpdate = getTimeStampShortPair(args);
	recastMagnet->psuState.first = pairToUpdate.first;

	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: recastMagnet->psuState.second = STATE::OFF; break;
	case GlobalConstants::one_int:  recastMagnet->psuState.second = STATE::ON; break;
	default:
		recastMagnet->psuState.second = STATE::ERR;
	}
	//psuState2.second = tv->value;
	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("RPOWER"), args);
	//STATE value = returnValueFromArgsAsState(args);
	//recastMagnet->setPSUState(value);
	messenger.printDebugMessage("RPOWER VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ ENUM_TO_STRING(recastMagnet->psuState.second));
}

void EPICSMagnetInterface::updateREADI(const struct event_handler_args args)
{
	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	updateTimeStampDoublePair(args, recastMagnet->READI);

	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("READI"), args);
	//double value = returnValueFromArgsAsDouble(args);
	//recastMagnet->READI2 = returnTSVFromArgsDouble(args);
	//messenger.printDebugMessage("READI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
	//													+ std::to_string(recastMagnet->READI.second));
}

void EPICSMagnetInterface::updateRILK(const struct event_handler_args args)
{
	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastMagnet->ilkState.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: recastMagnet->ilkState.second = STATE::OK; break;
	case GlobalConstants::one_int:  recastMagnet->ilkState.second = STATE::ERR; break;
	default:
		recastMagnet->ilkState.second = STATE::ERR;
	}
	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("RILK"), args);
	//STATE value = returnValueFromArgsAsState(args);
	//recastMagnet->setILKState(value);
	messenger.printDebugMessage("RILK VALUE FOR: " + recastMagnet->getHardwareName() + ": "
								+ ENUM_TO_STRING(recastMagnet->ilkState.second));
}





// TODO rename 
bool EPICSMagnetInterface::setNewCurrent(const double &value, const pvStruct &pv) const
{
	//we have checked that pvRecord is SETI before reaching here.
	return putValue2(pv, value);
}

// TODO rename 
bool EPICSMagnetInterface::setNewPSUState(const STATE value, const pvStruct& pv) const
{
	// This needs to switch 
	int epics_value = GlobalConstants::zero_int;
	switch (value)
	{
	case STATE::ON: epics_value = GlobalConstants::one_int; break;
	case STATE::OFF: epics_value = GlobalConstants::zero_int; break;
	}
	return putValue2(pv, epics_value);
}
