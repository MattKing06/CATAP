#include "EPICSMagnetInterface.h"

LoggingSystem EPICSMagnetInterface::messenger;



EPICSMagnetInterface::EPICSMagnetInterface() : 
EPICSInterface()
{
	messenger = LoggingSystem(true, true);
	messenger.printDebugMessage("EPICSMagnetInterface Constructor Called");
}

EPICSMagnetInterface::EPICSMagnetInterface(const EPICSMagnetInterface& copyInterface)
{
	messenger.printDebugMessage("EPICSMagnetInterface Copy Constructor Called");
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
	//messenger.printDebugMessage("EPICS_UPDATE GETSETI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
	//													+ std::to_string(recastMagnet->GETSETI.second));
}

// TODO rename 
void EPICSMagnetInterface::updatePSUState(const struct event_handler_args args)
{
	// TODO: this seems to get called twice on startup, often the first time the number returned is totally incorrect, 
	// on the second callback the correct number is returned, maybe some weird EPIPCS thing
	Magnet* recastMagnet = getHardwareFromArgs<Magnet>(args);
	// we get the value and then manually convert to a STATE 
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastMagnet->psuState.first = pairToUpdate.first;
	switch(pairToUpdate.second)
	{
		case GlobalConstants::zero_ushort: recastMagnet->psuState.second = STATE::OFF; break;
		case GlobalConstants::one_ushort:  recastMagnet->psuState.second = STATE::ON; break;
		default: recastMagnet->psuState.second = STATE::ERR;
		//messenger.printDebugMessage("!!WARNING!! EPICS_UPDATE RPOWER VALUE FOR: " + recastMagnet->getHardwareName() + ": ",a);
	}
	//messenger.printDebugMessage("EPICS_UPDATE RPOWER VALUE FOR: " + recastMagnet->getHardwareName() + ": "
	//													+ ENUM_TO_STRING(recastMagnet->psuState.second));
}

void EPICSMagnetInterface::updateREADI(const struct event_handler_args args)
{
	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	updateTimeStampDoublePair(args, recastMagnet->READI);

	//setPVTimeStampFromArgs(recastMagnet->pvStructs.at("READI"), args);
	//double value = returnValueFromArgsAsDouble(args);
	//recastMagnet->READI2 = returnTSVFromArgsDouble(args);

	//if(recastMagnet ->hardwareName == "CLA-C2V-MAG-VCOR-01")
	//{ 
	//	messenger.printDebugMessage("EPICS_UPDATE READI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
	//		+ std::to_string(recastMagnet->READI.second));
	//}

}

void EPICSMagnetInterface::updateRILK(const struct event_handler_args args)
{
	Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampUShortPair(args);
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
	//messenger.printDebugMessage("EPICS_UPDATE RILK VALUE FOR: " + recastMagnet->getHardwareName() + ": "
	//							+ ENUM_TO_STRING(recastMagnet->ilkState.second));
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
	unsigned short epics_value = GlobalConstants::zero_ushort;
	switch (value)
	{
	case STATE::ON: epics_value = GlobalConstants::one_ushort; break;
	case STATE::OFF: epics_value = GlobalConstants::zero_ushort; break;
	}
	return putValue2(pv, epics_value);
}
