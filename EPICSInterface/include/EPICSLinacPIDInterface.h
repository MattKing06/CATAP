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
