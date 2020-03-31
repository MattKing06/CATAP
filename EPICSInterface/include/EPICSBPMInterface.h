#ifndef EPICS_BPM_INTERFACE_H_
#define EPICS_BPM_INTERFACE_H_
#ifndef BPM_H_
#include <BPM.h>
#endif //BPM_H_
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif 
#include <iostream>
#include <string>
#include <boost/circular_buffer.hpp>
#include <GlobalStateEnums.h>
/**@addtogroup epicsInterface
@{*/
class BPM;
typedef void(*updateFunctionPtr)(struct event_handler_args args);
/*! A class for monitoring, getting, and setting BPM related EPICS Process Variables via Channel Access*/
class EPICSBPMInterface : public EPICSInterface
{
public:
	/*! default constructor for EPICSBPMInterface.*/
	EPICSBPMInterface();
	/*! default destructor for EPICSBPMInterface.*/
	~EPICSBPMInterface();
	/*! sets the updateFunction pointer in pvStruct if we want to monitor that PV
	* @param[in] pvStruct : The pvStruct associated with the PV we want to monitor
	*/
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	/*! callback function for EPICS to update the x position from the EPICS PV in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateXPV(const struct event_handler_args args);
	/*! callback function for EPICS to update the y position from the EPICS PV in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
								  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateYPV(const struct event_handler_args args);
	/*! callback function for EPICS to update the raw data vector in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
								  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateData(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal set attenuation in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
								  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateSA1(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical set attenuation in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateSA2(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal set delay in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateSD1(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical set delay in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateSD2(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal read attenuation in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
								  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateRA1(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical read attenuation in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateRA2(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal read delay in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateRD1(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical read delay in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateRD2(const struct event_handler_args args);
	/*! callback function for EPICS to update the awake PV in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateAWAK(const struct event_handler_args args);
	/*! callback function for EPICS to update the ready PV in the BPM object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
							  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateRDY(const struct event_handler_args args);
	/*! sends a pulse (a long) to set the horizontal attenuation
	* @param[in] value : The desired horizontal attenuation
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setSA1(const long& value, const pvStruct& pv);
	/*! sends a pulse (a long) to set the vertical attenuation
	* @param[in] value : The desired vertical attenuation
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setSA2(const long& value, const pvStruct& pv);
	/*! sends a pulse (a long) to set the horizontal delay
	* @param[in] value : The desired horizontal delay
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setSD1(const long& value, const pvStruct& pv);
	/*! sends a pulse (a long) to set the vertical delay
	* @param[in] value : The desired vertical delay
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setSD2(const long& value, const pvStruct& pv);
    static LoggingSystem messenger;
};
/** \copydoc EPICSInterface*/
/** @}*/
#endif //EPICS_BPM_INTERFACE_H_