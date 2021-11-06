#include "EPICSScreenInterface.h"

std::mutex EPICSScreenInterface::screen_is_moving_mtx;           // mutex for critical section


LoggingSystem EPICSScreenInterface::static_messenger;


EPICSScreenInterface::EPICSScreenInterface() : EPICSInterface()
{
	this->static_messenger = LoggingSystem(true, true);
	static_messenger.printDebugMessage("CONSTRUCTING");
}
EPICSScreenInterface::~EPICSScreenInterface()
{
	static_messenger.printDebugMessage("EPICSScreenInterface Destructor Called");
}
void EPICSScreenInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == ScreenRecords::H_SDEV || pvStruct.pvRecord == ScreenRecords::V_SDEV || pvStruct.pvRecord == ScreenRecords::SDEV)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
			case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHSDEV; break;
			case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVSDEV; break;
			case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateSDEV; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_TRIGGER || pvStruct.pvRecord == ScreenRecords::V_TRIGGER || pvStruct.pvRecord == ScreenRecords::TRIGGER)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
			case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHTRIGGER; break;
			case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVTRIGGER; break;
			case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateTRIGGER; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_EX || pvStruct.pvRecord == ScreenRecords::V_EX)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHEX; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVEX; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_TGTPOS || pvStruct.pvRecord == ScreenRecords::V_TGTPOS )
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHTGTPOS; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVTGTPOS; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_JOG || pvStruct.pvRecord == ScreenRecords::V_JOG)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHJOG; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVJOG; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_JDIFF || pvStruct.pvRecord == ScreenRecords::V_JDIFF)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHJDIFF; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVJDIFF; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_READY || pvStruct.pvRecord == ScreenRecords::V_READY || pvStruct.pvRecord == ScreenRecords::READY)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHREADY; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVREADY; break;
		case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateREADY; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_GETDEV || pvStruct.pvRecord == ScreenRecords::V_GETDEV || pvStruct.pvRecord == ScreenRecords::GETDEV)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
			case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHGETDEV; break;
			case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVGETDEV; break;
			case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateGETDEV; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_DEVSTATE || pvStruct.pvRecord == ScreenRecords::V_DEVSTATE || pvStruct.pvRecord == ScreenRecords::DEVSTATE)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHDEVSTATE; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVDEVSTATE; break;
		case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateDEVSTATE; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_MAXPOS || pvStruct.pvRecord == ScreenRecords::V_MAXPOS || pvStruct.pvRecord == ScreenRecords::MAXPOS)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHMAXPOS; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVMAXPOS; break;
		case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateMAXPOS; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_DEVCENT || pvStruct.pvRecord == ScreenRecords::V_DEVCENT || pvStruct.pvRecord == ScreenRecords::DEVCENT)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHDEVCENT; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVDEVCENT; break;
		case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateDEVCENT; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_MOVING || pvStruct.pvRecord == ScreenRecords::V_MOVING || pvStruct.pvRecord == ScreenRecords::MOVING)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHMOVING; break;
		case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVMOVING; break;
		case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateMOVING; break;
		}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_ACTPOS || pvStruct.pvRecord == ScreenRecords::V_ACTPOS)
	{
	switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
	{
	case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHACTPOS; break;
	case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVACTPOS; break;
	case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateACTPOS; break;
	}
	}
	else if (pvStruct.pvRecord == ScreenRecords::H_EN || pvStruct.pvRecord == ScreenRecords::V_EN)
	{
	switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
	{
	case TYPE::HORIZONTAL: pvStruct.updateFunction = this->updateHEN; break;
	case TYPE::VERTICAL: pvStruct.updateFunction = this->updateVEN; break;
	case TYPE::PNEUMATIC: pvStruct.updateFunction = this->updateEN; break;
	}
	}
	else
	{
	static_messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSScreenInterface::updateHGETDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenSetStateH.first = pairToUpdate.first;
	recastScreen->screenSetStateH.second = ScreenRecords::h_device_to_state_map.at(pairToUpdate.second);
	recastScreen->screenSetState.second = ScreenRecords::h_device_to_state_map.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateHGETDEV SCREEN SET STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenSetStateH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVGETDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenSetStateV.first = pairToUpdate.first;
	recastScreen->screenSetStateV.second = ScreenRecords::v_device_to_state_map.at(pairToUpdate.second);
	recastScreen->screenSetState.second = ScreenRecords::v_device_to_state_map.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateVGETDEV SCREEN SET STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenSetStateV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateGETDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = ScreenRecords::p_device_to_state_map.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateGETDEV SCREEN SET STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenSetState.second));
}

void EPICSScreenInterface::updateHDEVSTATE(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenStateH.first = pairToUpdate.first;
	recastScreen->screenStateH.second = ScreenRecords::screenHElementMap.at(pairToUpdate.second);
	recastScreen->screenState.second = ScreenRecords::screenHElementMap.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateHDEVSTATE SCREEN STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenStateH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVDEVSTATE(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenStateV.first = pairToUpdate.first;
	recastScreen->screenStateV.second = ScreenRecords::screenVElementMap.at(pairToUpdate.second);
	recastScreen->screenState.second = ScreenRecords::screenVElementMap.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateVDEVSTATE SCREEN STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenStateV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateDEVSTATE(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenState.first = pairToUpdate.first;
	recastScreen->screenState.second = ScreenRecords::screenPElementMap.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateDEVSTATE STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenState.second));
}

