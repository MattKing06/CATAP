#ifndef EPICS_INTERFACE_H
#define EPICS_INTERFACE_H
#include <LoggingSystem.h>
#if !defined LISTENER_H && !defined GETTER_H && !defined PUTTER_H
/* if we are using EPICSTools/Listener/Getter/Putter, then
/* we don't want to include Hardware as it is
/* unnecessary for EPICSTools/Listener/Getter/Putter */
#include <Hardware.h>
#endif // LISTENER_H || GETTER_H || PUTTER_H
#include <functional>
#include <vector>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#include <epicsTime.h>
#include <GlobalStateEnums.h>
//#include <pyconfig.h>
#endif
#ifndef PV_H_
#include "PV.h"
#endif

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
	{							\
		printf("CATAP: The requested ca operation didn't complete successfully because \"%s\"\n",ca_message(status)); \
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
#define CA_PEND_IO_TIMEOUT 10.0
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
	// We also need to create a STATIC messenger in derived epicsinterface claases, 
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
	template<typename SubscriptionType>
	void createSubscription(SubscriptionType& returnObject, pvStruct& pvStruct) const
	{
		int status = ca_create_subscription(pvStruct.monitorCHTYPE, pvStruct.COUNT,
			pvStruct.CHID, pvStruct.MASK,
			pvStruct.updateFunction,
			(void*)&returnObject,
			&pvStruct.EVID);
		MY_SEVCHK(status);
	}
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
	/*! Used to send ca_flush_io after creating a channel */
	static int caFlushIO(const std::string& ca)
	{
		int status = ca_flush_io();
		SEVCHK(status, ca.c_str());
		//printStatusResult(status, mess1.c_str(), mess2.c_str());
		return status;
	}


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
	static long returnValueFromArgsAsLong(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a float
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a float.*/
	static float returnValueFromArgsAsFloat(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a vector of doubles
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a vector of doubles.*/
	static std::vector<double> returnValueFromArgsAsDoubleVector(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a vector of integers
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a vector of integers.*/
	static std::vector<int> returnValueFromArgsAsIntVector(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a vector of floats
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a vector of floats.*/
	static std::vector<float> returnValueFromArgsAsFloatVector(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a vector of longs
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a vector of longs.*/
	static std::vector<long> returnValueFromArgsAsLongVector(const struct event_handler_args args);
	/*! Casts the value from EPICS (in args object) to a vector of unsigned shorts
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a vector of unsigned shorts.*/
	static std::vector<unsigned short> returnValueFromArgsAsEnumVector(const struct event_handler_args);
	/*! Casts the value from EPICS (in args object) to a vector of std::strings
	 * @param[in] args : The object returned by EPICS containing the new PV value
	 * @param[out] value : The value from EPICS cast as a vector of std::strings.*/
	static std::vector<std::string> returnValueFromArgsAsStringVector(const struct event_handler_args);

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
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, unsigned short pair and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampUShortPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, unsigned short>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, long pair and sets the Hardware parameter to that pair.
	* @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampLongPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, long>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, std::string pair and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampStringPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::string>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, float pair and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampFloatPair(const struct event_handler_args& args, std::pair < epicsTimeStamp, float>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, short pair and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampShortPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, short>& pairToUpdate);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, vector<double> and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampDoubleVectorPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::vector<double>>& pairToUpdate, long size);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, vector<int> and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampIntegerVectorPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::vector<int>>& pairToUpdate, long size);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, vector<long> and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampLongVectorPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::vector<long>>& pairToUpdate, long size);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, vector<float> and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampFloatVectorPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::vector<float>>& pairToUpdate, long size);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, vector<unsigned short> and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampEnumVectorPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::vector<unsigned short>>& pairToUpdate, long size);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, vector<std::string> and sets the Hardware parameter to that pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp*/
	static void updateTimeStampStringVectorPair(const struct event_handler_args& args, std::pair<epicsTimeStamp, std::vector<std::string>>& pairToUpdate, long size);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, short pair and returns the pair.
	 * @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp
	 * @param[out] pair : The value and timestamp from EPICS cast as a epicsTimeStamp, short pair. */
	static std::pair<epicsTimeStamp, short> getTimeStampShortPair(const struct event_handler_args& args);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, unsigned short pair (enum) and returns that pair.
	* @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp
	* @param[out] pair : std::pair containing the EPICS timestamp and the unsigned short (enum) value returned from EPICS */
	static std::pair<epicsTimeStamp, int> getTimeStampEnumPair(const struct event_handler_args& args);
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, double pair and returns that pair.
	* @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp
	* @param[out] pair : std::pair containing the EPICS timestamp and the double value returned from EPICS */
	static std::pair<epicsTimeStamp, double> getTimeStampDoublePair(const struct event_handler_args& args);
	static std::pair<epicsTimeStamp, unsigned short> getTimeStampUnsignedShortPair(const struct event_handler_args& args);
	// Add in some more for vectors as we need them ... 


	// TODO: what should this functiOn return? and how should that get passed to PYTHON users???
	/*! Send a value to an EPICS PV over Channel Access using ca_put.
	/*! Casts the value from EPICS (in args object) to a epicsTimeStamp, unsigned short pair and returns that pair.
	* @param[in] args : The object returned by EPICS containing the new PV value and its associated timestamp
	* @param[out] pair : std::pair containing the EPICS timestamp and the unsigned short value returned from EPICS */
	static std::pair<epicsTimeStamp, unsigned short> getTimeStampUShortPair(const struct event_handler_args& args);
	/*! Send an array to an EPICS PV over Channel Access using ca_put_array.
	* @param[in] pvStruct : Contains the PV we want to put a value to.
	* @param[in] value : The array we want to ca_put_array with.*/
	template<typename T>
	void putArray(const pvStruct& pvStruct, const std::vector<T>& value) const
	{
		if (ca_state(pvStruct.CHID) == cs_conn)
		{
			if (ca_write_access(pvStruct.CHID))
			{
				int status = ca_array_put(pvStruct.CHTYPE, pvStruct.COUNT, pvStruct.CHID, &value[0]);
				MY_SEVCHK(status);
				status = ca_pend_io(CA_PEND_IO_TIMEOUT);
				MY_SEVCHK(status);
			}
			else
			{
				messenger.printMessage(pvStruct.fullPVName, " does not have write access.");
			}
		}
		else
		{
			messenger.printMessage(pvStruct.fullPVName, " could not connect to EPICS. ");
		}
	}
	/*! Send an value to an EPICS PV over Channel Access using ca_put.
	 * @param[in] pvStruct : Contains the PV we want to put a value to.
	 * @param[in] value : The value we want to ca_put.*/
	template<typename T>
	void putValue(const pvStruct& pvStruct, const T& value) const
	{
		if (ca_state(pvStruct.CHID) == cs_conn)
		{
			if (ca_write_access(pvStruct.CHID))
			{
				int status = ca_put(pvStruct.CHTYPE, pvStruct.CHID, &value);
				MY_SEVCHK(status);
				status = ca_pend_io(CA_PEND_IO_TIMEOUT);
				MY_SEVCHK(status);
			}
			else
			{
				messenger.printMessage(pvStruct.fullPVName, " does not have write access.");
			}
		}
		else
		{
			messenger.printMessage(pvStruct.fullPVName, " could not connect to EPICS. ");
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
			//status = ca_pend_io(CA_PEND_IO_TIMEOUT);
			status = ca_flush_io();
			MY_SEVCHK(status);
			return true;

			// we should return true here if the put command got sent correctly 
		}
		return false;
	}


	template<typename T>
	static bool putArrayValue(const pvStruct& pvStruct, const T& value)
	{
		if (ca_state(pvStruct.CHID) == cs_conn)
		{
			int status = ca_array_put(pvStruct.CHTYPE, pvStruct.COUNT, pvStruct.CHID, &value);
			MY_SEVCHK(status);
			status = ca_pend_io(CA_PEND_IO_TIMEOUT);
			MY_SEVCHK(status);
			return true;

			// we should return true here if the put command got sent correctly 
		}
		return false;
	}
	/*! Send an value to an EPICS PV over Channel Access using ca_put.
	* @param[in] pvStruct : Contains the PV we want to put a value to.
	* @param[in] count : Contains the PV we want to put a value to.
	* @param[in] pointer_to_dbr_type : pointer_to_dbr_type pointer to where got array will be.
	* @param[out] bool: if command got sent to epics correctls (not if it worked!).*/
	bool getArrayUserCount(const pvStruct& pvStruct, unsigned count, void* pointer_to_dbr_type) const;
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
