#include "EPICSScreeenInterface.h"

LoggingSystem EPICSScreeenInterface::messenger;

EPICSScreeenInterface::EPICSScreeenInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSScreeenInterface::~EPICSScreeenInterface()
{
	messenger.printDebugMessage("EPICSScreeenInterface Destructor Called");
}
void EPICSScreeenInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == ScreeenRecords::HSDEV || pvStruct.pvRecord == ScreeenRecords::VSDEV || pvStruct.pvRecord == ScreeenRecords::SDEV)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
			case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHSDEV; break;
			case STATE::VERTICAL: pvStruct.updateFunction = this->updateVSDEV; break;
			case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateSDEV; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HTRIGGER || pvStruct.pvRecord == ScreeenRecords::VTRIGGER || pvStruct.pvRecord == ScreeenRecords::TRIGGER)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
			case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHTRIGGER; break;
			case STATE::VERTICAL: pvStruct.updateFunction = this->updateVTRIGGER; break;
			case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateTRIGGER; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HEX || pvStruct.pvRecord == ScreeenRecords::VEX || pvStruct.pvRecord == ScreeenRecords::EX)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHEX; break;
		case STATE::VERTICAL: pvStruct.updateFunction = this->updateVEX; break;
		case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateEX; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HTGTPOS || pvStruct.pvRecord == ScreeenRecords::HTGTPOS|| pvStruct.pvRecord == ScreeenRecords::TGTPOS)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHTGTPOS; break;
		case STATE::VERTICAL: pvStruct.updateFunction = this->updateVTGTPOS; break;
		case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateTGTPOS; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HJOG || pvStruct.pvRecord == ScreeenRecords::VJOG || pvStruct.pvRecord == ScreeenRecords::JOG)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHJOG; break;
		case STATE::VERTICAL: pvStruct.updateFunction = this->updateVJOG; break;
		case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateJOG; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HJDIFF || pvStruct.pvRecord == ScreeenRecords::VJDIFF || pvStruct.pvRecord == ScreeenRecords::JDIFF)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHDIFF; break;
		case STATE::VERTICAL: pvStruct.updateFunction = this->updateVDIFF; break;
		case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateDIFF; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HREADY || pvStruct.pvRecord == ScreeenRecords::VREADY || pvStruct.pvRecord == ScreeenRecords::READY)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
		case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHTRIGGER; break;
		case STATE::VERTICAL: pvStruct.updateFunction = this->updateVTRIGGER; break;
		case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateTRIGGER; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HGETDEV || pvStruct.pvRecord == ScreeenRecords::VGETDEV || pvStruct.pvRecord == ScreeenRecords::GETDEV)
	{
		switch (ScreenRecords::screenDirectionList.at(pvStruct.pvRecord))
		{
			case STATE::HORIZONTAL: pvStruct.updateFunction = this->updateHGETDEV; break;
			case STATE::VERTICAL: pvStruct.updateFunction = this->updateVGETDEV; break;
			case STATE::PNEUMATIC: pvStruct.updateFunction = this->updateGETDEV; break;
		}
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HDEVSTATE || pvStruct.pvRecord == ScreeenRecords::VDEVSTATE || pvStruct.pvRecord == ScreeenRecords::DEVSTATE)
	{
		pvStruct.updateFunction = this->updateDEVSTATE;
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HMAXPOS || pvStruct.pvRecord == ScreeenRecords::VMAXPOS || pvStruct.pvRecord == ScreeenRecords::MAXPOS)
	{
		pvStruct.updateFunction = this->updateMAXPOS;
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HDEVCENT || pvStruct.pvRecord == ScreeenRecords::VDEVCENT || pvStruct.pvRecord == ScreeenRecords::DEVCENT)
	{
		pvStruct.updateFunction = this->updateDEVCENT;
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HACTPOS || pvStruct.pvRecord == ScreeenRecords::VACTPOS || pvStruct.pvRecord == ScreeenRecords::ACTPOS)
	{
		pvStruct.updateFunction = this->updateACTPOS;
	}
	else if (pvStruct.pvRecord == ScreeenRecords::HEN || pvStruct.pvRecord == ScreeenRecords::VEN || pvStruct.pvRecord == ScreeenRecords::EN)
	{
		pvStruct.updateFunction = this->updateEN;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSValveInterface::updateHGETDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, enum> pairToUpdate = getTimeStampEnumPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = pairToUpdate.second;
	static_messenger.printDebugMessage("SCREEN STATE FOR: " + recastScreen->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->screenSetState.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSValveInterface::updateVGETDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, enum> pairToUpdate = getTimeStampEnumPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = pairToUpdate.second;
	static_messenger.printDebugMessage("SCREEN STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->screenSetState.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSValveInterface::updateGETDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, enum> pairToUpdate = getTimeStampEnumPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = pairToUpdate.second;
	static_messenger.printDebugMessage("SCREEN STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->screenSetState.second));
}

void EPICSValveInterface::updateHSDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, enum> pairToUpdate = getTimeStampEnumPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = pairToUpdate.second;
	static_messenger.printDebugMessage("SCREEN STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->screenSetState.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSValveInterface::updateVSDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, enum> pairToUpdate = getTimeStampEnumPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = pairToUpdate.second;
	static_messenger.printDebugMessage("SCREEN STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->screenSetState.second) + " IN HORIZONTAL DIRECTION");
}

void EPICSValveInterface::updateSDEV(const struct event_handler_args args)
{
	Screen* recastScreen = getHardwareFromArgs<Screen>(args);
	std::pair<epicsTimeStamp, enum> pairToUpdate = getTimeStampEnumPair(args);
	recastScreen->screenSetState.first = pairToUpdate.first;
	recastScreen->screenSetState.second = pairToUpdate.second;
	static_messenger.printDebugMessage("SCREEN STATE FOR: " + recastValve->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastValve->screenSetState.second));
}

void EPICSScreeenInterface::updateXPV(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampDoublePair(args, recastScreeen->xPV);
	recastScreeen->setXPV(recastScreeen->xPV.second);
	messenger.printDebugMessage("XPV VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->xPV.second));
}

void EPICSScreeenInterface::updateYPV(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampDoublePair(args, recastScreeen->yPV);
	recastScreeen->setYPV(recastScreeen->yPV.second);
	messenger.printDebugMessage("YPV VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->yPV.second));
}

void EPICSScreeenInterface::updateData(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	/*const struct dbr_time_double* tv = (const struct dbr_time_double*)(args.dbr);
	recastScreeen->data.first = tv->stamp;*/
	updateTimeStampDoubleVectorPair(args, recastScreeen->data, args.count);
	recastScreeen->setData(recastScreeen->data.second);
	messenger.printDebugMessage("DATA PV VALUE FOR: " + recastScreeen->getHardwareName());
	messenger.printDebugMessage(" CALLED UPDATE DATA ");
}

void EPICSScreeenInterface::updateRA1(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->ra1);
	messenger.printDebugMessage("RA1 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->ra1.second));
}

void EPICSScreeenInterface::updateRA2(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->ra2);
	messenger.printMessage("RA2 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->ra2.second));
}

void EPICSScreeenInterface::updateRD1(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->rd1);
	messenger.printDebugMessage("RD1 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->rd1.second));
}

void EPICSScreeenInterface::updateRD2(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->rd2);
	messenger.printDebugMessage("RD2 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->rd1.second));
}

void EPICSScreeenInterface::updateSA1(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->sa1);
	messenger.printDebugMessage("SA1 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->sa1.second));
}

void EPICSScreeenInterface::updateSA2(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->sa2);
	messenger.printDebugMessage("SA2 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->sa2.second));
}

void EPICSScreeenInterface::updateSD1(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->sd1);
	messenger.printDebugMessage("SD1 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->sd1.second));
}

void EPICSScreeenInterface::updateSD2(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampLongPair(args, recastScreeen->sd2);
	messenger.printDebugMessage("SD2 VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->sd2.second));
}

void EPICSScreeenInterface::updateAWAK(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampDoublePair(args, recastScreeen->awak);
	messenger.printDebugMessage("AWAK VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->awak.second));
}

void EPICSScreeenInterface::updateRDY(const struct event_handler_args args)
{
	Screeen* recastScreeen = getHardwareFromArgs<Screeen>(args);
	updateTimeStampDoublePair(args, recastScreeen->rdy);
	messenger.printDebugMessage("RDY VALUE FOR: " + recastScreeen->getHardwareName() + ": "
		+ std::to_string(recastScreeen->rdy.second));
}

void EPICSScreeenInterface::setSA1(const long& value, const pvStruct& pv)
{
	putValue2(pv, value);
}

void EPICSScreeenInterface::setSA2(const long& value, const pvStruct& pv)
{
	putValue2(pv, value);
}

void EPICSScreeenInterface::setSD1(const long& value, const pvStruct& pv)
{
	putValue2(pv, value);
}

void EPICSScreeenInterface::setSD2(const long& value, const pvStruct& pv)
{
	putValue2(pv, value);
}