#ifndef EPICS_SCREEN_INTERFACE_H_
#define EPICS_SCREEN_INTERFACE_H_
#ifndef SCREEN_H_
#include <Screen.h>
#endif //SCREEN_H_
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif 
#include <iostream>
#include <string>
#include <boost/circular_buffer.hpp>
#include <GlobalStateEnums.h>

/**@addtogroup epicsInterface
@{*/
class Screen;
typedef void(*updateFunctionPtr)(struct event_handler_args args);
/*! A class for monitoring, getting, and setting Screen related EPICS Process Variables via Channel Access*/
class EPICSScreenInterface : public EPICSInterface
{
public:
	/*! default constructor for EPICSScreenInterface.*/
	EPICSScreenInterface();
	/*! default destructor for EPICSScreenInterface.*/
	~EPICSScreenInterface();
	/*! sets the updateFunction pointer in pvStruct if we want to monitor that PV
	* @param[in] pvStruct : The pvStruct associated with the PV we want to monitor
	*/
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	/*! callback function for EPICS to update the horizontal set device in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHSDEV(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal trigger in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHTRIGGER(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal set move execution in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHEX(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal target actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHTGTPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal set jog position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHJOG(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal set position difference in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHJDIFF(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal moving parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHMOVING(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal ready parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHREADY(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal get device parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHGETDEV(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal device state in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHDEVSTATE(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal max position parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHMAXPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal device actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHDEVCENT(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal device actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHACTPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the horizontal stage enable state in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateHEN(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical set device in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVSDEV(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical trigger in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVTRIGGER(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical set move execution in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVEX(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical target actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVTGTPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical set position difference in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVJOG(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical set position difference in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVJDIFF(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical moving parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVMOVING(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical ready parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVREADY(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical get device parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVGETDEV(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical device state in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVDEVSTATE(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical max position parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVMAXPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical device actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVDEVCENT(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical device actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVACTPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical stage enable state in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateVEN(const struct event_handler_args args);
	/*! callback function for EPICS to update the vertical set device in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateSDEV(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic trigger in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateTRIGGER(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic set move execution in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateEX(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic target actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateTGTPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic set position difference in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateJOG(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic set position difference in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateJDIFF(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic moving parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateMOVING(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic ready parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateREADY(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic get device parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateGETDEV(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic device state in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateDEVSTATE(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic max position parameter in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateMAXPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic device actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateDEVCENT(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic device actuator position in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateACTPOS(const struct event_handler_args args);
	/*! callback function for EPICS to update the pneumatic stage enable state in the Screen object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateEN(const struct event_handler_args args);

	/*! sends a pulse (an integer associated with the screen device enum) to the PV associated with the state provided
	* @param[in] value : The state we want to set the screen to to (see GlobalStateEnums / ScreenPVRecords)
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setSDEV(const int value, const pvStruct& pv);
	/*! sends a pulse (1,0) to execute a move of actuator
	* @param[in] value : 1 to fire
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setEX(const int& value, const pvStruct& pv);
	/*! sends a pulse (1,0) enable the axis
	* @param[in] value : The axis to enable
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setEN(const int& value, const pvStruct& pv);
	/*! sends a pulse (1,0) to the PV associated with the screen trigger
	* @param[in] value : 1 to fire
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setTRIGGER(const int& value, const pvStruct& pv);
	/*! sends a pulse (a double) to the PV associated with the target position
	* @param[in] value : The desired target actuator position
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setTGTPOS(const double& value, const pvStruct& pv);
	/*! sends a pulse (a double) to the PV associated with the position deviation
	* @param[in] value : The current position deviation
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setJDIFF(const double& value, const pvStruct& pv);
	/*! sends a pulse (a double) to the PV associated with the requested position deviation
	* @param[in] value : The requested position deviation
	* @param[in] pv : The pvStruct associated with the screen state PV*/
	void setJOG(const double& value, const pvStruct& pv);
    static LoggingSystem static_messenger;
};
/** \copydoc EPICSInterface*/
/** @}*/
#endif //EPICS_Screen_INTERFACE_H_