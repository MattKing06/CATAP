#include "HardwareFactory.h"
#include "PythonTypeConversions.h"
#include "GlobalFunctions.h"

HardwareFactory::HardwareFactory() : HardwareFactory(STATE::OFFLINE)
{
}
HardwareFactory::~HardwareFactory()
{
	messenger.printDebugMessage("HardwareFactory Destruction Called");
}
HardwareFactory::HardwareFactory(STATE mode) :
	messenger(LoggingSystem(true, true)),
	magnetFactory(MagnetFactory(mode)),
	bpmFactory(BPMFactory(mode)),
	chargeFactory(ChargeFactory(mode)),
	screenFactory(ScreenFactory(mode)),
	valveFactory(ValveFactory(mode)),
	imgFactory(IMGFactory(mode)),
	rfProtectionFactory(RFProtectionFactory(mode)),
	llrffactory(LLRFFactory(mode)),
	cameraFactory(CameraFactory(mode)),
	laserEnergyMeterFactory(LaserEnergyMeterFactory(mode)),
	laserHWPFactory(LaserHWPFactory(mode)),
	shutterFactory(ShutterFactory(mode)),
	rfmodulatorFactory(RFModulatorFactory(mode)),
	rfHeartbeatFactory(RFHeartbeatFactory(mode)),
	mode(mode)
{
	//messenger = LoggingSystem(true, true);
	messenger.printDebugMessage("Hardware Factory constructed, mode = ", ENUM_TO_STRING(mode));
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
	}
	else if (hardwareType == "BPM")
	{
		if (!bpmFactory.hasBeenSetup)
		{
			setup = bpmFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Charge")
	{
		if (!chargeFactory.hasBeenSetup)
		{
			setup = chargeFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Screen")
	{
		if (!screenFactory.hasBeenSetup)
		{
			setup = screenFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Valve")
	{
		if (!valveFactory.hasBeenSetup)
		{
			setup = valveFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Laser Energy Meter")
	{
		if (!laserEnergyMeterFactory.hasBeenSetup)
		{
			setup = laserEnergyMeterFactory.setup(VERSION);
		}
	}
	else if (hardwareType == "Laser HWP")
	{
		if (!laserHWPFactory.hasBeenSetup)
		{
			setup = laserHWPFactory.setup(VERSION);
		}
	}
	return setup;
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

bool HardwareFactory::saveMachineSnapshot()
{
	std::string now = GlobalFunctions::getTimeAndDateString();
	const std::string snapshotLocation = "\\\\claraserv3\\claranet\\test\\CATAP\\MachineState\\" + now;
	boost::filesystem::path saveLocation = snapshotLocation;
	boost::filesystem::create_directory(saveLocation);
	if (valveFactory.hasBeenSetup)
	{
		valveFactory.exportSnapshotToYAML(saveLocation.string(), "Valves.yaml");
	}
	return false;
}

std::vector<std::string> getAllFilesInDirectory(const std::string& dirPath, const std::vector<std::string> skipList = {})
{
	std::vector<std::string> fileList;
	const std::vector<std::string> extensions = {"yml", "yaml", "YML", "YAML"};
	try
	{
		if (boost::filesystem::exists(dirPath) && boost::filesystem::is_directory(dirPath))
		{
			boost::filesystem::directory_iterator iter(dirPath);
			boost::filesystem::directory_iterator end;
			while (iter != end)
			{
				if (boost::filesystem::is_directory(iter->path()) &&
					(std::find(skipList.begin(), skipList.end(), iter->path().filename()) != skipList.end()))
				{
					boost::system::error_code ec;
					iter.increment(ec);
					if (ec)
					{
						std::cout << " COULD NOT ACCESS : " << iter->path().string() << std::endl;
					}
				}
				else if (std::find(extensions.begin(), extensions.end(), iter->path().extension().string())
						 != extensions.end())
				{
					fileList.push_back(iter->path().string());
					std::cout << "FOUND: " << iter->path().string() << std::endl;
					boost::system::error_code ec;
					iter.increment(ec);
					if (ec)
					{
						std::cout << " COULD NOT ACCESS : " << iter->path().string() << std::endl;
					}
				}
				else
				{
					boost::system::error_code ec;
					iter.increment(ec);
					std::cout << iter->path().string() << " is in the wrong Format." << std::endl;
				}
				}
			}
		else
		{
			std::cout << " DOES NOT EXIST " << std::endl;
		}
	}
	catch (std::system_error & e)
	{
		std::cout << e.what() << std::endl;
	}
	return fileList;
}


bool HardwareFactory::loadMachineSnapshot(const std::string& location="")
{
	std::vector<std::string> fileList = getAllFilesInDirectory(location);
	for (auto file : fileList)
	{
		messenger.printMessage(file);
	}
	return true;
}


void HardwareFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("HARDWARE-FAC - ", "DEBUG ON");
	magnetFactory.debugMessagesOn();
	bpmFactory.debugMessagesOn();
	chargeFactory.debugMessagesOn();
	cameraFactory.debugMessagesOn();
	laserEnergyMeterFactory.debugMessagesOn();
	laserHWPFactory.debugMessagesOn();
}

void HardwareFactory::debugMessagesOff()
{
	messenger.printDebugMessage("HARDWARE-FAC - ", "DEBUG OFF");
	messenger.debugMessagesOff();
	magnetFactory.debugMessagesOff();
	bpmFactory.debugMessagesOff();
	chargeFactory.debugMessagesOff();
	valveFactory.debugMessagesOff();
	laserEnergyMeterFactory.debugMessagesOff();
	laserHWPFactory.debugMessagesOff();
}

void HardwareFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("HARDWARE-FAC - MESSAGES ON");
	magnetFactory.messagesOn();
	bpmFactory.messagesOn();
	chargeFactory.messagesOn();
	valveFactory.messagesOn();
	laserEnergyMeterFactory.messagesOn();
	laserHWPFactory.messagesOn();
}

void HardwareFactory::messagesOff()
{
	messenger.printMessage("HARDWARE-FAC - MESSAGES OFF");
	messenger.messagesOff();
	magnetFactory.messagesOff();
	bpmFactory.messagesOff();
	chargeFactory.messagesOff();
	valveFactory.messagesOff();
	laserEnergyMeterFactory.messagesOff();
	laserHWPFactory.messagesOff();
}

bool HardwareFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}

bool HardwareFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool HardwareFactory::operator==(const HardwareFactory& HardwareFactory) const
{
	/*return(HardwareFactory::HardwareFactory_name.compare(HardwareFactory.HardwareFactory_name)
		&& HardwareFactory::HardwareFactory_type.compare(HardwareFactory.HardwareFactory_type));*/
	return true;
}
