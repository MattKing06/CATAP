#ifndef EPICS_VALVE_INTERFACE_H_
#define EPICS_VALVE_INTERFACE_H_
#ifndef VALVE_H_
#include <Valve.h>
#endif // VALVE
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif // EPICS INTERFACE

#include <iostream>
#include <GlobalStateEnums.h>

/**@addtogroup epicsInterface 
@{*/
class Valve;
typedef void(*updateFunctionPtr)(struct event_handler_args);
/*! A class for monitoring, getting, and settings Valve related EPICS Process Variables via Channel Access*/
class EPICSValveInterface : public EPICSInterface
{
public:
	/*! default constructor for EPICSValveInterface.*/
	EPICSValveInterface();
	/*! default destructor for EPICSValveInterface*/
	~EPICSValveInterface();
	/*! sets the updateFunction pointer in pvStruct if we want to monitor that PV
	* @param[in] pvStruct : The pvStruct associated with the PV we want to monitor
	*/
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	/*! sends a pulse (1,0) to the PV associated with the state provided
	* @param[in] value : The state we want to set the valve to (OPEN,CLOSED)
	* @param[in] value : The pvStruct associated with the valve state PV*/
	bool setNewValveState(const STATE& value, const pvStruct& pv);
	/*! callback function for EPICS to update the valveState in the Valve object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateValveState(const struct event_handler_args);

	/*! messenger object for EPICSValveInterface messaging. */
	static LoggingSystem messenger;



};
/** \copydoc EPICSInterface*/
/** @}*/
#endif //EPICS_VALVE_INTERFACE_H_