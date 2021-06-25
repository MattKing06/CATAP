#include <PILaserSystem.h>

PILaserSystem::PILaserSystem(STATE mode) :
	cameras(CameraFactory(mode)),
	mirrors(LaserMirrorFactory(mode)),
	shutters(ShutterFactory(mode)),
	halfWavePlate(LaserHWPFactory(mode)),
	charge(ChargeFactory(mode)),
	laserEnergyMeter(LaserEnergyMeterFactory(mode)),
	messenger(LoggingSystem(true, true))
{

}


bool PILaserSystem::setup(const std::string& version)
{
	cameras.setup(version, TYPE::CLARA_LASER);
	mirrors.setup(version);
	shutters.setup(version);
	halfWavePlate.setup(version);
	charge.setup(version);
	laserEnergyMeter.setup(version);
	if (cameras.hasBeenSetup && mirrors.hasBeenSetup
		&& shutters.hasBeenSetup && halfWavePlate.hasBeenSetup
		&& charge.hasBeenSetup && laserEnergyMeter.hasBeenSetup)
	{
		return true;
	}
	else
	{
		messenger.printMessage("COULD NOT SETUP PILaserSystem.");
		return false;
	}
}


PILaserSystem::~PILaserSystem() {}

std::vector<std::string> PILaserSystem::getCameraName()
{
	return cameras.getCameraNames();
}

boost::python::list PILaserSystem::getCameraName_Py()
{
	return to_py_list(getCameraName());
}

double PILaserSystem::getQ()
{
	return charge.getQ(wallCurrentMonitorName);
}

double PILaserSystem::getEnergyMeterReadback()
{
	return laserEnergyMeter.getEnergy(energyMeterName);
}

bool PILaserSystem::openLaserShutter(const std::string& shutterName)
{
	if (shutters.isEnergyInterlockGood(shutterName) &&
		shutters.isChargeInterlockGood(shutterName) &&
		shutters.isPSInterlockGood(shutterName))
	{
		return shutters.open(shutterName);
	}
	else
	{
		messenger.printMessage("LASER SHUTTER INTERLOCKS BAD: ");
		auto shutterCMIBitmap = shutters.getCMIBitMap(shutterName);
		for (auto& bitmap : shutterCMIBitmap)
		{
			messenger.printMessage(bitmap.first, ": ", STATE(bitmap.second));
		}
		return false;
	}
	
}

bool PILaserSystem::closeLaserShutter(const std::string& shutterName)
{
	return false;
}

