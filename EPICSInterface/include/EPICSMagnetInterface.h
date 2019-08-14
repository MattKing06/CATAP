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
	updateFunctionPtr retrieveUpdateFunctionForRecord(const std::string &record);
	static void updateCurrent(const struct event_handler_args args);
	const void setNewCurrent(const double &value, const pvStruct &pv);
	template<typename T>
	const void putValue(const chtype &CHTYPE, const chid &CHID, const T &value);
	static LoggingSystem messenger;
};

#endif //EPICS_MAGNET_INTERFACE_H_