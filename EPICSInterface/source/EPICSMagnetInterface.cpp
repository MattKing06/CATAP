#include "EPICSMagnetInterface.h"

double EPICSMagnetInterface::latestCurrentFromEPICS = std::numeric_limits<double>::min();

EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{
}

void EPICSMagnetInterface::updateValue(const event_handler_args args)
{
	//int status;
	//double current;
	//ca_task_initialize();
	////ca_get(DBR_DOUBLE, channelID, &current);
	//status = ca_pend_io(1.0);
	//messaging.printMessage("IN EPICS MAG INTERFACE: current = " + std::to_string(current));
	//SEVCHK(status, "ca_pend_io failed");
	//return current

	/*THIS IS WHERE TELL EPICS HOW TO UPDATE THE CURRENT, i.e. mag->current = args.dbr; */
	pvStruct* magnetPVStruct = static_cast<pvStruct*> (args.usr);
	if (magnetPVStruct->pvRecord == "READI")
	{
		//HOW WILL WE LET A MAGNET OBJECT ACCESS THIS???
		// NEED TO FIND A WAY TO LINK THIS TO MAGNET current MEMBER VARIABLE...
		EPICSMagnetInterface::updateCurrent(args);
	}
}

void EPICSMagnetInterface::updateCurrent(const event_handler_args args)
{
	EPICSMagnetInterface::latestCurrentFromEPICS = *(double *)(args.dbr);
}