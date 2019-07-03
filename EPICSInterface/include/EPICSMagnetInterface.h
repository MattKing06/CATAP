#ifndef EPICS_MAGNET_INTERFACE_H_
#define EPICS_MAGNET_INTERFACE_H_
#include "EPICSInterface.h"
#include <iostream>

class EPICSMagnetInterface : public EPICSInterface
{
public:
	EPICSMagnetInterface();
	void setCurrent(double value);
	static void updateValue(const event_handler_args args);
	static void updateCurrent(const event_handler_args args);
	LoggingSystem messenger;
    static double latestCurrentFromEPICS;
};

#endif //EPICS_MAGNET_INTERFACE_H_