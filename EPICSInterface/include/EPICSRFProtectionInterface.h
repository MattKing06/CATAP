#ifndef EPICS_RF_PROTECTION_INTERFACE_H_
#define EPICS_RF_PROTECTION_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSRFProtectionInterface : public EPICSInterface
{
public:
	EPICSRFProtectionInterface();
	~EPICSRFProtectionInterface();
	std::string ownerName;
};

#endif //EPICS_RF_PROTECTION_INTERFACE_H_