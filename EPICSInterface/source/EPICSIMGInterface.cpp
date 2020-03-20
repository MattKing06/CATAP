#include <EPICSIMGInterface.h>

//LoggingSystem EPICSIMGInterface::static_messenger;

EPICSIMGInterface::EPICSIMGInterface() : EPICSInterface()
{
//	static_messenger = LoggingSystem(false, false)
	messenger = LoggingSystem(true, true);
}

EPICSIMGInterface::~EPICSIMGInterface()
{
	messenger.printDebugMessage("EPICSIMGInterface Destructor Called");
}

void EPICSIMGInterface::updateIMGP(const struct event_handler_args args)
{
	IMG* recastIMG = static_cast<IMG*>(args.usr);
	updateTimeStampDoublePair(args, recastIMG->pressure);

	messenger.printDebugMessage("Pressure VALUE FOR: " + recastIMG->getHardwareName() + ": "
		+ std::to_string(recastIMG->pressure.second));
}
