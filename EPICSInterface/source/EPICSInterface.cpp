#include "EPICSInterface.h"
#include "GlobalFunctions.h"
#include <exception>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>



//std::map<int, std::string> EPICSInterface::status_to_string = {
//	{ECA_NORMAL, "ECA_NORMAL"},
//	{ECA_TIMEOUT, "ECA_TIMEOUT"},
//	{ECA_BADTYPE, "ECA_BADTYPE"},
//	{ECA_BADCHID, "ECA_BADCHID"},
//	{ECA_BADCOUNT, "ECA_BADCOUNT"},
//	{ECA_GETFAIL, "ECA_GETFAIL"},
//	{ECA_NORDACCESS, "ECA_NORDACCESS"},
//	{ECA_DISCONN, "ECA_DISCONN"}
//};

//CURRENT_CA_CONTEXT = nullptr;


void EPICSInterface::initialiseContext()
{
	int status;
	/*
		This 'enables' callbacks, monitoring, etc
		thisCaContext is the current (AND ONLY) context,
		use it to join from new threads with ca_attach_context
		you NEED to attach to this context if multi-threading
	*/
	if (!CURRENT_CA_CONTEXT)
	{
		status = ca_context_create(ca_enable_preemptive_callback);
		CURRENT_CA_CONTEXT = ca_current_context();
		switch (status)
		{
		case(ECA_NORMAL):
			std::cout << "Success, created ca_context" << std::endl;
			break;
		case(ECA_ALLOCMEM):
			std::cout << "Failed to create ca_context, unable to allocate space in pool." << std::endl;
			break;
		case(ECA_NOTTHREADED):
			std::cout << "Someone has disabled pre-emptive callbacks..." << std::endl;
			break;
		}
	}
	else
	{
		status = ca_attach_context(CURRENT_CA_CONTEXT);
		switch (status)
		{
		case(ECA_NORMAL):
			std::cout << "Success, attached to ca_context" << std::endl;
			break;
		case(ECA_ISATTACHED):
			std::cout << "Already joined to this ca context." << std::endl;
			break;
		case(ECA_NOTTHREADED):
			std::cout << "Someone has disabled pre-emptive callbacks..." <<	std::endl;
			break;
		}
	}
}

EPICSInterface::EPICSInterface() : 
messenger(LoggingSystem(true, true))
{
	initialiseContext();
	messenger.printDebugMessage("EPICSInterface constuctor ");
}

EPICSInterface::EPICSInterface(const bool& startEpics, const bool& startVirtualMachine):
messenger(LoggingSystem(true, true))
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	initialiseContext();
}

EPICSInterface::~EPICSInterface()
{
	messenger.printDebugMessage("EPICSInterface Destructor Called");
	//messenger.printMessage("ca_context_status: ", ca_context_status(thisCaContext, 2));
	//ca_context_destroy();
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
	try
	{
		int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		MY_SEVCHK(status);
	}
	catch (const std::runtime_error & runtimeError)
	{
		std::cout << runtimeError.what() << std::endl;
	}
}

void EPICSInterface::sendToEPICSm(const char* m )
{
	try
	{
		int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		MY_SEVCHK_Mess(status,m);
	}
	catch (const std::runtime_error& runtimeError)
	{
		std::cout << runtimeError.what() << std::endl;
	}
}
bool EPICSInterface::sendToEPICSm2(const char* m)
{
	int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
	MY_SEVCHK_Mess(status, m);
	if (status == ECA_NORMAL)
	{
		return true;
	}
	coutECASTATUS(status);
	return false;
}


int EPICSInterface::sendToEPICSReturnStatus()
{
	int status = ca_pend_io(CA_PEND_IO_TIMEOUT);
	std::string status_str = "ca_pend_io return status: " + status;
	SEVCHK(status, status_str.c_str());
	return status;
}

void EPICSInterface::attachTo_thisCaContext()
{
	if (CURRENT_CA_CONTEXT)
	{
		int status = ca_attach_context(CURRENT_CA_CONTEXT);
		switch (status)
		{
		case(ECA_NORMAL):
			messenger.printMessage("Success, created ca_context");
			break;
		case(ECA_ISATTACHED):
			messenger.printMessage("Already joined to this ca context.");
			break;
		case(ECA_NOTTHREADED):
			messenger.printMessage("Someone has disabled pre-emptive callbacks...");
			break;
		}
	}

}

