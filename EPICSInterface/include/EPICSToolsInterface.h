#ifndef EPICS_GENERAL_MONITOR_INTERFACE_H_
#define EPICS_GENERAL_MONITOR_INTERFACE_H_
#include <EPICSInterface.h>
#include <EPICSTools.h>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class EPICSTools;

class EPICSToolsInterface : public EPICSInterface
{
public:
	EPICSToolsInterface();
	~EPICSToolsInterface();
	std::string ownerName;
	static void updateValue(const struct event_handler_args args);
	void retrieveUpdateFunction(pvStruct& pv);
};

#endif //EPICS_GENERAL_MONITOR_INTERFACE_H_


