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
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSMagnetInterface : public EPICSInterface
{
public:
	EPICSMagnetInterface();
	~EPICSMagnetInterface();
	void retrieveUpdateFunctionForRecord(pvStruct& pvStruct) const;
	static void updateCurrent(const struct event_handler_args args);
	void setNewCurrent(const double &value, const pvStruct &pv) const;
	template<typename T>
	void putValue(const pvStruct& pvStruct, const T& value) const;
	static LoggingSystem messenger;
};

#endif //EPICS_MAGNET_INTERFACE_H_