#ifndef EPICS_LINAC_PID_INTERFACE_H_
#define EPICS_LINAC_PID_INTERFACE_H_

#include <LinacPID.h>
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif // !EPICS_INTERFACE_H_
#include <GlobalStateEnums.h>
#include <LinacPIDPVRecords.h>

/**@addtogroup epicsInterface
@{*/

class LinacPID;
typedef void(*updateFunctionPtr)(struct event_handler_args);

class EPICSLinacPIDInterface : public EPICSInterface
{
public:
	EPICSLinacPIDInterface();
	~EPICSLinacPIDInterface();
	void retrieveUpdateFunctionForRecord(pvStruct& pvStruct) const;

	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	// EPICS calls these functiOns to update a variable
	static void update_AVG_PHASE(const struct event_handler_args args);
	static void update_FORWARD_PHASE_WEIGHT(const struct event_handler_args args);
	static void update_PROBE_PHASE_WEIGHT(const struct event_handler_args args);
	static void update_SCAN(const struct event_handler_args args);
	static void update_PID_OVAL(const struct event_handler_args args);
	static void update_FORWARD_PHASE_WRAPPED(const struct event_handler_args args);
	static void update_PROBE_PHASE_WRAPPED(const struct event_handler_args args);


	
	/*! messenger object for EPICSLinacPIDInterface messaging. */
	static LoggingSystem static_messenger;
	/*! turns debug messaging on for this EPICSLinacPIDInterface instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this EPICSLinacPIDInterface instance*/
	void debugMessagesOff();
	/*! turns messaging on for this EPICSLinacPIDInterface instance*/
	void messagesOn();
	/*! turns messaging off for this EPICSLinacPIDInterface instance*/
	void messagesOff();
	/*! Checks if messaging flag is on
	* @param[out] bool : returns true if messenger messagesOn flag is true, false otherwise.*/
	bool isMessagingOn();
	/*! Checks if debug flag is on
	* @param[out] bool : returns true if messenger debugOn flag is true, false otherwise.*/
	bool isDebugOn();

};

/** \copydoc EPICSInterface*/
/** @}*/
#endif // !EPICS_LINAC_PID_INTERFACE_H_
