#ifndef EPICS_IMG_INTERFACE_H_
#define EPICS_IMG_INTERFACE_H_
#include <EPICSInterface.h>
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif // EPICS INTERFACE
#ifndef IMG_H_
#include <IMG.h>
#endif // IMG

#include <iostream>
#include <GlobalStateEnums.h>
/**@addtogroup epicsInterface
@{*/
class IMG;
typedef void(*updateFunctionPtr)(struct event_handler_args args);
/*! A class for monitoring, getting, and settings IMG related EPICS Process Variables via Channel Access*/
class EPICSIMGInterface : public EPICSInterface
{
public:
	/*! default constructor for EPICSIMGInterface.*/
	EPICSIMGInterface();
	/*! default destructor for EPICSIMGInterface*/
	~EPICSIMGInterface();
	std::string ownerName;
	/*! sets the updateFunction pointer in pvStruct if we want to monitor that PV
	* @param[in] pvStruct : The pvStruct associated with the PV we want to monitor
	*/
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	/*! callback function for EPICS to update the IMGPressure in the IMG object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateIMGP(const struct event_handler_args args);
	/*! callback function for EPICS to update the IMGState in the IMG object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateIMGState(const struct event_handler_args);
	/*! messenger object for EPICSValveInterface messaging. */
	static LoggingSystem messenger;

};
/** \copydoc EPICSInterface*/
/** @}*/
#endif //EPICS_IMG_INTERFACE_H_