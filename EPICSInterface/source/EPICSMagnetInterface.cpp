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
EPICSMagnetInterface::~EPICSMagnetInterface()
{
	messenger.messagesOn();
	messenger.printMessage("EPICSMagnetInterface Destructor Called");
}
void EPICSMagnetInterface::retrieveUpdateFunctionForRecord(pvStruct &pvStruct) const
{
	if (pvStruct.pvRecord == "GETSETI")
	{
		pvStruct.updateFunction = this->updateCurrent;
	}
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
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		messenger.printMessage(recastMagnet->getHardwareName());
		recastMagnet->setCurrent(*(double*)(args.dbr));
		messenger.printMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": "  + std::to_string(*(double*)(args.dbr)));
	}
	messenger.printMessage(" CALLED UPDATE CURRENT ");

}

void EPICSMagnetInterface::setNewCurrent(const double &value, const pvStruct &pv) const
{
	//we have checked that pvRecord is SETI before reaching here.
	putValue(pv, value);
	messenger.printMessage("SENT CURRENT " + std::to_string(value) + " FOR " + pv.fullPVName + " TO EPICS ");
}

template<typename T>
void EPICSMagnetInterface::putValue(const pvStruct& pvStruct, const T& value) const
{
	if (ca_state(pvStruct.CHID) == cs_conn)
	{
		int status = ca_put(pvStruct.CHTYPE, pvStruct.CHID, &value);
		MY_SEVCHK(status);
		status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		MY_SEVCHK(status);
	}
	else
	{
		std::cout << "NOT CONNECTED TO EPICS" << std::endl;
	}
}