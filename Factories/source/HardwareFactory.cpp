#include "HardwareFactory.h"
#include <boost/filesystem/convenience.hpp>
#include "PythonTypeConversions.h"
#include "GlobalFunctions.h"
#include <SnapshotFileManager.h>

HardwareFactory::HardwareFactory() : HardwareFactory(STATE::OFFLINE)
{
}
HardwareFactory::~HardwareFactory()
{
	messenger.printDebugMessage("HardwareFactory Destruction Called");
}
HardwareFactory::HardwareFactory(STATE mode) :
	HardwareFactory(mode, MASTER_LATTICE_FILE_LOCATION)
{}

HardwareFactory::HardwareFactory(STATE mode, const std::string& primeLatticeLocation) :
messenger(LoggingSystem(true, true)),
magnetFactory(MagnetFactory(mode, primeLatticeLocation)),
bpmFactory(BPMFactory(mode, primeLatticeLocation)),
chargeFactory(ChargeFactory(mode, primeLatticeLocation)),
screenFactory(ScreenFactory(mode, primeLatticeLocation)),
valveFactory(ValveFactory(mode, primeLatticeLocation)),
imgFactory(IMGFactory(mode, primeLatticeLocation)),
rfProtectionFactory(RFProtectionFactory(mode, primeLatticeLocation)),
llrffactory(LLRFFactory(mode, primeLatticeLocation)),
cameraFactory(CameraFactory(mode, primeLatticeLocation)),
laserEnergyMeterFactory(LaserEnergyMeterFactory(mode, primeLatticeLocation)),
laserHWPFactory(LaserHWPFactory(mode, primeLatticeLocation)),
shutterFactory(ShutterFactory(mode, primeLatticeLocation)),
rfmodulatorFactory(RFModulatorFactory(mode, primeLatticeLocation)),
rfHeartbeatFactory(RFHeartbeatFactory(mode, primeLatticeLocation)),
lightingFactory(LightingFactory(mode, primeLatticeLocation)),
stageFactory(StageFactory(mode, primeLatticeLocation)),
mode(mode)
{
	messenger.printDebugMessage("Hardware Factory constructed, mode = ", ENUM_TO_STRING(mode));
}

bool HardwareFactory::setup(const std::string& VERSION)
{
	bool setup = false;
	// TODO these TYPES should be the type ENUM
	if (!magnetFactory.hasBeenSetup)
	{
		setup = magnetFactory.setup(VERSION);
	}
	messenger.printMessage("MagnetFactory has been setup.");
	if (!bpmFactory.hasBeenSetup)
	{
		setup = bpmFactory.setup(VERSION);
	}
	messenger.printMessage("BPMFactory has been setup.");
	if (!chargeFactory.hasBeenSetup)
	{
		setup = chargeFactory.setup(VERSION);
	}
	messenger.printMessage("ChargeFactory has been setup.");
	if (!screenFactory.hasBeenSetup)
	{
		setup = screenFactory.setup(VERSION);
	}
	messenger.printMessage("ScreenFactory has been setup.");
	if (!valveFactory.hasBeenSetup)
	{
		setup = valveFactory.setup(VERSION);
	}
	messenger.printMessage("ValveFactory has been setup.");
	if (!laserEnergyMeterFactory.hasBeenSetup)
	{
		setup = laserEnergyMeterFactory.setup(VERSION);
	}
	messenger.printMessage("LaserEnergyMeterFactory has been setup.");
	if (!laserHWPFactory.hasBeenSetup)
	{
		setup = laserHWPFactory.setup(VERSION);
	}
	messenger.printMessage("LaserHWPFactory has been setup.");
	if (!stageFactory.hasBeenSetup)
	{
		setup = stageFactory.setup(VERSION);
	}
	messenger.printMessage("StageFactory has been setup.");
	return setup;
}

