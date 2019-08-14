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
EPICSInterface::EPICSInterface(bool& startEpics, bool& startVirtualMachine)
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	EPICSInterface::messenger = LoggingSystem(false, false);
}

EPICSInterface::EPICSInterface(bool& startEpics, bool& startVirtualMachine, LoggingSystem& messager)
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
void EPICSInterface::createSubscription(Hardware& hardware, std::string pvName)
{
	std::map<std::string, pvStruct*> pvList = hardware.getPVStructs();
	pvStruct* pv;
	if (pvName == "GETSETI")//since READI is the only PV with a proper updateFunction, 
						 // we skip over any with NULL updateFunctions for now.
	{
		for (auto currentPV = pvList.begin(); currentPV != pvList.end(); currentPV++)
		{
			pv = currentPV->second;
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
		messenger.printDebugMessage(pvCstr);
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
