#ifndef EPICS_LIGHTING_INTERFACE_H_
#define EPICS_LIGHTING_MONITOR_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSLightingInterface : EPICSInterface
{
public:
	EPICSLightingInterface();
	~EPICSLightingInterface();
	std::string ownerName;
};

#endif //EPICS_LIGHTING_INTERFACE_H_