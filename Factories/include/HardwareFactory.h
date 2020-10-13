#ifndef HARDWARE_FACTORY_H_
#define HARDWARE_FACTORY_H_
#include "MagnetFactory.h"
#include "BPMFactory.h"
#include "ChargeFactory.h"
#include "ScreenFactory.h"
#include "ValveFactory.h"
#include "IMGFactory.h"
#include "LLRFFactory.h"
#include "CameraFactory.h"
#include "GlobalStateEnums.h"
#include "GlobalTypeEnums.h"

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
	BPMFactory& getBPMFactory();
	ChargeFactory& getChargeFactory();
	ScreenFactory& getScreenFactory();
	ValveFactory& getValveFactory();
	IMGFactory& getIMGFactory();
	
	CameraFactory& getCameraFactory();
	CameraFactory& getCameraFactory_Single(const TYPE machineArea);
	CameraFactory& getCameraFactory_Mulitple_Py(const boost::python::list& machineAreas);
	CameraFactory& getCameraFactory_Mulitple(const std::vector<TYPE>& machineAreas);


	LLRFFactory& getLLRFFactory_Single(const TYPE machineArea);
	LLRFFactory& getLLRFFactory_Py(const boost::python::list& machineAreas);
	LLRFFactory& getLLRFFactory(const std::vector<TYPE>& machineAreas);
	bool operator ==(const HardwareFactory& HardwareFactory) const;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();
	std::map<std::string, Hardware> hardwareMap;
	MagnetFactory magnetFactory;
	ChargeFactory chargeFactory;
	ScreenFactory screenFactory;
	ValveFactory valveFactory;
	IMGFactory imgFactory;
	LLRFFactory llrffactory;
	BPMFactory bpmFactory;
	CameraFactory cameraFactory;
	// virtual physical or offline
	STATE mode;
	LoggingSystem messenger;
private:

};

/** @} */
#endif // HardwareFactory_H_
