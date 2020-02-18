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


/** @defgroup epicsInterface EPICS Interfaces
	@brief A collection of classes responsible for EPICS Channel Access calls.
	
	*The collection of EPICS Interfaces provide a way of communicating with physical or virtual hardware objects
	*over EPICS Channel Access. The hardware-specific EPICS Interfaces provide setting, getting, and callback functions
	*for EPICS PVs that are associated with that hardware type. The general EPICSInterface class is concerned with setting
	*up connections and subscriptions for PVs as well as general functions that are needed by all child EPICS Interfaces, i.e. putValve.
@{*/
class PV;
#define CA_PEND_IO_TIMEOUT 5.0
class EPICSInterface
{

public:
	/*! Default Constructor: sets up a Channel Access context with pre-emptive callback enabled.*/
	EPICSInterface();
	/*! Custom Constructor: Not Currently Used.*/
	EPICSInterface(const bool& shouldStartEpics, const bool& shouldStartVirtualMachine);
	/*! Default Destructor for EPICSInterface.*/
	~EPICSInterface();
	/*! Retrieves the macro for the time EPICS should wait until it timesout on a request.
	* @param[out] CA_PEND_IO_TIMEOUT : 5.0*/
	double get_CA_PEND_IO_TIMEOUT() const;
	/*! Defines which hardware owns this EPICSInterface, set to hardwareName in constructor of 
	* the associated hardware object.*/
	std::string ownerName;
	LoggingSystem messenger;
	/*! turns debug messaging on for this EPICSInterface instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this EPICSInterface instance*/
	void debugMessagesOff();
	/*! turns messaging on for this EPICSInterface instance*/
	void messagesOn();
	/*! turns messaging off for this EPICSInterface instance*/
	void messagesOff();
	/*! Checks if messaging flag is on 
	* @param[out] bool : returns true if messenger messagesOn flag is true, false otherwise.*/
	bool isMessagingOn();
	/*! Checks if debug flag is on
	* @param[out] bool : returns true if messenger debugOn flag is true, false otherwise.*/
	bool isDebugOn();
	/*! Creates a channel using the pvStruct fullPVName and stores the channel ID in the pvStruct CHID member.
     * @param[in] pvStruct : Contains the PV we want to create a channel for .*/
	void retrieveCHID(pvStruct& pvStruct) const;
	/*! Requests the Channel Type from EPICS using the channel ID assigned in pvStruct, then sets the CHTYPE and monitorCHTYPE
	*   members in pvStruct. The monitorCHTYPE is forced to be the DBR_TIME equivalent of the CHTYPE retrieved from EPICS.
	* @param[in] pvStruct : Contains the PV we want to set the CHTYPE and monitorCHTYPE for .*/
	void retrieveCHTYPE(pvStruct& pvStruct) const;
	/*! Requests the number of items expected to be brought back from EPICS in a monitor or get request using the Channel ID in pvStruct.
	 * @param[in] pvStruct : Contains the PV we want to set the COUNT for.*/
	void retrieveCOUNT(pvStruct& pvStruct) const;
	/*! Sets up a monitor to a PV which provides EPICS a callback function for updating the hardware object we pass in.
	* @param[in] hardware : The hardware object that is passed to our callback function to update
	* @param[in] pvStruct : Used to tell EPICS which PV to monitor and which callback function to use when the PV changes*/
	void createSubscription(Hardware& hardware, pvStruct& pvStruct) const;
	/*! Removes the monitor for a given PV
	* @param[in] pv : The PV we want to remove the monitor for.*/
	void removeSubscription(pvStruct& pv);
	/*! Removes the channel for a given PV
	* @param[in] pv : The PV we want to remove the channel for.*/
	void removeChannel(pvStruct& pv);
	/*! Removes the current ca_context (EPICS Connection)*/
	void detachFromContext();
	/*! Used to send the buffered requests to EPICS using ca_pend_io function*/
	static void sendToEPICS();
	/*! Converts an epicsTimeStamp into a formatted string for printing the timestamp.
	* @param[in] stamp : The timestamp we want to convert to string
	* @param[out] stampString : The string representation of the epicsTimeStamp input.*/
	static std::string getEPICSTime(const epicsTimeStamp& stamp);
	/*! Sets the time member of pvStruct using the returned value (args) from EPICS.
	* @param[in] pv : The PV for which we want to set the time member.
	* @param[in] args : The object returned from EPICS that contains the timestamp.*/
	static void setPVTimeStampFromArgs(pvStruct& pv, const struct event_handler_args args);
	/*! Templated function that casts the object returned from EPICS into a given hardware type.
	* @param[in] args : The object returned from an EPICS monitor
	* @param[out] hardwareType : The hardware object we have cast args to.*/
	template<typename hardwareType>
	static hardwareType* getHardwareFromArgs(const struct event_handler_args args)
	{
		return static_cast<hardwareType*>(args.usr);
	}




	// functiOns to return a pair<timestamp, TYPE > for simple types 
	//static std::pair<epicsTimeStamp, int>    returnTSVFromArgsInt(const struct event_handler_args& args);
	//static std::pair<epicsTimeStamp, double> (const struct event_handler_args& args);

	/*! Casts the value from EPICS (in args object) to a double
	* @param[in] args : The object returned by EPICS containing the new PV value
	* @param[out] value : The value from EPICS cast as a double.*/
	static double returnValueFromArgsAsDouble(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a STATE
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a STATE.*/
	static STATE returnValueFromArgsAsState(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a long
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a long.*/
	static long returnValueFromArgsAslong(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a float
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a float.*/
	static float returnValueFromArgsAsFloat(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a vector of doubles
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a vector of doubles.*/
	static std::vector<double> returnValueFromArgsAsDoubleVector(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a std::string
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a std::string.*/
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
	
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, double pair and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampDoublePair(const struct event_handler_args& args, std::pair<epicsTimeStamp, double>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, int pair and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampIntPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, int>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, short pair and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampShortPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, short>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, short pair and returns the pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp
	 * @param[out] pair : The value and timestamp from EPICS cast as a epicsTimeStamp, short pair. */
	static std::pair<epicsTimeStamp, short> getTimeStampShortPair(const struct event_handler_args& args);
	// Add in some more for vectors as we need them ... 


	// TODO: what should this functiOn return? and how should that get passed to PYTHON users???
	/*! Send a value to an EPICS PV over Channel Access using ca_put. 
	* @param[in] pvStruct : Contains the PV we want to put a value to. 
	* @param[in] value : The value we want to ca_put.*/
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

	/*! Send a value to an EPICS PV over Channel Access using ca_put.
	* @param[in] pvStruct : Contains the PV we want to put a value to.
	* @param[in] value : The value we want to ca_put.
	* @param[out] status : return true if ca_put request sent successfully, false otherwise.*/
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
	/*! NOT IN USE */
	bool shouldStartEpics = true;
	/*! NOT IN USE */
	bool shouldStartVirtualMachine = true;

	// some other stuff might be needed here, need to check interface.h from VELA-CLARA COntrollers
};
/** @}*/
#endif