void EPICSScreenInterface::updateHACTPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->actposH.first = pairToUpdate.first;
	recastScreen->actposH.second = pairToUpdate.second;
	recastScreen->actpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHACTPOS ACTPOS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->actposH.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateVACTPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->actposV.first = pairToUpdate.first;
	recastScreen->actposV.second = pairToUpdate.second;
	recastScreen->actpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVACTPOS ACTPOS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->actposV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateACTPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->actpos.first = pairToUpdate.first;
	recastScreen->actpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateACTPOS ACTPOS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->actpos.second));
}

void EPICSScreenInterface::updateHTGTPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->tgtposH.first = pairToUpdate.first;
	recastScreen->tgtposH.second = pairToUpdate.second;
	recastScreen->tgtpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHTGTPOS TGTPOS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->tgtposH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVTGTPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->tgtposV.first = pairToUpdate.first;
	recastScreen->tgtposV.second = pairToUpdate.second;
	recastScreen->tgtpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVTGTPOS TGTPOS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->tgtposV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateHDEVCENT(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->devcentH.first = pairToUpdate.first;
	recastScreen->devcentH.second = pairToUpdate.second;
	recastScreen->devcent.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHDEVCENT DEV_CENT FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->devcentH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVDEVCENT(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->devcentV.first = pairToUpdate.first;
	recastScreen->devcentV.second = pairToUpdate.second;
	recastScreen->devcent.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVDEVCENT DEV_CENT FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->devcentV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateDEVCENT(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->devcent.first = pairToUpdate.first;
	recastScreen->devcent.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateDEVCENT DEV_CENT FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->devcent.second));
}

void EPICSScreenInterface::updateHJDIFF(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->jdiffH.first = pairToUpdate.first;
	recastScreen->jdiffH.second = pairToUpdate.second;
	recastScreen->jdiff.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHJDIFF JDIFF FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->jdiffH.second));
}

void EPICSScreenInterface::updateVJDIFF(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->jdiffV.first = pairToUpdate.first;
	recastScreen->jdiffV.second = pairToUpdate.second;
	recastScreen->jdiff.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVJDIFF JDIFF FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->jdiffV.second));
}

void EPICSScreenInterface::updateHJOG(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->jogH.first = pairToUpdate.first;
	recastScreen->jogH.second = pairToUpdate.second;
	recastScreen->jog.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHJOG JOG FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->jogH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVJOG(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->jogV.first = pairToUpdate.first;
	recastScreen->jogV.second = pairToUpdate.second;
	recastScreen->jog.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVJOG JOG FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->jogV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateHEN(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->enH.first = pairToUpdate.first;
	recastScreen->enH.second = pairToUpdate.second;
	recastScreen->en.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHEN EN FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->enH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVEN(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->enV.first = pairToUpdate.first;
	recastScreen->enV.second = pairToUpdate.second;
	recastScreen->en.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVEN EN FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->enV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateEN(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->en.first = pairToUpdate.first;
	recastScreen->en.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateEN EN FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->en.second));
}

void EPICSScreenInterface::updateHTRIGGER(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->triggerH.first = pairToUpdate.first;;
	recastScreen->triggerH.second = pairToUpdate.second;
	recastScreen->trigger.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHTRIGGER TRIGGER FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->triggerH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVTRIGGER(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->triggerV.first = pairToUpdate.first;
	recastScreen->triggerV.second = pairToUpdate.second;
	recastScreen->trigger.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVTRIGGER TRIGGER FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->triggerV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateTRIGGER(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->trigger.first = pairToUpdate.first;
	recastScreen->trigger.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateTRIGGER TRIGGER FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->trigger.second));
}

void EPICSScreenInterface::updateHEX(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->exH.first = pairToUpdate.first;
	recastScreen->exH.second = pairToUpdate.second;
	recastScreen->ex.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHEX EX FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->exH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVEX(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->exV.first = pairToUpdate.first;
	recastScreen->exV.second = pairToUpdate.second;
	recastScreen->ex.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVEX EX FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->exV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateEX(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->ex.first = pairToUpdate.first;
	recastScreen->ex.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateEX EX FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->ex.second));
}

void EPICSScreenInterface::updateHREADY(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->readyH.first = pairToUpdate.first;
	recastScreen->readyH.second = pairToUpdate.second;
	recastScreen->ready.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHREADY READY FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->readyH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVREADY(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->readyV.first = pairToUpdate.first;
	recastScreen->readyV.second = pairToUpdate.second;
	recastScreen->ready.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVREADY READY FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->readyV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateREADY(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->ready.first = pairToUpdate.first;
	recastScreen->ready.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateREADY READY FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->ready.second));
}

void EPICSScreenInterface::updateHMOVING(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->movingH.first = pairToUpdate.first;
	recastScreen->movingH.second = pairToUpdate.second;
	recastScreen->moving.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHMOVING MOVING FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->movingH.second) + " IN HORIZONTAL DIRECTION");
	recastScreen->updateIsmoving();
}