bool HardwareFactory::setup(const std::string& hardwareType, const std::string& VERSION)
{
	bool setup = false;
	// TODO these TYPES should be the type ENUM
	if (hardwareType == "Magnet")
	{
		if (!magnetFactory.hasBeenSetup)
		{
			setup = magnetFactory.setup(VERSION);
		}
		messenger.printMessage("MagnetFactory has been setup.");
	}
	else if (hardwareType == "BPM")
	{
		if (!bpmFactory.hasBeenSetup)
		{
			setup = bpmFactory.setup(VERSION);
		}
		messenger.printMessage("BPMFactory has been setup.");
	}
	else if (hardwareType == "Charge")
	{
		if (!chargeFactory.hasBeenSetup)
		{
			setup = chargeFactory.setup(VERSION);
		}
		messenger.printMessage("ChargeFactory has been setup.");
	}
	else if (hardwareType == "Screen")
	{
		if (!screenFactory.hasBeenSetup)
		{
			setup = screenFactory.setup(VERSION);
		}
		messenger.printMessage("ScreenFactory has been setup.");
	}
	else if (hardwareType == "Valve")
	{
		if (!valveFactory.hasBeenSetup)
		{
			setup = valveFactory.setup(VERSION);
		}
		messenger.printMessage("ValveFactory has been setup.");
	}
	else if (hardwareType == "Laser Energy Meter")
	{
		if (!laserEnergyMeterFactory.hasBeenSetup)
		{
			setup = laserEnergyMeterFactory.setup(VERSION);
		}
		messenger.printMessage("LaserEnergyMeterFactory has been setup.");
	}
	else if (hardwareType == "Laser HWP")
	{
		if (!laserHWPFactory.hasBeenSetup)
		{
			setup = laserHWPFactory.setup(VERSION);
		}
		messenger.printMessage("LaserHWPFactory has been setup.");
	}
	else if (hardwareType == "Lighting")
	{
		if (!lightingFactory.hasBeenSetup)
		{
			setup = lightingFactory.setup(VERSION);
		}
		messenger.printMessage("lightingFactory has been setup.");
	}
	else if (hardwareType == "Stage")
	{
		if (!stageFactory.hasBeenSetup)
		{
			setup = stageFactory.setup(VERSION);
		}
		messenger.printMessage("StageFactory has been setup.");
	}
	return setup;
}

bool HardwareFactory::setup(const std::vector<std::string>& hardwareTypes, const std::string& VERSION)
{
	for (auto&& hardware : hardwareTypes)
	{
		bool status = setup(hardware, VERSION);
	}
	return false;
}

bool HardwareFactory::setup(const boost::python::list& hardwareTypes, const std::string& VERSION)
{
	std::vector<std::string> hardware = to_std_vector<std::string>(hardwareTypes);
	setup(hardware, VERSION);
	return false;
}

RFProtectionFactory& HardwareFactory::getRFProtectionFactory()
{
	messenger.printMessage("getRFProtectionFactory Called");
	if (!rfProtectionFactory.hasBeenSetup)
	{
		messenger.printMessage("getRFProtectionFactory calling setup");
		bool setup = rfProtectionFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getRFProtectionFactory Complete");
			return rfProtectionFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup RFProtectionFactory, Hopefully you'll never see this");
		}
	}
	else
	{
		messenger.printMessage("getRFProtectionFactory Complete");
		return rfProtectionFactory;
	}
}

StageFactory& HardwareFactory::getStageFactory()
{
	messenger.printMessage("getStageFactory Called");
	if (!stageFactory.hasBeenSetup)
	{
		messenger.printMessage("getStageFactory calling setup");
		bool setup = stageFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getStageFactory Complete");
			return stageFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup StageFactory, Hopefully you'll never see this");
		}
	}
	else
	{
		messenger.printMessage("getStageFactory Complete");
		return stageFactory;
	}
}

RFModulatorFactory& HardwareFactory::getRFModulatorFactory()
{
	return 	getRFModulatorFactory_Single(TYPE::ALL_VELA_CLARA);
}
RFModulatorFactory& HardwareFactory::getRFModulatorFactory_Single(TYPE machine_areas)
{
	return 	getRFModulatorFactory(std::vector<TYPE>{machine_areas});
}
RFModulatorFactory& HardwareFactory::getRFModulatorFactory_Py(const boost::python::list& machine_areas)
{
	return getRFModulatorFactory(to_std_vector<TYPE>(machine_areas));
}
RFModulatorFactory& HardwareFactory::getRFModulatorFactory(const std::vector<TYPE>& machine_areas)
{
	messenger.printMessage("getRFProtectionFactory Called");
	if (!rfmodulatorFactory.hasBeenSetup)
	{
		messenger.printMessage("getRFModulatorFactory calling setup");
		bool setup = rfmodulatorFactory.setup("nominal", machine_areas);
		if (setup)
		{
			messenger.printMessage("getRFProtectionFactory Complete");
			return rfmodulatorFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup RFModulatorFactory, Hopefully you'll never see this");
		}
	}
	else
	{
		messenger.printMessage("getRFModulatorFactory Complete");
		return rfmodulatorFactory;
	}
}

