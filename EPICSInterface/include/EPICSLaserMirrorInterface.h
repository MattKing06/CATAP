#ifndef EPICS_LASER_MIRROR_INTERFACE_H_
#define EPICS_LASER_MIRROR_INTERFACE_H_
#include <EPICSInterface.h>
#include <LaserMirror.h>
#include <GlobalStateEnums.h>

class EPICSLaserMirrorInterface : public EPICSInterface
{
public:
	EPICSLaserMirrorInterface();
	~EPICSLaserMirrorInterface();
	std::string ownerName;
	void retrieveUpdateFunctionForRecord(pvStruct& pvStruct);
	static void updateHorizontalPosition(const struct event_handler_args args);
	static void updateVerticalPosition(const struct event_handler_args args);
	void setNewHorizontalPosition(const double& value, const pvStruct& pv);
	void setNewVerticalPosition(const double& value, const pvStruct& pv);
	void moveHorizontal(const pvStruct& pv);
	void moveVertical(const pvStruct& pv);
};

#endif //EPICS_LASER_MIRROR_INTERFACE_H_