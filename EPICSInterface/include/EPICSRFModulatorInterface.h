#ifndef EPICS_RF_MODULATOR_INTERFACE_H_
#define EPICS_RF_MODULATOR_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSRFModulatorInterface : EPICSInterface
{
public:
	EPICSRFModulatorInterface();
	~EPICSRFModulatorInterface();
	std::string ownerName;
};

#endif //EPICS_RF_MODULATOR_INTERFACE_H_