ShutterFactory& HardwareFactory::getShutterFactory()
{
	messenger.printMessage("getShutterFactory Called");
	if (!shutterFactory.hasBeenSetup)
	{
		messenger.printMessage("getShutterFactory calling setup");
		bool setup = shutterFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getShutterFactory Complete");
			return shutterFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup ShutterFactory, Hopefully you'll never see this");
		}
	}
	else
	{
		messenger.printMessage("getShutterFactory Complete");
		return shutterFactory;
	}
}

// YOU MUST define a machine area to get a LLRF factory, you CANNOT just get them all
LLRFFactory& HardwareFactory::getLLRFFactory_Single(const TYPE machineArea)
{
	return getLLRFFactory(std::vector<TYPE>{machineArea});
}
LLRFFactory& HardwareFactory::getLLRFFactory_Py(const boost::python::list& machineAreas)
{
	return getLLRFFactory(to_std_vector<TYPE>(machineAreas));
}
LLRFFactory& HardwareFactory::getLLRFFactory(const std::vector<TYPE>& machineAreas)
{
	messenger.printMessage("getLLRFFactory Called");
	if (!llrffactory.hasBeenSetup)
	{
		bool setup = llrffactory.setup("nominal", machineAreas);
		if (setup)
		{
			return llrffactory;
		}
		else
		{
			messenger.printMessage("Unable to setup LLRFFactory, Hopefully you'll never see this");
		}
	}
	else
	{
		return llrffactory;
	}
}




MagnetFactory& HardwareFactory::getMagnetFactory()
{
	return getMagnetFactory(std::vector<TYPE>{TYPE::ALL_VELA_CLARA});
}
MagnetFactory& HardwareFactory::getMagnetFactory(TYPE machineArea)
{
	return getMagnetFactory(std::vector<TYPE>{machineArea});
}
MagnetFactory& HardwareFactory::getMagnetFactory(const boost::python::list& machineAreas)
{
	return getMagnetFactory(to_std_vector<TYPE>(machineAreas));
}
MagnetFactory& HardwareFactory::getMagnetFactory(const std::vector<TYPE>& machineAreas )
{
	if (!magnetFactory.hasBeenSetup)
	{
		messenger.printMessage("magnetFactory is being setup");
		bool setup = magnetFactory.setup(GlobalConstants::nominal, machineAreas);

		// messenger.printMessage("Setup magnet Factory Nominal");
		// bool setup = magnetFactory.setup("nominal");

		if(setup)
		{
			messenger.printMessage("magnetFactory setup complete");
			return magnetFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup MagnetFactory, Hopefully you'll never see this");
		}
	}
	else
	{
		messenger.printMessage("magnetFactory has already been setup");
		return magnetFactory;
	}
}

LightingFactory& HardwareFactory::getLightingFactory()
{
	if (!lightingFactory.hasBeenSetup)
	{
		bool setup = lightingFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getLightingFactory Complete");
			return lightingFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup LightingFactory");
		}
	}
	else
	{
		messenger.printMessage("getLightingFactory Complete");
		return lightingFactory;
	}
}

ValveFactory& HardwareFactory::getValveFactory()
{
	if (!valveFactory.hasBeenSetup)
	{
		bool setup = valveFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getValveFactory Complete");
			return valveFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup ValveFactory");
		}
	}
	else
	{
		messenger.printMessage("getValveFactory Complete");
		return valveFactory;
	}
}
IMGFactory& HardwareFactory::getIMGFactory()
{
	if (!imgFactory.hasBeenSetup)
	{
		bool setup = imgFactory.setup("nominal");
		if (setup)
		{
			return imgFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup IMGFactory");
		}
	}
	else
	{
		return imgFactory;
	}
}


