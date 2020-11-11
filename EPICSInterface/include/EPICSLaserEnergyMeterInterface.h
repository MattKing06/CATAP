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

class EPICSLaserEnergyMeterInterface : public EPICSInterface
{
public:
	EPICSLaserEnergyMeterInterface();
	~EPICSLaserEnergyMeterInterface();
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	static void updateStart(const struct event_handler_args args);
	static void updateOverRange(const struct event_handler_args args);
	static void updateRange(const struct event_handler_args args);
	void setSTART(const int& value, const pvStruct& pv);
	void setRANGE(const int& value, const pvStruct& pv);
	static LoggingSystem messenger;
};

#endif //EPICS_LASER_ENERGY_METER_INTERFACE_H_