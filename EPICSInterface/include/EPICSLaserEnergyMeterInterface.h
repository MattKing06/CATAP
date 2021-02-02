#ifndef EPICS_LASER_ENERGY_METER_INTERFACE_H_
#define EPICS_LASER_ENERGY_METER_INTERFACE_H_
#ifndef LASER_ENERGY_METER_H_
#include <LaserEnergyMeter.h>
#endif //LASER_ENERGY_METER_H_
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif 
#include <iostream>
#include <string>
#include <GlobalStateEnums.h>
// forward declaratiOn of laser class
// tells compiler that we will use this class.
class LaserEnergyMeter;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

/*! A class for monitoring, getting, and setting laser energy meter-related EPICS Process Variables via Channel Access*/
class EPICSLaserEnergyMeterInterface : public EPICSInterface
{
public:
	/*! default constructor for EPICSLaserEnergyMeterInterface.*/
	EPICSLaserEnergyMeterInterface();
	/*! default destructor for EPICSLaserEnergyMeterInterface.*/
	~EPICSLaserEnergyMeterInterface();
	/*! sets the updateFunction pointer in pvStruct if we want to monitor that PV
	* @param[in] pvStruct : The pvStruct associated with the PV we want to monitor
	*/
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	/*! callback function for EPICS to update the start button from the EPICS PV in the laser energy meter object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateStart(const struct event_handler_args args);
	/*! callback function for EPICS to update the overrange parameter from the EPICS PV in the laser energy meter object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateOverRange(const struct event_handler_args args);
	/*! callback function for EPICS to update the range of the EPICS PV in the laser energy meter object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateRange(const struct event_handler_args args);
	/*! callback function for EPICS to update laser energy from the EPICS PV in the laser energy meter object returned from event_handler_args
	* @param[in] event_handler_args : an EPICS object that holds the value of the PV and the Hardware object passed to it
									  when the subscription was set up in the EPICSInterface createSubscription function.
	*/
	static void updateEnergy(const struct event_handler_args args);
	/*! sends a pulse (an int) to set the start button
	* @param[in] value : The desired value (0 for stop, 1 for start)
	* @param[in] pv : The pvStruct associated with the laser energy start PV*/
	void setSTART(const int& value, const pvStruct& pv);
	/*! sends a pulse (an int) to set the range
	* @param[in] value : The desired range -- 0 = 20nJ, 1 = 200nJ, 2 = 2uJ, 3 = 20uJ
	* @param[in] pv : The pvStruct associated with the laser energy meter range PV*/
	void setRANGE(const int& value, const pvStruct& pv);
	static LoggingSystem messenger;
};

#endif //EPICS_LASER_ENERGY_METER_INTERFACE_H_