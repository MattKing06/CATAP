#ifndef EPICS_SHUTTER_INTERFACE_H_
#define EPICS_SHUTTER_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSShutterInterface : public EPICSInterface
{
public:
	EPICSShutterInterface();
	~EPICSShutterInterface();
	std::string ownerName;

	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	/*! sends a pulse (1,0) to the PV associated with the state provided
	* @param[in] value : The state we want to set the valve to (OPEN,CLOSED)
	* @param[in] value : The pvStruct associated with the valve state PV*/
	static void updateShutterState(const struct event_handler_args args);
	static void updateShutterCmi(const struct event_handler_args args);






	/*! messenger object for EPICSValveInterface messaging. */
	static LoggingSystem static_messenger;

	/*! turns debug messaging on for this EPICSShutterInterface instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this EPICSShutterInterface instance*/
	void debugMessagesOff();
	/*! turns messaging on for this EPICSShutterInterface instance*/
	void messagesOn();
	/*! turns messaging off for this EPICSShutterInterface instance*/
	void messagesOff();
	/*! Checks if messaging flag is on
	* @param[out] bool : returns true if messenger messagesOn flag is true, false otherwise.*/
	bool isMessagingOn();
	/*! Checks if debug flag is on
	* @param[out] bool : returns true if messenger debugOn flag is true, false otherwise.*/
	bool isDebugOn();

};

#endif //EPICS_SHUTTER_INTERFACE_H_