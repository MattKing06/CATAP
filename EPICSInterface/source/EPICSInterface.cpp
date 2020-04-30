#include "EPICSInterface.h"
#include <exception>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>


EPICSInterface::EPICSInterface() : 
thisCaContext(nullptr),
messenger(LoggingSystem(false, false))
{
	int status;
	/* 
		This 'enables' callbacks, monitoring, etc
		thisCaContext is the current (AND ONLY) context,
		use it to join from new threads with ca_attach_context
		you NEED to attach to this context if multi-threading
	*/
	if (!ca_current_context())
	{
		status = ca_context_create(ca_enable_preemptive_callback);
		MY_SEVCHK(status);
	}

	thisCaContext = ca_current_context();
}

EPICSInterface::EPICSInterface(const bool& startEpics, const bool& startVirtualMachine):
messenger(LoggingSystem(true, true))
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
}

EPICSInterface::~EPICSInterface()
{
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

void EPICSInterface::sendToEPICS()
{
	ca_pend_io(CA_PEND_IO_TIMEOUT);
}

void EPICSInterface::attachTo_thisCaContext()
{
	if (thisCaContext)
	{
		ca_attach_context(thisCaContext);
	}
}

void EPICSInterface::createSubscription(Hardware& hardware, pvStruct& pvStruct) const
{
	int status = ca_create_subscription(pvStruct.monitorCHTYPE, pvStruct.COUNT,
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
		//chid CHID;
		// This should eb defeind in the hardware objst, so that we can handle non-standrd PV names
		std::string pv = pvStruct.fullPVName + ":" + pvStruct.pvRecord;
		/*CURRENTLY PV STRUCTS FOR MAGNET CONTAIN FULL PV at pvStruct.FullPVName*/
		if (pvStruct.fullPVName.find("MAG") != std::string::npos ||
			pvStruct.fullPVName.find("VALV") != std::string::npos ||
			pvStruct.fullPVName.find("BPM") != std::string::npos ||
			pvStruct.fullPVName.find("WCM") != std::string::npos ||
			pvStruct.fullPVName.find("FCUP") != std::string::npos ||
			pvStruct.fullPVName.find("SCR") != std::string::npos ||
			pvStruct.fullPVName.find("YAG") != std::string::npos ||
			pvStruct.fullPVName.find("IMG") != std::string::npos )
		{
			pv = pvStruct.fullPVName;
		}
		if (pvStruct.fullPVName.find("VALV") != std::string::npos)
		{
			pv = pvStruct.fullPVName;
		}
		else if (pvStruct.fullPVName.find("IMG") != std::string::npos)
		{
			pv = pvStruct.fullPVName;
		}

		status = ca_create_channel(pv.c_str(), NULL, NULL, CA_PRIORITY_DEFAULT, &pvStruct.CHID);
		messenger.printDebugMessage("ca_create_channel to  ", pv, " = ", status);
		
		//std::cout << "MY_SEVCHK " << std::endl;
		
		//SEVCHK(status, "ca_create_channel");
		//MY_SEVCHK(status);
		//std::cout << "MY_SEVCHK fin" << std::endl;
		//std::this_thread::sleep_for(std::chrono::seconds(4));
		
		
		//SEVCHK(ca_pend_io(5.0), "ca_pend_io");
		
		
		//std::cout << "ca_pend_io status = " << status << std::endl;
		//std::cout << "CHID = " << pvStruct.CHID << std::endl;
	//pvStruct.CHID = CHID;
	}

	catch (std::exception &e)
	{
		std::cout << "ERR " << std::endl;
		std::cout << e.what() << std::endl;
	}

}
void EPICSInterface::retrieveCHTYPE(pvStruct &pvStruct) const
{
	if (pvStruct.monitor)
	{
		if (ca_field_type(pvStruct.CHID) == DBR_DOUBLE)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_DOUBLE;
		}
		else if (ca_field_type(pvStruct.CHID) == DBR_ENUM)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_ENUM;
		}
		else if (ca_field_type(pvStruct.CHID) == DBR_LONG)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_LONG;
		}
		else if (ca_field_type(pvStruct.CHID) == DBR_SHORT)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_SHORT;
		}
		else if (ca_field_type(pvStruct.CHID) == DBR_INT)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_INT;
		}
		else
		{
			pvStruct.monitorCHTYPE = ca_field_type(pvStruct.CHID);
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
	char timeString[37];
	epicsTimeToStrftime(timeString, sizeof(timeString), "[%a %b %d %Y %H:%M:%S.%f]", &stamp);
	return timeString;
}

void EPICSInterface::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG On");
}

void EPICSInterface::debugMessagesOff()
{
	messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG OFF");
	messenger.debugMessagesOff();
}

void EPICSInterface::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(ownerName, " EPICS Interface - MESSAGES On");
}

void EPICSInterface::messagesOff()
{
	messenger.printMessage(ownerName, " EPICS Interface - MESSAGES OFF");
	messenger.messagesOff();
}

