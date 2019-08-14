#include "EPICSMagnetInterface.h"

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
		{						\
		SEVCHK(status, NULL);	\
		exit(status);			\
		}						\
}								\

LoggingSystem EPICSMagnetInterface::messenger;

EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
void EPICSMagnetInterface::updateCurrent(const struct event_handler_args args)
{
	messenger.messagesOn();
	if (args.status != ECA_NORMAL)
	{
		std::cerr << "Something went wrong with update function!" << std::endl;
	}
	else if (args.type == DBR_DOUBLE)
	{
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		messenger.printMessage(recastMagnet->getFullPVName());
		recastMagnet->setCurrent(*(double*)(args.dbr));
		messenger.printMessage("GETSETI VALUE: " + std::to_string(*(double*)(args.dbr)));
	}
	messenger.printMessage(" CALLED UPDATE CURRENT ");

}

void EPICSMagnetInterface::setNewCurrent(double value, pvStruct pv)
{
	//we have checked that pvRecord is SETI before reaching here.
	if (ca_state(pv.CHID) == cs_conn)
	{
		int status = ca_put(pv.CHTYPE, pv.CHID, &(value));
		status = ca_flush_io();
		MY_SEVCHK(status);
		std::cout << "SENT CURRENT " + std::to_string(value) + " FOR " + pv.fullPVName + " TO EPICS (STATUS = " + std::to_string(status) + ") " << std::endl;
	}
	else
	{
		std::cout << "NOT CONNECTED TO EPICS" << std::endl;
	}
	
}
