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
	EPICSInterface::messaging = LoggingSystem(false, false);
	status = ca_context_create(ca_enable_preemptive_callback);
	if (status != ECA_NORMAL)
	{
		std::printf("ca_context_create failed: \n%s\n", 
			ca_message(status));
		exit(1);
	}
	status = ca_task_initialize();
	SEVCHK(status, "initialize failed");
	thisCaContext = ca_current_context();
}
EPICSInterface::EPICSInterface(bool& startEpics, bool& startVirtualMachine)
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	EPICSInterface::messaging = LoggingSystem(false, false);
}

EPICSInterface::EPICSInterface(bool& startEpics, bool& startVirtualMachine, LoggingSystem& messager)
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	EPICSInterface::messaging = messager;
}

void EPICSInterface::createSubscription(Hardware& hardware, std::string pvName)
{
	std::vector<pvStruct*> pvList = hardware.getPVStructs();
	pvStruct* pv;
	if (pvName == "GETSETI")//since READI is the only PV with a proper updateFunction, 
						 // we skip over any with NULL updateFunctions for now.
	{
		for (auto currentPV = pvList.begin(); currentPV != pvList.end(); currentPV++)
		{
			pv = *(currentPV);
			if (pv->pvRecord == pvName)
			{
				int status = ca_create_subscription(pv->CHTYPE, pv->COUNT, pv->CHID, pv->MASK,
					pv->updateFunction, (void*)&hardware, &pv->EVID);
				MY_SEVCHK(status);
			}
		}
	}
}

chid EPICSInterface::retrieveCHID(std::string &pv)
{
	try
	{
		int status;
		chid CHID;
		char *pvCstr = new char[pv.size() +1];
		strcpy(pvCstr,pv.c_str());
		status = ca_create_channel(pvCstr, NULL, NULL, CA_PRIORITY_DEFAULT, &CHID);
		//std::cout << "CHID FROM EPICS INTERFACE: " << CHID << std::endl;
		MY_SEVCHK(status);
		status = ca_pend_io(1.0);
		messaging.printDebugMessage(pvCstr);
		return CHID;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return NULL;
	}

}
chtype EPICSInterface::retrieveCHTYPE(chid &channelID)
{
	chtype channelType;
	channelType = ca_field_type(channelID);
	return channelType;
}

unsigned long EPICSInterface::retrieveCOUNT(chid &channelID)
{
	unsigned long count;
	count = ca_element_count(channelID);
	return count;
}
