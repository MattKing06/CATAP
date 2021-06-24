#ifndef EPICS_CHARGE_INTERFACE_H_
#define EPICS_CHARGE_INTERFACE_H_
#ifndef CHARGE_H_
#include "Charge.h"
#endif //BPM_H_
#ifndef EPICS_INTERFACE_H_
#include "EPICSInterface.h"
#endif 
#include <iostream>
#include <string>
#include <boost/circular_buffer.hpp>
/**@addtogroup epicsInterface
@{*/
class Charge;
typedef void(*updateFunctionPtr)(struct event_handler_args args);
/*! A class for monitoring, getting, and setting charge related EPICS Process Variables via Channel Access*/
class EPICSChargeInterface : public EPICSInterface
{
public:
	/*! default constructor for EPICSChargeInterface.*/
	EPICSChargeInterface();
	/*! default destructor for EPICSChargeInterface.*/
	~EPICSChargeInterface();
	/*! sets the updateFunction pointer in pvStruct if we want to monitor that PV
	* @param[in] pvStruct : The pvStruct associated with the PV we want to monitor
	*/
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	/*! callback function for EPICS to update the charge from the EPICS PV in the Charge object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
								  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateQ(const struct event_handler_args args);
	/*! sends a pulse (a double) to set the charge PV (virtual only)
	* @param[in] value : The desired charge value
	* @param[in] pv : The pvStruct associated with the charge PV*/
	void setQ(const double& value, const pvStruct& pv);
	static LoggingSystem messenger;
};
/** \copydoc EPICSInterface*/
/** @}*/
#endif //EPICS_CHARGE_INTERFACE_H_