bool EPICSInterface::isMessagingOn()
{
	return messenger.isMessagingOn();
}

bool EPICSInterface::isDebugOn()
{
	return messenger.isDebugOn();
}


void EPICSInterface::setPVTimeStampFromArgs(pvStruct& pv, const event_handler_args args)
{
	const struct dbr_time_double* time = (const struct dbr_time_double*)(args.dbr);
	pv.time = time->stamp;
}

void EPICSInterface::updateTimeStampDoublePair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, double>& pairToUpdate)
{
	const struct dbr_time_double* tv = (const struct dbr_time_double*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	pairToUpdate.second = tv->value;
}

void EPICSInterface::updateTimeStampDoubleVectorPair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, std::vector< double > >& pairToUpdate, long size)
{
	const struct dbr_time_double* tv = (const struct dbr_time_double*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	std::vector<double> vec(size);
	int i = 0;
	for (auto&& it : vec)
	{
		vec[i] = *(&tv->value + i);
		i++;
	}
	pairToUpdate.second = vec;
}

void EPICSInterface::updateTimeStampIntPair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, int>& pairToUpdate)
{
	const struct dbr_time_long* tv = (const struct dbr_time_long*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	pairToUpdate.second = (int)tv->value;
}

//void EPICSInterface::updateTimeStampShortPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, short>& pairToUpdate)
//{
//	std::pair<epicsTimeStamp, short> r;
//	const struct dbr_time_short* tv = (const struct dbr_time_short*)(args.dbr);
//	pairToUpdate.first = tv->stamp;
//	pairToUpdate.second = tv->value;
//}

void EPICSInterface::updateTimeStampUShortPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, unsigned short>& pairToUpdate)
{
	//std::pair<epicsTimeStamp, unsigned short> r;
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	pairToUpdate.second = tv->value;
}


void EPICSInterface::updateTimeStampLongPair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, long>& pairToUpdate)
{
	const struct dbr_time_long* tv = (const struct dbr_time_long*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	pairToUpdate.second = tv->value;
}

void EPICSInterface::updateTimeStampShortPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, short>& pairToUpdate)
{
	const struct dbr_time_short* tv = (const struct dbr_time_short*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	pairToUpdate.second = tv->value;
}


std::pair<epicsTimeStamp, unsigned short> EPICSInterface::getTimeStampUShortPair(const struct event_handler_args& args)
{
	std::pair<epicsTimeStamp, unsigned short> r;
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	dbr_enum_t a = tv->value;
	//std::cout << "tv->value  = " << a << std::endl;
	r.first  = tv->stamp;
	r.second = (unsigned short)a;
/* 
	std::pair<epicsTimeStamp, short> r;
	const struct dbr_time_short* tv = (const struct dbr_time_short*)(args.dbr);
	r.first = tv->stamp;
	r.second = tv->value;
 */	
 return r;
}

std::pair<epicsTimeStamp, short> EPICSInterface::getTimeStampShortPair(const struct event_handler_args& args)
{
	std::pair<epicsTimeStamp, short> r;
	const struct dbr_time_short* tv = (const struct dbr_time_short*)(args.dbr);
	r.first = tv->stamp;
	r.second = (short)tv->value;
	return r;
}

std::pair<epicsTimeStamp, int> EPICSInterface::getTimeStampEnumPair(const struct event_handler_args& args)
{
	std::pair<epicsTimeStamp, short> r;
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	r.first = tv->stamp;
	r.second = (int)tv->value;
	return r;
}

std::pair<epicsTimeStamp, double> EPICSInterface::getTimeStampDoublePair(const struct event_handler_args& args)
{
	std::pair<epicsTimeStamp, double> r;
	const struct dbr_time_double* tv = (const struct dbr_time_double*)(args.dbr);
	r.first = tv->stamp;
	r.second = tv->value;
	return r;
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
		std::cout << "Something went wrOng with the update functiOn!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_double*)(args.dbr);
	return double(timeObject->value);
}

STATE EPICSInterface::returnValueFromArgsAsState(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrOng with the update functiOn!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_enum*)(args.dbr);
	return STATE(timeObject->value);
}

long EPICSInterface::returnValueFromArgsAslong(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrOng with the update functiOn!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_long*)(args.dbr);
	return long(timeObject->value);
}

float EPICSInterface::returnValueFromArgsAsFloat(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrOng with the update functiOn!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_float*)(args.dbr);
	return float(timeObject->value);
}

std::vector<double> EPICSInterface::returnValueFromArgsAsDoubleVector(const struct event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrOng with the update functiOn!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_double*)(args.dbr);
	size_t i = 0;
	auto elementCount = ca_element_count(args.chid);
	std::vector<double> rawVectorCOntainer(elementCount);
	for (auto&& it : rawVectorCOntainer)
	{
		it = *(&timeObject->value + i);
		++i;
	}
	return rawVectorCOntainer;
}
