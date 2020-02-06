#ifndef HARDWARE_FACTORY_H_
#define HARDWARE_FACTORY_H_
#include "MagnetFactory.h"
#include "BPMFactory.h"
#include "ChargeFactory.h"
#include "ValveFactory.h"
#include "GlobalStateEnums.h"

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
	HardwareFactory(STATE mode);
	//HardwareFactory(std::string hardwareType, std::string VERSION);
	~HardwareFactory();
	bool setup(const std::string& hardwareType, const std::string& VERSION);
	MagnetFactory& getMagnetFactory();
	BPMFactory& getBPMFactory();
	ChargeFactory& getChargeFactory();
	ValveFactory& getValveFactory();
	bool operator ==(const HardwareFactory& HardwareFactory) const;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();
	std::map<std::string, Hardware> hardwareMap;
	MagnetFactory magnetFactory;
	BPMFactory bpmFactory;
	ChargeFactory chargeFactory;
	ValveFactory valveFactory;
	// virtual physical or offline
	STATE mode;
	LoggingSystem messenger;
private:

};


#endif // HardwareFactory_H_
