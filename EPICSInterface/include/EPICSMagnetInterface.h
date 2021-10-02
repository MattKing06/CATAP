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
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSMagnetInterface : public EPICSInterface
{
public:
	EPICSMagnetInterface();
	EPICSMagnetInterface(const EPICSMagnetInterface& copyInterface);
	~EPICSMagnetInterface();

	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;

	// setters
	bool setNewCurrent(const double &value, const pvStruct& pv) const;
	bool setNewKSetP(const double &value, const pvStruct& pv) const;
	bool setNewPSUState(const STATE value, const pvStruct& pv) const;
	bool resetILK(const pvStruct& pv) const;


	// EPICS calls these functions to update a variable
	static void updatePSUState(const struct event_handler_args args);
	static void updateGETSETI(const struct event_handler_args args);
	static void updateREADI(const struct event_handler_args args);
	static void updateRILK(const struct event_handler_args args);


	static void updateKDipP(const struct event_handler_args args);
	static void updateIntStr_mm(const struct event_handler_args args);
	static void updateIntStr(const struct event_handler_args args);
	static void updateKSetP(const struct event_handler_args args);
	static void updateKAng(const struct event_handler_args args);
	static void updateKVal(const struct event_handler_args args);
	static void updateKmrad(const struct event_handler_args args);


	// We need to a STATIC messenger so that the static call back functions can use it to print messages 
	static LoggingSystem messenger;
};

#endif //EPICS_MAGNET_INTERFACE_H_