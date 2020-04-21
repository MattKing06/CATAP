#ifndef HARDWARE_FACTORY_H_
#define HARDWARE_FACTORY_H_
#include "MagnetFactory.h"
#include "BPMFactory.h"
#include "ChargeFactory.h"
#include "ScreenFactory.h"
#include "ValveFactory.h"
#include "GlobalStateEnums.h"

/** @defgroup factories Factories
	@brief A collection of classes for configuring, storing, and performing operations on multiple hardwares.

	The factories associated with each hardware type are the main point of entry for the CATAP python libraries.
	They will setup up all of the hardware configurations using the ConfigReader class, and then setup the connections
	and subscriptions to EPICS (Virtual or CLARA) using each harwdare object's EPICSInterface. Once configured, the factories
	can perform operations on multiple hardware objects which are stored in their associated maps of hardware.
@{*/

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
	MagnetFactory& getMagnetFactory(TYPE machineArea);
	MagnetFactory& getMagnetFactory(const boost::python::list& machineAreas);
	MagnetFactory& getMagnetFactory(const std::vector<TYPE>& machineAreas);

	BPMFactory& getBPMFactory();
	ChargeFactory& getChargeFactory();
	ScreenFactory& getScreenFactory();
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
	ScreenFactory screenFactory;
	ValveFactory valveFactory;
	// virtual physical or offline
	STATE mode;
	LoggingSystem messenger;
private:

};

/** @} */
#endif // HardwareFactory_H_
