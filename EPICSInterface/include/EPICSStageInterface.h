#ifndef EPICS_STAGE_INTERFACE_H_
#define EPICS_STAGE_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSStageInterface : public EPICSInterface
{
public:
	EPICSStageInterface();
	~EPICSStageInterface();
	std::string ownerName;
};

#endif //EPICS_STAGE_INTERFACE_H_