BPMFactory& HardwareFactory::getBPMFactory()
{
	if (!bpmFactory.hasBeenSetup)
	{
		bool setup = bpmFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getBPMFactory Complete");
			return bpmFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup BPMFactory");
		}
	}
	else
	{
		messenger.printMessage("getBPMFactory Complete");
		return bpmFactory;
	}
}
ChargeFactory& HardwareFactory::getChargeFactory()
{
	if (!chargeFactory.hasBeenSetup)
	{
		bool setup = chargeFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getChargeFactory Complete");
			return chargeFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup ChargeFactory");
		}
	}
	return chargeFactory;
}
ScreenFactory& HardwareFactory::getScreenFactory()
{
	if (!screenFactory.hasBeenSetup)
	{
		bool setup = screenFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getScreenFactory Complete");
			return screenFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup ScreenFactory");
		}
	}
	return screenFactory;
}

LinacPIDFactory& HardwareFactory::getLinacPIDFactory()
{
	if (!linacPIDFactory.hasBeenSetup)
	{
		bool setup = linacPIDFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("linacPIDFactory Complete");
			return linacPIDFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup linacPIDFactory");
		}
	}
	else
	{
		messenger.printMessage("linacPIDFactory Complete");
		return linacPIDFactory;
	}
}



CameraFactory& HardwareFactory::getCameraFactory()
{
	return getCameraFactory_Single(TYPE::ALL_VELA_CLARA);
}
CameraFactory& HardwareFactory::getCameraFactory_Single(const TYPE machineArea)
{
	const std::vector<TYPE> machineAreas{ machineArea };
	return getCameraFactory_Mulitple(machineAreas);
}
CameraFactory& HardwareFactory::getCameraFactory_Mulitple_Py(const boost::python::list& machineAreas)
{
	return getCameraFactory_Mulitple(to_std_vector<TYPE>(machineAreas));
}
CameraFactory& HardwareFactory::getCameraFactory_Mulitple(const std::vector<TYPE>& machineAreas)
{
	messenger.printDebugMessage("");
	std::stringstream ss;
	ss << "getCameraFactory_Mulitple  passed machine areas = ";
	for (auto&& area : machineAreas)
	{
		ss << ENUM_TO_STRING(area);
		ss << ", ";
	}
	messenger.printDebugMessage(ss.str());
	if (!cameraFactory.hasBeenSetup)
	{
		bool setup = cameraFactory.setup("nominal", machineAreas);
		if (setup)
		{
			messenger.printMessage("getCameraFactory Complete");
			return cameraFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup cameraFactory");
		}
	}
	else
	{
		messenger.printMessage("cameraFactory has already been setup!");
	}
	return cameraFactory;
}

CameraFactory& HardwareFactory::getCameraFactory_ByName(std::string name)
{
	const std::vector<std::string> names{ name };
	return getCameraFactory_ByNames(names);
}
CameraFactory& HardwareFactory::getCameraFactory_ByNames_Py(const boost::python::list& names)
{
	return getCameraFactory_ByNames(to_std_vector<std::string>(names));
}
CameraFactory& HardwareFactory::getCameraFactory_ByNames(const std::vector<std::string>& names)
{
	messenger.printDebugMessage("");
	std::stringstream ss;
	ss << "getCameraFactory_ByNames  passed names = ";
	for (auto&& name : names)
	{
		ss << name;
		ss << ", ";
	}
	messenger.printDebugMessage(ss.str());
	if (!cameraFactory.hasBeenSetup)
	{
		bool setup = cameraFactory.setup("nominal", names);
		if (setup)
		{
			messenger.printMessage("getCameraFactory Complete");
			return cameraFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup cameraFactory");
		}
	}
	return cameraFactory;
}




LaserEnergyMeterFactory& HardwareFactory::getLaserEnergyMeterFactory()
{
	if (!laserEnergyMeterFactory.hasBeenSetup)
	{
		bool setup = laserEnergyMeterFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getlaserEnergyMeterFactory Complete");
			return laserEnergyMeterFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup laserEnergyMeterFactory");
		}
	}
	return laserEnergyMeterFactory;
}

