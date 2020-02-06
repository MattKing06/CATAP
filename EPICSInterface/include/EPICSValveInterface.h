#ifndef EPICS_VALVE_INTERFACE_H_
#define EPICS_VALVE_INTERFACE_H_
#ifndef VALVE_H_
#include <Valve.h>
#endif // VALVE
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif // EPICS INTERFACE

#include <iostream>
#include <GlobalStateEnums.h>


class Valve;
typedef void(*updateFunctionPtr)(struct event_handler_args);

class EPICSValveInterface : public EPICSInterface
{
public:
	EPICSValveInterface();
	~EPICSValveInterface();

	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	bool setNewValveState(const STATE& value, const pvStruct& pv);
	static void updateValveState(const struct event_handler_args);

	static LoggingSystem messenger;



};

#endif //EPICS_VALVE_INTERFACE_H_