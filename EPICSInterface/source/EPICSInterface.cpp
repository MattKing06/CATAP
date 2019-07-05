#include "EPICSInterface.h"
#include <exception>
#include <iostream>
#include <cstring>

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
	{							\
		SEVCHK(status, NULL);	\
		exit(status);			\
	}							\
}								\

EPICSInterface::EPICSInterface()
{
	int status;
	EPICSInterface::messaging = LoggingSystem(false, false);
	status = ca_context_create(ca_disable_preemptive_callback);
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
	std::cout << "HARDWARE TYPE: " << hardware.getHardwareType();
	std::vector<pvStruct> *pvList = hardware.getPVStructs();
	pvStruct pv;
	if (pvName == "READI")//since READI is the only PV with a proper updateFunction, 
						 // we skip over any with NULL updateFunctions for now.
	{
		for (auto currentPV = pvList->begin(); currentPV != pvList->end(); currentPV++)
		{
			if (currentPV->pvRecord == pvName)
			{
				pv = *(currentPV);
			}
		}
		if (pv.updateFunction == NULL)
		{
			std::cout << "UPDATE FUNCTION FOR " << pv.pvRecord << " IS NULL" << std::endl;
		}
		int status = ca_create_subscription(pv.CHTYPE, pv.COUNT, pv.CHID, pv.MASK,
			pv.updateFunction, (void*)&hardware, 0);
		MY_SEVCHK(status);
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
		SEVCHK(status, "could not create channel");
		messaging.printDebugMessage(pvCstr);
		return CHID;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
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