void EPICSInterface::detachFrom_thisCaContext()
{
	ca_detach_context();
}
//void EPICSInterface::createSubscription(Hardware& hardware, pvStruct& pvStruct) const
//{
//	int status = ca_create_subscription(pvStruct.monitorCHTYPE, pvStruct.COUNT,
//										pvStruct.CHID, pvStruct.MASK,
//										pvStruct.updateFunction,
//										(void*)&hardware, 
//										&pvStruct.EVID);
//	MY_SEVCHK(status);
//}

void EPICSInterface::retrieveCHID(pvStruct &pvStruct) const // createChannel is a better name ?? 
{
	try
	{
		int status;
		//chid CHID;
		// This should eb defeind in the hardware objst, so that we can handle non-standrd PV names
		//std::string pv = pvStruct.fullPVName + ":" + pvStruct.pvRecord;
		///*CURRENTLY PV STRUCTS FOR MAGNET CONTAIN FULL PV at pvStruct.FullPVName*/
		//if (pvStruct.fullPVName.find("MAG") != std::string::npos ||
		//	pvStruct.fullPVName.find("VALV") != std::string::npos ||
		//	pvStruct.fullPVName.find("BPM") != std::string::npos ||
		//	pvStruct.fullPVName.find("WCM") != std::string::npos ||
		//	pvStruct.fullPVName.find("FCUP") != std::string::npos ||
		//	pvStruct.fullPVName.find("SCR") != std::string::npos ||
		//	pvStruct.fullPVName.find("YAG") != std::string::npos ||
		//	pvStruct.fullPVName.find("IMG") != std::string::npos )
		//{
		//	pv = pvStruct.fullPVName;
		//}
		//if (pvStruct.fullPVName.find("VALV") != std::string::npos)
		//{
		//	pv = pvStruct.fullPVName;
		//}
		std::string pv = pvStruct.fullPVName;
		messenger.printDebugMessage("ca_create_channel to  ", pv);
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
	pvStruct.CHTYPE = ca_field_type(pvStruct.CHID);
	if (pvStruct.monitor)
	{
		if (pvStruct.CHTYPE == DBR_DOUBLE)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_DOUBLE;
		}
		else if (pvStruct.CHTYPE == DBR_ENUM)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_ENUM;
		}
		else if (pvStruct.CHTYPE == DBR_LONG)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_LONG;
		}
		else if (pvStruct.CHTYPE == DBR_SHORT)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_SHORT;
		}
		else if (pvStruct.CHTYPE == DBR_INT)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_INT;
		}
		else if (pvStruct.CHTYPE == DBR_CHAR)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_CHAR;
		}
		else if (pvStruct.CHTYPE == DBR_STRING)
		{
			pvStruct.monitorCHTYPE = DBR_TIME_STRING;
		}
		else
		{
			messenger.printMessage(pvStruct.fullPVName, " !!WARNING!! unexpected ca_field_type = ", ca_field_type(pvStruct.CHID));
			pvStruct.monitorCHTYPE = ca_field_type(pvStruct.CHID);
		}
	}
}

void EPICSInterface::retrieveCOUNT(pvStruct &pvStruct) const
{
	pvStruct.COUNT = ca_element_count(pvStruct.CHID);
}

std::string EPICSInterface::getEPICSTime(const epicsTimeStamp& stamp)
{
	char timeString[37];
	epicsTimeToStrftime(timeString, sizeof(timeString), "[%a %b %d %Y %H:%M:%S.%f]", &stamp);
	return std::string(timeString);
}

void EPICSInterface::getEPICSTime(const epicsTimeStamp& stamp, std::string& str)
{
	char timeString[37];
	epicsTimeToStrftime(timeString, sizeof(timeString), "[%a %b %d %Y %H:%M:%S.%f]", &stamp);
	str = std::string(timeString);
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

bool EPICSInterface::check_ca_state(const pvStruct& pvStruct)
{
	if (ca_state(pvStruct.CHID) == cs_conn)
	{
		return true;
	}
	else
	{
		messenger.printMessage(pvStruct.fullPVName, " could not connect to EPICS. ");
	}
	return false;
}

bool EPICSInterface::check_ca_write_access(const pvStruct& pvStruct)
{
	if (ca_write_access(pvStruct.CHID))
	{
		return true;
	}
	else
	{
		messenger.printMessage(pvStruct.fullPVName, " does not have write access.");
	}
	return false;
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

void EPICSInterface::updateTimeStampLongPair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, long>& pairToUpdate)
{
	const struct dbr_time_long* tv = (const struct dbr_time_long*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	pairToUpdate.second = tv->value;
}

void EPICSInterface::updateTimeStampBoolPair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, bool>& pairToUpdate)
{
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	if(tv->value == GlobalConstants::one_int)
	{
		pairToUpdate.second = true;
	}
	else
	{
		pairToUpdate.second = false;
	}
	pairToUpdate.first = tv->stamp;
}

void EPICSInterface::updateTimeStampDoubleVectorPair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, std::vector<double>>& pairToUpdate, long size)
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

