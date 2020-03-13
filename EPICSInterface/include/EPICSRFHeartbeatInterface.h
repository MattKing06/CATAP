#ifndef EPICS_RF_HEARTBEAT_INTERFACE_H_
#define EPICS_RF_HEARTBEAT_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSRFHeartbeatInterface : EPICSInterface
{
public:
	EPICSRFHeartbeatInterface();
	~EPICSRFHeartbeatInterface();
	std::string ownerName;
};

#endif //EPICS_RF_HEARTBEAT_INTERFACE_H_