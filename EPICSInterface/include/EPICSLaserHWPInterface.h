#ifndef EPICS_LASER_HWP_INTERFACE_H_
#define EPICS_LASER_HWP_INTERFACE_H_
#ifndef LASER_HWP_H_
#include <LaserHWP.h>
#endif //LASER_H_
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif 
#include <iostream>
#include <string>
#include <GlobalStateEnums.h>
// forward declaratiOn of laser class
// tells compiler that we will use this class.
class LaserHWP;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSLaserHWPInterface : public EPICSInterface
{
public:
	EPICSLaserHWPInterface();
	~EPICSLaserHWPInterface();
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	static void updatePositions(const struct event_handler_args args);
	static void updateHWPSet(const struct event_handler_args args);
	static void updateHWPGet(const struct event_handler_args args);
	static void updateENABLE(const struct event_handler_args args);
	void setMABS(const double& value, const pvStruct& pv);
	static LoggingSystem messenger;
};

#endif //EPICS_LASER_HWP_INTERFACE_H_