#ifndef UPDATE_FUNCTIONS_H
#define UPDATE_FUNCTIONS_H

#include <PV.h>


typedef void(*UpdateFunctionPtr)(const struct event_handler_args args);
/** @addtogroup epicsTools
 *@{*/
 /*! UpdateFunctionHolder
 @brief A class for EPICS-related update functions.
 */
class UpdateFunctionHolder
{
public:
	/*! Default Constructor: No member variables to set. Does not do anything important.*/
	UpdateFunctionHolder();
	/*! Copy Constructor: No member variables to copy over. Does not do anything important.*/
	UpdateFunctionHolder(const UpdateFunctionHolder& copyUpdateFunctionHolder);
	/*! A function to find the appropriate update function via pv CHTYPE/monitorCHTYPE.
		This function will also consider the pv COUNT to determine if an array update function
		is required.
		@param[in] pvStruct : Used to tell EPICS which PV to monitor and which update function to use
		@param[out] UpdateFunctionPtr: A function pointer to the appropriate update function */
	UpdateFunctionPtr findUpdateFunction(pvStruct& pv);
	/*! Update function for DBR_DOUBLE and DBR_TIME_DOUBLE pv types with COUNT = 1 
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateDoubleValue(const struct event_handler_args args);
	/*! Update function for DBR_INT and DBR_TIME_INT pv types with COUNT = 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateIntegerValue(const struct event_handler_args args);
	/*! Update function for DBR_SHORT and DBR_TIME_SHORT pv types with COUNT = 1
	@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
*/	
	static void updateShortValue(const struct event_handler_args args);
	/*! Update function for DBR_ENUM and DBR_TIME_ENUM pv types with COUNT = 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateEnumValue(const struct event_handler_args args);
	/*! Update function for DBR_FLOAT and DBR_TIME_FLOAT pv types with COUNT = 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateFloatValue(const struct event_handler_args args);
	/*! Update function for DBR_LONG and DBR_TIME_LONG pv types with COUNT = 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateLongValue(const struct event_handler_args args);
	/*! Update function for DBR_STRING and DBR_TIME_STRING pv types with COUNT = 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateStringValue(const struct event_handler_args args);
	/*! Update function for DBR_DOUBLE and DBR_TIME_DOUBLE pv types with COUNT > 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateDoubleArray(const struct event_handler_args args);
	/*! Update function for DBR_INT and DBR_TIME_INT pv types with COUNT > 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateIntegerArray(const struct event_handler_args args);
	/*! Update function for DBR_SHORT and DBR_TIME_SHORT pv types with COUNT > 1
	@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
*/
	static void updateShortArray(const struct event_handler_args args);
	/*! Update function for DBR_ENUM and DBR_TIME_ENUM pv types with COUNT > 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateEnumArray(const struct event_handler_args args);
	/*! Update function for DBR_FLOAT and DBR_TIME_FLOAT pv types with COUNT > 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateFloatArray(const struct event_handler_args args);
	/*! Update function for DBR_LONG and DBR_TIME_LONG pv types with COUNT > 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateLongArray(const struct event_handler_args args);
	/*! Update function for DBR_STRING and DBR_TIME_STRING pv types with COUNT > 1
		@param[in] args : Passed to the function by EPICS, contains the Listener object to update.
	*/
	static void updateStringArray(const struct event_handler_args args);
};

#endif
/**@}*/