LaserHWPFactory& HardwareFactory::getLaserHWPFactory()
{
	if (!laserHWPFactory.hasBeenSetup)
	{
		bool setup = laserHWPFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getLaserHWPFactory Complete");
			return laserHWPFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup LaserHWPFactory");
		}
	}
	return laserHWPFactory;
}


RFHeartbeatFactory& HardwareFactory::getRFHeartbeatFactory()
{
	if (!rfHeartbeatFactory.hasBeenSetup)
	{
		bool setup = rfHeartbeatFactory.setup("nominal");
		if (setup)
		{
			messenger.printMessage("getRFHeartbeatFactory Complete");
			return rfHeartbeatFactory;
		}
		else
		{
			messenger.printMessage("Unable to setup rfHeartbeatFactory");
		}
	}
	return rfHeartbeatFactory;
}


bool HardwareFactory::saveMachineSnapshot()// TODO check names
{
	// TOD this couldn't this just call bool HardwareFactory::saveMachineSnapshot(const std::string& location) ???
	boost::filesystem::path now(GlobalFunctions::getTimeAndDateString());
	boost::filesystem::path snapshotLocation;
	boost::system::error_code returnedError;
	snapshotLocation = boost::filesystem::path(SnapshotFileManager::defaultMachineSnapshotLocation) / now;
	boost::filesystem::create_directory(snapshotLocation, returnedError);
	if (returnedError)
	{
		messenger.printMessage("Could not fine snapshot location: ", snapshotLocation.string());
		return false;
	}
	else
	{
		if (valveFactory.hasBeenSetup)
		{
			valveFactory.exportSnapshotToYAML(snapshotLocation.string(), "Valves.yaml");
			return true;
		}
		else
		{
			bool status = valveFactory.setup("nominal");
			if (status)
			{
				valveFactory.exportSnapshotToYAML(snapshotLocation.string(), "Valves.yaml");
				return true;
			}
			else
			{
				messenger.printMessage("Failed to setup ValveFactory, cannot save snapshot for Valves.");
				return true;
			}
		}
		return false;
	}

	return false;
}

bool HardwareFactory::saveMachineSnapshot(const std::string& location)// TODO check names
{
	boost::filesystem::path now(GlobalFunctions::getTimeAndDateString());
	boost::filesystem::path snapshotLocation;
	boost::system::error_code returnedError;
	if (location.empty())
	{ 
		snapshotLocation = boost::filesystem::path(SnapshotFileManager::defaultMachineSnapshotLocation) / now;
	}
	else 
	{
		snapshotLocation = boost::filesystem::path(location) / now;
	}
	boost::filesystem::create_directory(snapshotLocation, returnedError);
	if (returnedError)
	{
		messenger.printMessage("Could not fine snapshot location: ", snapshotLocation.string());
		return false;
	}
	else
	{
		if (valveFactory.hasBeenSetup)
		{
			valveFactory.exportSnapshotToYAML(snapshotLocation.string(), "Valves.yaml");
			return true;
		}
		return false;
	}

	return false;
}

bool HardwareFactory::loadMachineSnapshot(const std::string& directory_location)// TODO check names
{
	if (directory_location.empty())
	{
		messenger.printMessage("Please provide a snapshot folder for loading.");
		return false;
	}
	std::vector<std::string> fileList = SnapshotFileManager::getAllFilesInDirectory(directory_location);
	if (fileList.empty())
	{
		messenger.printMessage("Could not find machine snapshot files at: ", directory_location, ". Please provide another directory.");
		return false;
	}
	for (auto file : fileList)
	{
		messenger.printMessage(file);
		applySnapshot(file);
	}
	return true;
}

bool HardwareFactory::applySnapshot(const std::string& filename)// TODO check names
{
	std::ifstream inFile(filename);
	if (!inFile) { return false; }
	messenger.printMessage("FULL PATH: ", filename);
	YAML::Node snapshotInfo = YAML::LoadFile(filename);
	for (auto&& item : snapshotInfo)
	{
		TYPE hardwareType = GlobalConstants::stringToTypeMap.at(item.first.as<std::string>());
		switch (hardwareType)
		{
			case(TYPE::VALVE):
			{ valveFactory.loadSnapshot(item.second); break; }
		}
	}
	return false;
}

