#ifndef EPICS_INTERFACE_H
#define EPICS_INTERFACE_H
#include <LoggingSystem.h>
#include <Hardware.h>
#include <functional>
#include <vector>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#include <epicsTime.h>
//#include <pyconfig.h>
#endif
#ifndef PV_H_
#include "PV.h"
#endif

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
	{							\
		ca_message(status);		\
		SEVCHK(status, NULL);   \
		exit(status);			\
	}							\
}								\



class PV;
#define CA_PEND_IO_TIMEOUT 5.0
class EPICSInterface
{

public:
	EPICSInterface();
	EPICSInterface(const bool& shouldStartEpics, const bool& shouldStartVirtualMachine);
	~EPICSInterface();
	double get_CA_PEND_IO_TIMEOUT() const;
	std::string ownerName;
	LoggingSystem messenger;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();
	void set_CA_PEND_IO_TIMEOUT(double value);
	void retrieveCHID(pvStruct& pvStruct) const;
	void retrieveCHTYPE(pvStruct& pvStruct) const;
	void retrieveCOUNT(pvStruct& pvStruct) const;
	void createSubscription(Hardware& hardware, pvStruct& pvStruct) const;
	void removeSubscription(pvStruct& pv);
	void removeChannel(pvStruct& pv);
	void detachFromCOntext();
	static void sendToEPICS();
	static std::string getEPICSTime(const epicsTimeStamp& stamp);
	static void setPVTimeStampFromArgs(pvStruct& pv, const struct event_handler_args args);
	template<typename hardwareType>
	static hardwareType* getHardwareFromArgs(const struct event_handler_args args)
	{
		return static_cast<hardwareType*>(args.usr);
	}




	// functiOns to return a pair<timestamp, TYPE > for simple types 
	//static std::pair<epicsTimeStamp, int>    returnTSVFromArgsInt(const struct event_handler_args& args);
	//static std::pair<epicsTimeStamp, double> (const struct event_handler_args& args);


	static double returnValueFromArgsAsDouble(const struct event_handler_args args);
	static STATE returnValueFromArgsAsState(const struct event_handler_args args);
	static long returnValueFromArgsAslong(const struct event_handler_args args);
	static float returnValueFromArgsAsFloat(const struct event_handler_args args);
	static std::vector<double> returnValueFromArgsAsDoubleVector(const struct event_handler_args args);
	static std::string returnValueFromArgsAsString(const event_handler_args args);


#ifndef __CINT__
	ca_client_context* thisCaContext;
	void attachTo_thisCaContext();
	void detachFrom_thisCaContext();
	void addILockChannels(const int numILocks,
		const std::string& pvRoot,
		const std::string& objectName
		// map_ilck_pvstruct& ILockPVStructs,
	);
	
	
	static void updateTimeStampDoublePair(const struct event_handler_args& args, std::pair<epicsTimeStamp, double>& pairToUpdate);
	static void updateTimeStampDoubleVectorPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::vector< double > >& pairToUpdate, int size);
	static void updateTimeStampIntPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, int>& pairToUpdate);
	static void updateTimeStampLongPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, long>& pairToUpdate);
	static void updateTimeStampShortPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, short>& pairToUpdate);
	// sometimes you have to return a pair<timestamp,int> and then choose a STAT based On the int
	static std::pair<epicsTimeStamp, short> getTimeStampShortPair(const struct event_handler_args& args);
	// Add in some more for vectors as we need them ... 


	// TODO: what should this functiOn return? and how should that get passed to PYTHON users??? 
	template<typename T>
	void putValue(const pvStruct& pvStruct, const T& value) const
	{
		if (ca_state(pvStruct.CHID) == cs_conn)
		{
			int status = ca_put(pvStruct.CHTYPE, pvStruct.CHID, &value);
			MY_SEVCHK(status);
			status = ca_pend_io(CA_PEND_IO_TIMEOUT);
			MY_SEVCHK(status);
		}
	}

	// A putvalue that returns something 
	// TODO: what should this functiOn return? and how should that get passed to PYTHON users??? 
	template<typename T>
	static bool putValue2(const pvStruct& pvStruct, const T& value)
	{
		if (ca_state(pvStruct.CHID) == cs_conn)
		{
			int status = ca_put(pvStruct.CHTYPE, pvStruct.CHID, &value);
			MY_SEVCHK(status);
			status = ca_pend_io(CA_PEND_IO_TIMEOUT);
			MY_SEVCHK(status);
			return true;

			// we should return true here if the put commadn got sent correctly 
		}
		return false;
	}

#endif
protected:
	bool shouldStartEpics = true;
	bool shouldStartVirtualMachine = true;
	unsigned short EPICS_ACTIVATE, EPICS_SEND, EPICS_RESET;

	// some other stuff might be needed here, need to check interface.h from VELA-CLARA COntrollers
};

#endif

