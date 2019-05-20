#include "EPICSMagnetInterface.h"
EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{}

double EPICSMagnetInterface::getCurrent(chid &channelID)
{
	int status;
	double current;
	ca_task_initialize();
	ca_get(DBR_DOUBLE, channelID, &current);
	status = ca_pend_io(1.0);
	messaging.printMessage("IN EPICS MAG INTERFACE: current = " + std::to_string(current));
	SEVCHK(status, "ca_pend_io failed");
	return current;
}