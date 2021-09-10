#ifndef EPICS_LIGHTING_INTERFACE_H_
#define EPICS_LIGHTING_INTERFACE_H_
#include <Lighting.h>
#include <LoggingSystem.h>
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif 
#include <string>



class EPICSLightingInterface : public EPICSInterface
{
public:
	EPICSLightingInterface();
	~EPICSLightingInterface();
	// We need to a STATIC messenger so that the static call back functions can use it to print messages 
	//static LoggingSystem messenger;
	
	/*! sets the updateFunction pointer in pvStruct if we want to monitor that PV
	* @param[in] pvStruct : The pvStruct associated with the PV we want to monitor	*/
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;

	// update functions 
	/*! callback function for EPICS to update the CLARA LED Status 
	* @param[in] event_handler_args	*/
	static void update_CLARA_LED_Sta(const struct event_handler_args args);
	/*! callback function for EPICS to update the VELA LED Status
	* @param[in] event_handler_args	*/
	static void update_VELA_LED_Sta(const struct event_handler_args args);

	/*! callback function for EPICS to update the VELA LED Status
	* @param[in] event_handler_args	*/
	static void update_ACCELERATOR_HALL_LIGHT_Sta(const struct event_handler_args args);

	/*! callback function for EPICS to update the VELA LED Status
	* @param[in] event_handler_args	*/
	static void update_BA1_LIGHT_Sta(const struct event_handler_args args);

	std::string ownerName;
};

#endif //EPICS_LIGHTING_INTERFACE_H_