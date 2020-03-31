#ifndef EPICS_STAGE_INTERFACE_H_
#define EPICS_STAGE_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSStageInterface : EPICSInterface
{
public:
	EPICSStageInterface();
	~EPICSStageInterface();
	std::string ownerName;
};

#endif //EPICS_STAGE_INTERFACE_H_