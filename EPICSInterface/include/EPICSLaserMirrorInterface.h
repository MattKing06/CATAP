#ifndef EPICS_LASER_MIRROR_INTERFACE_H_
#define EPICS_LASER_MIRROR_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSLaserMirrorInterface : public EPICSInterface
{
public:
	EPICSLaserMirrorInterface();
	~EPICSLaserMirrorInterface();
	std::string ownerName;
};

#endif //EPICS_LASER_MIRROR_INTERFACE_H_