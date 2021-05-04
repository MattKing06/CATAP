#ifndef HARDWARE_FACTORY_H_
#define HARDWARE_FACTORY_H_
#include "MagnetFactory.h"
#include "BPMFactory.h"
#include "ChargeFactory.h"
#include "ScreenFactory.h"
#include "ValveFactory.h"
#include "IMGFactory.h"
#include "LLRFFactory.h"
#include "ShutterFactory.h"
#include "CameraFactory.h"
#include "LaserEnergyMeterFactory.h"
#include "LaserHWPFactory.h"
#include "RFProtectionFactory.h"
#include "RFModulatorFactory.h"
#include <RFHeartbeatFactory.h>
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
	MagnetFactory& getMagnetFactory(TYPE machineArea);
	MagnetFactory& getMagnetFactory(const boost::python::list& machineAreas);
	MagnetFactory& getMagnetFactory(const std::vector<TYPE>& machineAreas);

	BPMFactory& getBPMFactory();
	ChargeFactory& getChargeFactory();
	ScreenFactory& getScreenFactory();
	ValveFactory& getValveFactory();
	IMGFactory& getIMGFactory();
	
	CameraFactory& getCameraFactory();

	CameraFactory& getCameraFactory_Single(const TYPE machineArea);
	CameraFactory& getCameraFactory_Mulitple_Py(const boost::python::list& machineAreas);
	CameraFactory& getCameraFactory_Mulitple(const std::vector<TYPE>& machineAreas);

	CameraFactory& getCameraFactory_ByName(std::string name);
	CameraFactory& getCameraFactory_ByNames_Py(const boost::python::list& names);
	CameraFactory& getCameraFactory_ByNames(const std::vector<std::string>& names);



	LaserEnergyMeterFactory& getLaserEnergyMeterFactory();
	LaserHWPFactory& getLaserHWPFactory();


	/*! The LLRF Factory requires at least 1 machine area to be instantiatated. 
	This is due to the ambiguity over which cavity a LLRF computer is driving 
	(a LLRF computer can dirve mutliple cavities but not simultaneoulsy). 
	Therefore YOU MUST specifiy which cavities you want to control and monitor 
	@param[in] TYPE, setup LLRF for a single cavity (machineArea)
	@param[out] LLRFFactory&, reference to LLRFFactory */
	LLRFFactory& getLLRFFactory_Single(const TYPE machineArea);
	/*! Generate a LLRF factor for multiple machineAreas.
	@param[in] vector<TYPE>, machineArea
	@param[out] LLRFFactory&, reference to LLRFFactory */
	LLRFFactory& getLLRFFactory(const std::vector<TYPE>& machineAreas);
	/*! Generate a LLRF factor for multiple machineAreas (python version).
	@param[in] list, machineArea
	@param[out] LLRFFactory&, reference to LLRFFactory */
	LLRFFactory& getLLRFFactory_Py(const boost::python::list& machineAreas);



	ShutterFactory& getShutterFactory();
		
	RFProtectionFactory& getRFProtectionFactory();
	RFModulatorFactory& getRFModulatorFactory();
	RFModulatorFactory& getRFModulatorFactory_Single(TYPE machine_area);
	RFModulatorFactory& getRFModulatorFactory_Py(const boost::python::list& machine_areas);
	RFModulatorFactory& getRFModulatorFactory(const std::vector<TYPE>& machine_areas);
	RFHeartbeatFactory& getRFHeartbeatFactory();



	bool saveMachineSnapshot();
	bool loadMachineSnapshot(const std::string& location);


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
	LaserEnergyMeterFactory laserEnergyMeterFactory;
	LaserHWPFactory laserHWPFactory;
	ShutterFactory shutterFactory;
	RFProtectionFactory rfProtectionFactory;
	RFModulatorFactory rfmodulatorFactory;
	RFHeartbeatFactory rfHeartbeatFactory;

	// virtual physical or offline
	STATE mode;
	LoggingSystem messenger;
private:

};

/** @} */
#endif // HardwareFactory_H_
