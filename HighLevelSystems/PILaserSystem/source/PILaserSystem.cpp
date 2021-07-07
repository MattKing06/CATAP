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

LaserHWP& PILaserSystem::getLaserHWP()
{
	return halfWavePlate.getLaserHWP(halfWavePlateName);
}

double PILaserSystem::getHWPRead()
{
	return halfWavePlate.getHWPRead(halfWavePlateName);
}

double PILaserSystem::getHWPSet()
{
	return halfWavePlate.getHWPSet(halfWavePlateName);
}

void PILaserSystem::setHStep(const double& value)
{
	mirrors.setHStep(mirrorName, value);
}

void PILaserSystem::setVStep(const double& value)
{
	mirrors.setVStep(mirrorName, value);
}

STATE PILaserSystem::moveLeft(const double& value)
{
	bool status = mirrors.moveLeft(mirrorName, value);
	if(status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
}

STATE PILaserSystem::moveRight(const double& value)
{
	bool status = mirrors.moveRight(mirrorName, value);
	if (status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
}

STATE PILaserSystem::moveUp(const double& value)
{
	bool status = mirrors.moveUp(mirrorName, value);
	if (status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
}

STATE PILaserSystem::moveDown(const double& value)
{
	bool status = mirrors.moveDown(mirrorName, value);
	if (status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
}

double PILaserSystem::getCurrentHorizontalPosition()
{
	return mirrors.getCurrentHorizontalPosition(mirrorName);
}

double PILaserSystem::getCurrentVerticalPosition()
{
	return mirrors.getCurrentVerticalPosition(mirrorName);
}

double PILaserSystem::getMaximumStepSize()
{
	return mirrors.getMaximumStepSize(mirrorName);
}

double PILaserSystem::getLeftSense()
{
	return mirrors.getLeftSense(mirrorName);
}

double PILaserSystem::getRightSense()
{
	return mirrors.getRightSense(mirrorName);
}

double PILaserSystem::getUpSense()
{
	return mirrors.getUpSense(mirrorName);
}

double PILaserSystem::getDownSense()
{
	return mirrors.getDownSense(mirrorName);
}

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

double PILaserSystem::getEnergy()
{
	return laserEnergyMeter.getEnergy(energyMeterName);
}

/** THE ENERGY RANGES HERE NEED TO BE MADE INTO ENUMS..**/

std::string PILaserSystem::getEnergyRange()
{
	/** THE ENERGY RANGES HERE NEED TO BE MADE INTO ENUMS..**/
	int rangeEnum = laserEnergyMeter.getRange(energyMeterName);
	switch (rangeEnum)
	{
	case(0):
		return "20nJ";
		break;
	case(1):
		return "200nJ";
		break;
	case(2):
		return "2uJ";
		break;
	case(3):
		return "20uJ";
		break;
	}
}

STATE PILaserSystem::setEnergyRange20uJ()
{
	/** THE ENERGY RANGES HERE NEED TO BE MADE INTO ENUMS..**/
	bool status = laserEnergyMeter.setRange(energyMeterName, 3);
	if (status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
}

STATE PILaserSystem::setEnergyRange2uJ()
{
	/** THE ENERGY RANGES HERE NEED TO BE MADE INTO ENUMS..**/
	bool status = laserEnergyMeter.setRange(energyMeterName, 2);
	if (status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
}

STATE PILaserSystem::setEnergyRange200nJ()
{
	/** THE ENERGY RANGES HERE NEED TO BE MADE INTO ENUMS..**/
	bool status = laserEnergyMeter.setRange(energyMeterName, 1);
	if (status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
}

STATE PILaserSystem::setEnergyRange20nJ()
{
	/** THE ENERGY RANGES HERE NEED TO BE MADE INTO ENUMS..**/
	bool status = laserEnergyMeter.setRange(energyMeterName, 0);
	if (status) { return STATE::SUCCESS; }
	else { return STATE::FAIL; }
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

