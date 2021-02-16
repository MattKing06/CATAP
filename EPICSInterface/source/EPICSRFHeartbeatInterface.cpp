#include <EPICSRFHeartbeatInterface.h>
#include <GlobalFunctions.h>


LoggingSystem EPICSRFHeartbeatInterface::messenger;

EPICSRFHeartbeatInterface::EPICSRFHeartbeatInterface()
{
	messenger = LoggingSystem(true, true);
}
EPICSRFHeartbeatInterface::~EPICSRFHeartbeatInterface()
{
}


void EPICSRFHeartbeatInterface::update_KEEP_ALIVE(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_TRIG_SOURCE");
}

void EPICSRFHeartbeatInterface::retrieveUpdateFunctionForRecord(const pvStruct& pv)const
{
	if (GlobalFunctions::entryExists(updateFunctionMap, pv.pvRecord))
	{
		pv.updateFunction = updateFunctionMap.at(pv.pvRecord);
	}
	else
	{
		//messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pv.pvRecord);
		pv.updateFunction = nullptr;
	}
}
