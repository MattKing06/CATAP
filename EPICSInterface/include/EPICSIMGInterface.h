#ifndef EPICS_IMG_INTERFACE_H_
#define EPICS_IMG_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSIMGInterface : public EPICSInterface
{
public:
	EPICSIMGInterface();
	~EPICSIMGInterface();
	std::string ownerName;
};

#endif //EPICS_IMG_INTERFACE_H_