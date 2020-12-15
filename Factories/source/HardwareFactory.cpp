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
	llrffactory(LLRFFactory(mode)),
	cameraFactory(CameraFactory(mode)),
	laserEnergyMeterFactory(LaserEnergyMeterFactory(mode)),
	laserHWPFactory(LaserHWPFactory(mode)),
	shutterFactory(ShutterFactory(mode)),
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
	return getLLRFFactory(machineAreas);
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
			messenger.printMessage("getLLRFFactory Complete");
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
	if (!cameraFactory.hasBeenSetup)
	{
		bool setup = cameraFactory.setup("nominal");
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
