#include "EPICSInterface.h"
#include <exception>
#include <iostream>
#include <cstring>

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
	messenger.debugMessagesOff();
	messenger.printDebugMessage("EPICSInterface Destructor Called");
}

void EPICSInterface::removeSubscription(pvStruct& pvStruct)
{
	int status = ca_clear_subscription(pvStruct.EVID);
	MY_SEVCHK(status);
}

void EPICSInterface::removeChannel(pvStruct& pvStruct)
{
	int status = ca_clear_channel(pvStruct.CHID);
	MY_SEVCHK(status);
}

void EPICSInterface::detachFromContext()
{
	ca_detach_context();
}

void EPICSInterface::createSubscription(Hardware& hardware, pvStruct& pvStruct) const
{
	int status = ca_create_subscription(pvStruct.MonitorCHTYPE, pvStruct.COUNT,
										pvStruct.CHID, pvStruct.MASK,
										pvStruct.updateFunction,
										(void*)&hardware, 
										&pvStruct.EVID);
	MY_SEVCHK(status);
}

void EPICSInterface::retrieveCHID(pvStruct &pvStruct) const
{
	try
	{
		int status;
		chid CHID;
		std::string pv = pvStruct.fullPVName + ":" + pvStruct.pvRecord;
		status = ca_create_channel(pv.c_str(), NULL, NULL, CA_PRIORITY_DEFAULT, &CHID);
		MY_SEVCHK(status);
		status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		pvStruct.CHID = CHID;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}
void EPICSInterface::retrieveCHTYPE(pvStruct &pvStruct) const
{
	if (pvStruct.monitor)
	{
		if (ca_field_type(pvStruct.CHID) == DBR_DOUBLE)
		{
			pvStruct.MonitorCHTYPE = DBR_TIME_DOUBLE;
		}
		else if (ca_field_type(pvStruct.CHID) == DBR_ENUM)
		{
			pvStruct.MonitorCHTYPE = DBR_TIME_ENUM;
		}
		else
		{
			pvStruct.MonitorCHTYPE = ca_field_type(pvStruct.CHID);
		}
	}
	pvStruct.CHTYPE = ca_field_type(pvStruct.CHID);
}

void EPICSInterface::retrieveCOUNT(pvStruct &pvStruct) const
{
	pvStruct.COUNT = ca_element_count(pvStruct.CHID);
}

std::string EPICSInterface::getEPICSTime(const epicsTimeStamp& stamp)
{
	char timeString[36];
	epicsTimeToStrftime(timeString, sizeof(timeString), "%a %b %d %Y %H:%M:%S.%f", &stamp);
	return timeString;
}

void EPICSInterface::setPVTimeStampFromArgs(pvStruct& pv, const event_handler_args args)
{
	const struct dbr_time_double* time = (const struct dbr_time_double*)(args.dbr);
	pv.time = time->stamp;
}

std::string EPICSInterface::returnValueFromArgsAsString(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_string*)(args.dbr);
	return std::string(timeObject->value);
}

double EPICSInterface::returnValueFromArgsAsDouble(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_double*)(args.dbr);
	return double(timeObject->value);
}

STATE EPICSInterface::returnValueFromArgsAsState(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_enum*)(args.dbr);
	return STATE(timeObject->value);
}

long EPICSInterface::returnValueFromArgsAsLong(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_long*)(args.dbr);
	return long(timeObject->value);
}

float EPICSInterface::returnValueFromArgsAsFloat(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_float*)(args.dbr);
	return float(timeObject->value);
}

