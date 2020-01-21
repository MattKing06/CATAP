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
void EPICSMagnetInterface::retrieveUpdateFunctiOnForRecord(pvStruct &pvStruct) const
{
	if (pvStruct.pvRecord == "GETSETI")
	{
		pvStruct.updateFunctiOn = this->updateGETSETI;
	}
	if (pvStruct.pvRecord == "RPOWER")
	{
		pvStruct.updateFunctiOn = this->updatePSUState;
	}
	if (pvStruct.pvRecord == "READI")
	{
		pvStruct.updateFunctiOn = this->updateREADI;
	}
	if (pvStruct.pvRecord == "RILK")
	{
		pvStruct.updateFunctiOn = this->updateRILK;
	}
}


// TODO rename 
//void EPICSMagnetInterface::updateCurrent(const struct event_handler_args args)
void EPICSMagnetInterface::updateGETSETI(const struct event_handler_args args)
{
	Magnet* recastMagnet = getHardwareFromArgs<Magnet>(args);
	
	updateTimeStampDoublePair(args, recastMagnet->GETSETI2);
	
	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("GETSETI"), args);
	//double value = returnValueFromArgsAsDouble(args);
	//recastMagnet->offlineSETI(value);
	messenger.printDebugMessage("GETSETI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ std::to_string(recastMagnet->GETSETI2.second));
}

// TODO rename 
void EPICSMagnetInterface::updatePSUState(const struct event_handler_args args)
{
	Magnet* recastMagnet = getHardwareFromArgs<Magnet>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampIntPair(args);
	recastMagnet->psuState2.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: recastMagnet->psuState2.second = STATE::OFF; break;
	case GlobalConstants::one_int:  recastMagnet->psuState2.second = STATE::ON; break;
	default:
		recastMagnet->psuState2.second = STATE::ERR;
	}
	//psuState2.second = tv->value;
	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("RPOWER"), args);
	//STATE value = returnValueFromArgsAsState(args);
	//recastMagnet->setPSUState(value);
	messenger.printDebugMessage("RPOWER VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ ENUM_TO_STRING(recastMagnet->psuState2.second));
}

void EPICSMagnetInterface::updateREADI(const struct event_handler_args args)
{
	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	updateTimeStampDoublePair(args, recastMagnet->READI2);

	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("READI"), args);
	//double value = returnValueFromArgsAsDouble(args);
	//recastMagnet->READI2 = returnTSVFromArgsDouble(args);
	messenger.printDebugMessage("READI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
														+ std::to_string(recastMagnet->READI2.second));

}

void EPICSMagnetInterface::updateRILK(const struct event_handler_args args)
{
	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampIntPair(args);
	recastMagnet->ilkState2.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: recastMagnet->ilkState2.second = STATE::OK; break;
	case GlobalConstants::one_int:  recastMagnet->ilkState2.second = STATE::ERR; break;
	default:
		recastMagnet->ilkState2.second = STATE::ERR;
	}
	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("RILK"), args);
	//STATE value = returnValueFromArgsAsState(args);
	//recastMagnet->setILKState(value);
	messenger.printDebugMessage("RILK VALUE FOR: " + recastMagnet->getHardwareName() + ": "
								+ ENUM_TO_STRING(recastMagnet->ilkState2.second));
}





// TODO rename 
bool EPICSMagnetInterface::setNewCurrent(const double &value, const pvStruct &pv) const
{
	//we have checked that pvRecord is SETI before reaching here.
	return putValue2(pv, value);;
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