void EPICSScreenInterface::updateVMOVING(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->movingV.first = pairToUpdate.first;
	recastScreen->movingV.second = pairToUpdate.second;
	recastScreen->moving.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVMOVING MOVING FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->movingV.second) + " IN VERTICAL DIRECTION");
	recastScreen->updateIsmoving();
}

void EPICSScreenInterface::updateMOVING(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->moving.first = pairToUpdate.first;
	recastScreen->moving.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateMOVING MOVING FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->moving.second));
	recastScreen->updateIsmoving();
}

void EPICSScreenInterface::updateHMAXPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->maxposH.first = pairToUpdate.first;
	recastScreen->maxposH.second = pairToUpdate.second;
	recastScreen->maxpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateHMAXPOS MAX_POS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->maxposH.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSScreenInterface::updateVMAXPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->maxposV.first = pairToUpdate.first;
	recastScreen->maxposV.second = pairToUpdate.second;
	recastScreen->maxpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateVMAXPOS MAX_POS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->maxposV.second) + " IN VERTICAL DIRECTION");
}

void EPICSScreenInterface::updateMAXPOS(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, double> pairToUpdate = getTimeStampDoublePair(args);
	recastScreen->maxpos.first = pairToUpdate.first;
	recastScreen->maxpos.second = pairToUpdate.second;
	static_messenger.printDebugMessage("updateMAXPOS MAX_POS FOR: " + recastScreen->getHardwareName() + ": "
		+ std::to_string(recastScreen->maxpos.second));
}

void EPICSScreenInterface::updateHSDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetStateH.second = ScreenRecords::screenHElementMap.at(pairToUpdate.second);
	recastScreen->screenSetState.second = ScreenRecords::screenHElementMap.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateHSDEV SCREEN STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenSetState.second) + " IN HORIZONTAL DIRECTION");
	static_messenger.printDebugMessage("pairToUpdate.second = ", pairToUpdate.second);
}

void EPICSScreenInterface::updateVSDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetStateV.second = ScreenRecords::screenVElementMap.at(pairToUpdate.second);
	recastScreen->screenSetState.second = ScreenRecords::screenVElementMap.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateVSDEV SCREEN STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenSetState.second) + " IN HORIZONTAL DIRECTION");
	static_messenger.printDebugMessage("pairToUpdate.second = ", pairToUpdate.second);
}

void EPICSScreenInterface::updateSDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = ScreenRecords::screenPElementMap.at(pairToUpdate.second);
	static_messenger.printDebugMessage("updateSDEV SCREEN STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenSetState.second));
	static_messenger.printDebugMessage("pairToUpdate.second = ", pairToUpdate.second);
}
void EPICSScreenInterface::updateSTA(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;

	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_us:	recastScreen->screenState.second = STATE::RF;	break;
	case GlobalConstants::one_us:	recastScreen->screenState.second = STATE::YAG;	break;
	default: recastScreen->screenState.second = STATE::UNKNOWN_STATE;
	}
	static_messenger.printDebugMessage("updateSTA SCREEN STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastScreen->screenState.second));
}


bool EPICSScreenInterface::setSDEV(const int value, const pvStruct& pv)
{
	unsigned short us = (unsigned short)value;
	return putValue2<unsigned short>(pv, us);
}

bool EPICSScreenInterface::setTRIGGER(const int& value, const pvStruct& pv)
{
	double val = (double)value;
	return putValue2<double>(pv, val);
}

bool EPICSScreenInterface::setEX(const int& value, const pvStruct& pv)
{
	return putValue2<unsigned short>(pv, (unsigned short)value);
}

bool EPICSScreenInterface::setEN(const int& value, const pvStruct& pv)
{
	return putValue2<unsigned short>(pv, (unsigned short)value);
}

bool EPICSScreenInterface::setJOG(const double& value, const pvStruct& pv)
{
	return putValue2<unsigned short>(pv, (unsigned short)value);
}

bool EPICSScreenInterface::setJDIFF(const double& value, const pvStruct& pv)
{
	return 	putValue2<double>(pv, value);
}

bool EPICSScreenInterface::setTGTPOS(const double& value, const pvStruct& pv)
{
	return putValue2<double>(pv, value);
}


bool EPICSScreenInterface::setIN(const pvStruct& pv)
{
	if (putValue2<unsigned short>(pv, GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));//MAGIC_NUMBER!
		return putValue2<unsigned short>(pv, GlobalConstants::EPICS_SEND);
	}
	return false;
}


bool EPICSScreenInterface::setOUT(const pvStruct& pv)
{
	if (putValue2<unsigned short>(pv, GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));//MAGIC_NUMBER!
		return putValue2<unsigned short>(pv, GlobalConstants::EPICS_SEND);
	}
	return false;
}