#ifndef EPICS_LED_INTERFACE_H_
#define EPICS_LED_MONITOR_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSLEDInterface : EPICSInterface
{
public:
	EPICSLEDInterface();
	~EPICSLEDInterface();
	std::string ownerName;
};

#endif //EPICS_LED_INTERFACE_H_