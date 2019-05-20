#ifndef EPICS_MAGNET_INTERFACE_H_
#define EPICS_MAGNET_INTERFACE_H_
#include "EPICSInterface.h"


class EPICSMagnetInterface : public EPICSInterface
{
public:
	EPICSMagnetInterface();
	void setCurrent(double value);
	double getCurrent(chid &channelID);

	LoggingSystem messenger;
};

#endif //EPICS_MAGNET_INTERFACE_H_