bool HardwareFactory::applySnapshot(const std::map<std::string, std::string> settings)// TODO check names
{
	return false;
}

std::string HardwareFactory::getDefaultSnapshotLocation() const
{
	return SnapshotFileManager::defaultMachineSnapshotLocation;
}


void HardwareFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("HARDWARE-FAC - ", "DEBUG ON");
	bpmFactory.debugMessagesOn();
	cameraFactory.debugMessagesOn();
	chargeFactory.debugMessagesOn();
	imgFactory.debugMessagesOn();
	laserEnergyMeterFactory.debugMessagesOn();
	laserHWPFactory.debugMessagesOn();
	llrffactory.debugMessagesOn();
	magnetFactory.debugMessagesOn();
	rfHeartbeatFactory.debugMessagesOn();
	rfmodulatorFactory.debugMessagesOn();
	rfProtectionFactory.debugMessagesOn();
	screenFactory.debugMessagesOn();
	shutterFactory.debugMessagesOn();
	stageFactory.debugMessagesOn();
	valveFactory.debugMessagesOn();
}

void HardwareFactory::debugMessagesOff()
{
	messenger.printDebugMessage("HARDWARE-FAC - ", "DEBUG OFF");
	messenger.debugMessagesOff();
	bpmFactory.debugMessagesOff();
	cameraFactory.debugMessagesOff();
	chargeFactory.debugMessagesOff();
	imgFactory.debugMessagesOff();
	laserEnergyMeterFactory.debugMessagesOff();
	laserHWPFactory.debugMessagesOff();
	llrffactory.debugMessagesOff();
	magnetFactory.debugMessagesOff();
	rfHeartbeatFactory.debugMessagesOff();
	rfmodulatorFactory.debugMessagesOff();
	rfProtectionFactory.debugMessagesOff();
	screenFactory.debugMessagesOff();
	shutterFactory.debugMessagesOff();
	stageFactory.debugMessagesOff();
	valveFactory.debugMessagesOff();
}

void HardwareFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("HARDWARE-FAC - MESSAGES ON");
	bpmFactory.messagesOn();
	cameraFactory.messagesOn();
	chargeFactory.messagesOn();
	imgFactory.messagesOn();
	laserEnergyMeterFactory.messagesOn();
	laserHWPFactory.messagesOn();
	llrffactory.messagesOn();
	magnetFactory.messagesOn();
	rfHeartbeatFactory.messagesOn();
	rfmodulatorFactory.messagesOn();
	rfProtectionFactory.messagesOn();
	screenFactory.messagesOn();
	shutterFactory.messagesOn();
	stageFactory.messagesOn();
	valveFactory.messagesOn();
}

void HardwareFactory::messagesOff()
{
	messenger.printMessage("HARDWARE-FAC - MESSAGES OFF");
	messenger.messagesOff();

	bpmFactory.messagesOff();
	cameraFactory.messagesOff();
	chargeFactory.messagesOff();
	imgFactory.messagesOff();
	laserEnergyMeterFactory.messagesOff();
	laserHWPFactory.messagesOff();
	llrffactory.messagesOff();
	magnetFactory.messagesOff();
	rfHeartbeatFactory.messagesOff();
	rfmodulatorFactory.messagesOff();
	rfProtectionFactory.messagesOff();
	screenFactory.messagesOff();
	shutterFactory.messagesOff();
	stageFactory.messagesOff();
	valveFactory.messagesOff();
}


bool HardwareFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}

bool HardwareFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool HardwareFactory::isSilent()
{
	return messenger.silent;
}

bool HardwareFactory::makeSilent()
{
	messenger.silent = true;
	return messenger.silent;
}

bool HardwareFactory::makeVerbose()
{
	messenger.silent = false;
	return messenger.silent;
}



bool HardwareFactory::operator==(const HardwareFactory& HardwareFactory) const
{
	/*return(HardwareFactory::HardwareFactory_name.compare(HardwareFactory.HardwareFactory_name)
		&& HardwareFactory::HardwareFactory_type.compare(HardwareFactory.HardwareFactory_type));*/
	return true;
}
