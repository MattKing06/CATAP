#include <EPICSIMGInterface.h>

LoggingSystem EPICSIMGInterface::static_messenger;

EPICSIMGInterface::EPICSIMGInterface() : EPICSInterface()
{
	static_messenger = LoggingSystem(false, false);
}

EPICSIMGInterface::~EPICSIMGInterface()
{
	static_messenger.printDebugMessage("EPICSIMGInterface Destructor Called");
}
