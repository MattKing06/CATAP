#ifndef EPICS_MAGNET_INTERFACE_H_
#define EPICS_MAGNET_INTERFACE_H_
#ifndef MAGNET_H_
#include "Magnet.h"
#endif //MAGNET_H_
#ifndef EPICS_INTERFACE_H_
#include "EPICSInterface.h"
#endif 
#include <iostream>
// forward declaration of Magnet class
// tells compiler that we will use this class.
class Magnet;
class EPICSMagnetInterface : public EPICSInterface
{
public:
	EPICSMagnetInterface();
	void setCurrent(double value);
	static void updateValue(struct event_handler_args args);
	static void updateCurrent(struct event_handler_args args);
	LoggingSystem messenger;
	static Magnet* owner;
};

#endif //EPICS_MAGNET_INTERFACE_H_