void EPICSInterface::updateTimeStampShortVectorPair(const event_handler_args& args, std::pair<epicsTimeStamp, std::vector<short>>& pairToUpdate, long size)
{
	const struct dbr_time_short* tv = (const struct dbr_time_short*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	std::vector<short> vec(size);
	int i = 0;
	for (auto&& it : vec)
	{
		vec[i] = *(&tv->value + i);
		i++;
	}
	pairToUpdate.second = vec;
}

void EPICSInterface::updateTimeStampIntegerVectorPair(const event_handler_args& args, std::pair<epicsTimeStamp, std::vector<int>>& pairToUpdate, long size)
{
	const struct dbr_time_long* tv = (const struct dbr_time_long*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	std::vector<int> vec(size);
	int i = 0;
	for (auto&& it : vec)
	{
		vec[i] = *(&tv->value + i);
		i++;
	}
	pairToUpdate.second = vec;
}

void EPICSInterface::updateTimeStampLongVectorPair(const event_handler_args& args, std::pair<epicsTimeStamp, std::vector<long>>& pairToUpdate, long size)
{
	const struct dbr_time_long* tv = (const struct dbr_time_long*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	std::vector<long> vec(size);
	int i = 0;
	for (auto&& it : vec)
	{
		vec[i] = *(&tv->value + i);
		i++;
	}
	pairToUpdate.second = vec;
}

void EPICSInterface::updateTimeStampFloatVectorPair(const event_handler_args& args, std::pair<epicsTimeStamp, std::vector<float>>& pairToUpdate, long size)
{
	const struct dbr_time_float* tv = (const struct dbr_time_float*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	std::vector<float> vec(size);
	int i = 0;
	for (auto&& it : vec)
	{
		vec[i] = *(&tv->value + i);
		i++;
	}
	pairToUpdate.second = vec;
}

void EPICSInterface::updateTimeStampEnumVectorPair(const event_handler_args& args, std::pair<epicsTimeStamp, std::vector<unsigned short>>& pairToUpdate, long size)
{
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	std::vector<unsigned short> vec(size);
	int i = 0;
	for (auto&& it : vec)
	{
		vec[i] = *(&tv->value + i);
		i++;
	}
	pairToUpdate.second = vec;
}

void EPICSInterface::updateTimeStampStringVectorPair(const event_handler_args& args, std::pair<epicsTimeStamp, std::vector<std::string>>& pairToUpdate, long size)
{
	const struct dbr_time_string* tv = (const struct dbr_time_string*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	std::vector<std::string> vec(size);
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



void EPICSInterface::updateTimeStampStringPair(const struct event_handler_args& args,
	std::pair<epicsTimeStamp, std::string>& pairToUpdate)
{
	const struct dbr_time_string* tv = (const struct dbr_time_string*)(args.dbr);
	pairToUpdate.first = tv->stamp;
	pairToUpdate.second = tv->value;
}

void EPICSInterface::updateTimeStampFloatPair(const struct event_handler_args& args,
	std::pair < epicsTimeStamp, float>& pairToUpdate)
{
	const struct dbr_time_float* tv = (const struct dbr_time_float*)(args.dbr);
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
	//std::cout << "getTimeStampUShortPair, tv->value  = " << a << " sizeof(a) " << sizeof(a) << std::endl;
	r.first  = tv->stamp;
	r.second = (unsigned short)a;
	//std::cout << "r.second   = " << r.second  << " sizeof(r.second ) " << sizeof(r.second ) << std::endl;

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

std::pair<epicsTimeStamp, unsigned short> EPICSInterface::getTimeStampUnsignedShortPair(const struct event_handler_args& args)
{
	std::pair<epicsTimeStamp, unsigned short> r;
	const struct dbr_time_short* tv = (const struct dbr_time_short*)(args.dbr);
	r.first = tv->stamp;
	r.second = (unsigned short)tv->value;
	return r;
}


std::pair < epicsTimeStamp, std::string > EPICSInterface::getTimeStampStringPair(const struct event_handler_args& args)
{
	std::pair < epicsTimeStamp, std::string > r;
	const struct dbr_time_string* tv = (const struct dbr_time_string*)(args.dbr);
	r.first = tv->stamp;
	r.second = std::string((const char*)tv->value, MAX_STRING_SIZE); // MAGIC NUMBER !
	GlobalFunctions::rtrim(r.second);
	r.second.erase(r.second.find('\0'));
	return r;
}
std::pair < epicsTimeStamp, long > EPICSInterface::getTimeStampLongPair(const struct event_handler_args& args)
{
	std::pair < epicsTimeStamp, long > r;
	const struct dbr_time_long* tv = (const struct dbr_time_long*)(args.dbr);
	r.first = tv->stamp;
	r.second = (long)tv->value; 
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

std::vector<double> EPICSInterface::returnValueFromArgsAsDoubleVector(const struct event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_double*)(args.dbr);
	size_t i = 0;
	auto elementCount = ca_element_count(args.chid);
	std::vector<double> rawVectorContainer(elementCount);
	for (auto&& it : rawVectorContainer)
	{
		it = *(&timeObject->value + i);
		++i;
	}
	return rawVectorContainer;
}

std::vector<int> EPICSInterface::returnValueFromArgsAsIntVector(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_long*)(args.dbr);
	size_t i = 0;
	auto elementCount = ca_element_count(args.chid);
	std::vector<int> rawVectorContainer(elementCount);
	for (auto&& it : rawVectorContainer)
	{
		it = *(&timeObject->value + i);
		++i;
	}
	return rawVectorContainer;
}

std::vector<float> EPICSInterface::returnValueFromArgsAsFloatVector(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_float*)(args.dbr);
	size_t i = 0;
	auto elementCount = ca_element_count(args.chid);
	std::vector<float> rawVectorContainer(elementCount);
	for (auto&& it : rawVectorContainer)
	{
		it = *(&timeObject->value + i);
		++i;
	}
	return rawVectorContainer;
}

std::vector<long> EPICSInterface::returnValueFromArgsAsLongVector(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_long*)(args.dbr);
	size_t i = 0;
	auto elementCount = ca_element_count(args.chid);
	std::vector<long> rawVectorContainer(elementCount);
	for (auto&& it : rawVectorContainer)
	{
		it = *(&timeObject->value + i);
		++i;
	}
	return rawVectorContainer;
}

std::vector<unsigned short> EPICSInterface::returnValueFromArgsAsEnumVector(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_enum*)(args.dbr);
	size_t i = 0;
	auto elementCount = ca_element_count(args.chid);
	std::vector<unsigned short> rawVectorContainer(elementCount);
	for (auto&& it : rawVectorContainer)
	{
		it = *(&timeObject->value + i);
		++i;
	}
	return rawVectorContainer;
}

std::vector<std::string> EPICSInterface::returnValueFromArgsAsStringVector(const event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cout << "Something went wrong with the update function!" << std::endl;
	}
	auto timeObject = (const struct dbr_time_string*)(args.dbr);
	size_t i = 0;
	auto elementCount = ca_element_count(args.chid);
	std::vector<std::string> rawVectorContainer(elementCount);
	for (auto&& it : rawVectorContainer)
	{
		it = *(&timeObject->value + i);
		++i;
	}
	return rawVectorContainer;
}

bool EPICSInterface::getArrayUserCount(const pvStruct& pvStruct, unsigned count, void* pointer_to_dbr_type) const
{
	std::cout << "getArrayUserCount, count = " << count << std::endl;
	std::cout << "ECA_NORMAL check  " << ECA_NORMAL << std::endl;
		
	if (ca_state(pvStruct.CHID) == cs_conn)
	{
		std::cout << "ca_state(pvStruct.CHID) == cs_conn " << std::endl;
		int status = ca_array_get(pvStruct.CHTYPE, count, pvStruct.CHID, pointer_to_dbr_type);
		std::cout << "ca_array_get complete status =  " << status << std::endl;
		MY_SEVCHK(status);
		if ( status == ECA_NORMAL)
		{
			std::cout << "status == ECA_NORMAL " << std::endl;
			//int status2 = ca_pend_io(CA_PEND_IO_TIMEOUT);
			int status2 = ca_flush_io();
			std::cout << "ca_flush_io complete, status = " << status2 << std::endl;
			MY_SEVCHK(status2);
			if (status2 == ECA_NORMAL)
			{
				return true;
			}
		}
		else
		{
			std::cout << "ca_array_get did not return  ECA_NORMAL " << std::endl;
		}

	}
	return false;
}
void EPICSInterface::coutECASTATUS(const int status)
{
	std::cout << "status = ";
	switch (status)
	{
		case ECA_NORMAL: std::cout << "ECA_NORMAL" << std::endl; break;
		case ECA_ALLOCMEM: std::cout << "ECA_ALLOCMEM" << std::endl; break;
		case ECA_TOLARGE: std::cout << "ECA_TOLARGE" << std::endl; break;
		case ECA_TIMEOUT: std::cout << "ECA_TIMEOUT" << std::endl; break;
		case ECA_BADTYPE: std::cout << "ECA_BADTYPE" << std::endl; break;
		case ECA_INTERNAL: std::cout << "ECA_INTERNAL" << std::endl; break;
		case ECA_GETFAIL: std::cout << "ECA_GETFAIL" << std::endl; break;
		case ECA_PUTFAIL: std::cout << "ECA_PUTFAIL" << std::endl; break;
		case ECA_BADCOUNT: std::cout << "ECA_BADCOUNT" << std::endl; break;
		case ECA_BADSTR: std::cout << "ECA_BADSTR" << std::endl; break;
		case ECA_DBLCHNL: std::cout << "ECA_DISCONN" << std::endl; break;
		case ECA_EVDISALLOW: std::cout   <<"ECA_EVDISALLOW" << std::endl; break;
		case ECA_BADMONID: std::cout << "ECA_BADMONID" << std::endl; break;
		case ECA_BADMASK: std::cout << "ECA_BADMASK" << std::endl; break;
		case ECA_IODONE: std::cout << "ECA_IODONE" << std::endl; break;
		case ECA_IOINPROGRESS: std::cout << "ECA_IOINPROGRESS" << std::endl; break;
		case ECA_BADSYNCGRP: std::cout << "ECA_BADSYNCGRP" << std::endl; break;
		case ECA_PUTCBINPROG: std::cout << "ECA_PUTCBINPROG" << std::endl; break;
		case ECA_NORDACCESS: std::cout << "ECA_NORDACCESS" << std::endl; break;
		case ECA_NOWTACCESS: std::cout << "ECA_NOWTACCESS" << std::endl; break;
		case ECA_ANACHRONISM: std::cout << "ECA_ANACHRONISM" << std::endl; break;
		case ECA_NOSEARCHADDR: std::cout << "ECA_NOSEARCHADDR" << std::endl; break;
		case ECA_NOCONVERT: std::cout << "ECA_NOCONVERT" << std::endl; break;
		case ECA_BADCHID: std::cout << "ECA_BADCHID" << std::endl; break;
		case ECA_BADFUNCPTR: std::cout << "ECA_BADFUNCPTR" << std::endl; break;
		case ECA_ISATTACHED: std::cout << "ECA_ISATTACHED" << std::endl; break;
		case ECA_UNAVAILINSERV: std::cout << "ECA_UNAVAILINSERV" << std::endl; break;
		case ECA_CHANDESTROY: std::cout << "ECA_CHANDESTROY" << std::endl; break;
		case ECA_BADPRIORITY: std::cout << "ECA_BADPRIORITY" << std::endl; break;
		case ECA_NOTTHREADED: std::cout << "ECA_NOTTHREADED" << std::endl; break;
		case ECA_16KARRAYCLIENT: std::cout << "ECA_16KARRAYCLIENT" << std::endl; break;
		case ECA_CONNSEQTMO: std::cout << "ECA_CONNSEQTMO" << std::endl; break;
		case ECA_UNRESPTMO: std::cout << "ECA_UNRESPTMO" << std::endl; break;
		default: std::cout << "!!UNKONWN STATUS!!" << std::endl;
	}
}