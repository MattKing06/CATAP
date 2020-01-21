#ifndef EPICS_MAGNET_INTERFACE_H_
#define EPICS_MAGNET_INTERFACE_H_
#ifndef MAGNET_H_
#include "Magnet.h"
#endif //MAGNET_H_
#ifndef EPICS_INTERFACE_H_
#include "EPICSInterface.h"
#endif 
#include <iostream>
#include <GlobalStateEnums.h>
// forward declaratiOn of Magnet class
// tells compiler that we will use this class.
class Magnet;
typedef void(*updateFunctiOnPtr)(struct event_handler_args args);

class EPICSMagnetInterface : public EPICSInterface
{
public:
	EPICSMagnetInterface();
	~EPICSMagnetInterface();

	void retrieveUpdateFunctiOnForRecord(pvStruct& pvStruct) const;

	bool setNewCurrent(const double &value, const pvStruct& pv) const;
	bool setNewPSUState(const STATE value, const pvStruct& pv) const;


	// EPICS calls these functiOns to update a variable
	static void updatePSUState(const struct event_handler_args args);
	static void updateGETSETI(const struct event_handler_args args);
	static void updateREADI(const struct event_handler_args args);
	static void updateRILK(const struct event_handler_args args);


	static LoggingSystem messenger;
};

#endif //EPICS_MAGNET_INTERFACE_H_