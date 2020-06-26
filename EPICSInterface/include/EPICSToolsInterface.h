#ifndef EPICS_GENERAL_MONITOR_INTERFACE_H_
#define EPICS_GENERAL_MONITOR_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSToolsInterface : public EPICSInterface
{
public:
	EPICSToolsInterface();
	~EPICSToolsInterface();
	std::string ownerName;
};

#endif //EPICS_GENERAL_MONITOR_INTERFACE_H_



