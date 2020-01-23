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
// forward declaratiOn of BPM class
// tells compiler that we will use this class.
class Charge;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSChargeInterface : public EPICSInterface
{
public:
	EPICSChargeInterface();
	~EPICSChargeInterface();
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	static void updateQ(const struct event_handler_args args);
	static LoggingSystem messenger;
};

#endif //EPICS_CHARGE_INTERFACE_H_