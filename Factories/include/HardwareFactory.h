#ifndef HARDWARE_FACTORY_H_
#define HARDWARE_FACTORY_H_
#include "MagnetFactory.h"
#include "BPMFactory.h"
#include "ChargeFactory.h"

class HardwareFactory
{
    //what does a HardwareFactory need to be a HardwareFactory:
    /*
    *   - A Hardware HardwareFactory Type (ENUM/DEFINES?)
    *   - A Hardware HardwareFactory Name (string/ENUM?)
    *   - Needs to invoke LoggingSystem object without having it as an inherited member.
    *
    */
public:
	HardwareFactory();
	HardwareFactory(bool isVirtual);
	HardwareFactory(std::string hardwareType, std::string version);
	~HardwareFactory();
	bool setup(const std::string& hardwareType, const std::string& version);
	MagnetFactory& getMagnetFactory();
	BPMFactory& getBPMFactory();
	ChargeFactory& getChargeFactory();
	bool operator ==(const HardwareFactory &HardwareFactory) const;
	LoggingSystem messenger;
	//std::vector<Hardware*> hardwareVector;
	std::map<std::string, Hardware> hardwareMap;
	MagnetFactory magnetFactory;
	BPMFactory bpmFactory;
	ChargeFactory chargeFactory;
	bool isVirtual;
};


#endif // HardwareFactory_H_
