#ifndef EPICS_SHUTTER_INTERFACE_H_
#define EPICS_SHUTTER_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSShutterInterface : public EPICSInterface
{
public:
	EPICSShutterInterface();
	~EPICSShutterInterface();
	std::string ownerName;
};

#endif //EPICS_SHUTTER_INTERFACE_H_