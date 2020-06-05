#ifndef EPICS_CAMERA_INTERFACE_H_
#define EPICS_CAMERA_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSCameraInterface : public EPICSInterface
{
public:
	EPICSCameraInterface();
	~EPICSCameraInterface();
	std::string ownerName;
};

#endif //EPICS_CAMERA_INTERFACE_H_