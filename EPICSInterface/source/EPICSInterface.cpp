#include "EPICSInterface.h"
#include <exception>
#include <iostream>
#include <cstring>

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
	{							\
		ca_message(status);		\
		SEVCHK(status, NULL);   \
		exit(status);			\
	}							\
}								\

EPICSInterface::EPICSInterface()
{
	int status;
	messenger = LoggingSystem(false, false);
	if (!ca_current_context())
	{
		status = ca_context_create(ca_enable_preemptive_callback);
		MY_SEVCHK(status);
	}

	thisCaContext = ca_current_context();
}
EPICSInterface::EPICSInterface(const bool& startEpics, const bool& startVirtualMachine)
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	EPICSInterface::messenger = LoggingSystem(false, false);
}

EPICSInterface::EPICSInterface(const bool& startEpics, const bool& startVirtualMachine, LoggingSystem& messager)
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	EPICSInterface::messenger = messager;
}
EPICSInterface::~EPICSInterface()
{
	messenger.messagesOn();
	messenger.printMessage("EPICSInterface Destructor Called");
}
void EPICSInterface::createSubscription(Hardware& hardware, pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == "GETSETI" ||
		pvStruct.pvRecord == "RPOWER" || 
		pvStruct.pvRecord == "READI" ||
		pvStruct.pvRecord == "RILK")//since GETSETI is the only PV with a proper updateFunction, 
						 // we skip over any with NULL updateFunctions for now.
	{
		int status = ca_create_subscription(pvStruct.CHTYPE, pvStruct.COUNT,
											pvStruct.CHID, pvStruct.MASK,
											pvStruct.updateFunction,
											(void*)&hardware, 
								            &pvStruct.EVID);
		MY_SEVCHK(status);
	}
}

void EPICSInterface::retrieveCHID(pvStruct &pvStruct) const
{
	try
	{
		int status;
		chid CHID;
		std::string pv = pvStruct.fullPVName + ":" + pvStruct.pvRecord;
		char *pvCstr = new char[pv.size() +1];
		strcpy(pvCstr,pv.c_str());
		status = ca_create_channel(pvCstr, NULL, NULL, CA_PRIORITY_DEFAULT, &CHID);
		MY_SEVCHK(status);
		status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		pvStruct.CHID = CHID;
		delete pvCstr;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}
void EPICSInterface::retrieveCHTYPE(pvStruct &pvStruct) const
{
	pvStruct.CHTYPE = ca_field_type(pvStruct.CHID);
}

void EPICSInterface::retrieveCOUNT(pvStruct &pvStruct) const
{
	pvStruct.COUNT = ca_element_count(pvStruct.CHID);
}

