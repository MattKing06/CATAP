#include <EPICSIMGInterface.h>

//LoggingSystem EPICSIMGInterface::static_messenger;
LoggingSystem EPICSIMGInterface::messenger;

EPICSIMGInterface::EPICSIMGInterface() : EPICSInterface()
{
//	static_messenger = LoggingSystem(false, false)
	messenger = LoggingSystem(true, true);
}

EPICSIMGInterface::~EPICSIMGInterface()
{
	messenger.printDebugMessage("EPICSIMGInterface Destructor Called");
}

void EPICSIMGInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == IMGRecords::Pressure)
	{
		pvStruct.updateFunction = this->updateIMGP;
	}
	else if (pvStruct.pvRecord == IMGRecords::Sta)
	{
		pvStruct.updateFunction = this->updateIMGState;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSIMGInterface::updateIMGP(const struct event_handler_args args)
{
	IMG* recastIMG = static_cast<IMG*>(args.usr);
	updateTimeStampDoublePair(args, recastIMG->pressure);

	messenger.printDebugMessage("Pressure VALUE FOR: " + recastIMG->getHardwareName() + ": "
		+ std::to_string(recastIMG->pressure.second));
}

void EPICSIMGInterface::updateIMGState(const struct event_handler_args args)
{
	IMG* recastIMG = getHardwareFromArgs<IMG>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastIMG->state.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: recastIMG->setIMGState(STATE::OK); break;
	case GlobalConstants::one_int: recastIMG->setIMGState(STATE::ERR); break;
	default:
		recastIMG->setIMGState(STATE::ERR);
	}
	messenger.printDebugMessage("IMG STATE FOR: " + recastIMG->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastIMG->state.second));
}
