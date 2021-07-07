#include <PILaserSystem.h>

PILaserSystem::PILaserSystem(STATE mode) :
	cameraFactory(CameraFactory(mode)),
	mirrorFactory(LaserMirrorFactory(mode)),
	shutterFactory(ShutterFactory(mode)),
	halfWavePlateFactory(LaserHWPFactory(mode)),
	chargeFactory(ChargeFactory(mode)),
	laserEnergyMeterFactory(LaserEnergyMeterFactory(mode)),
	messenger(LoggingSystem(true, true))
{

}


bool PILaserSystem::setup(const std::string& version)
{
	cameraFactory.setup(version, TYPE::CLARA_LASER);
	mirrorFactory.setup(version);
	shutterFactory.setup(version);
	halfWavePlateFactory.setup(version);
	chargeFactory.setup(version);
	laserEnergyMeterFactory.setup(version);
	if (cameraFactory.hasBeenSetup && mirrorFactory.hasBeenSetup
		&& shutterFactory.hasBeenSetup && halfWavePlateFactory.hasBeenSetup
		&& chargeFactory.hasBeenSetup && laserEnergyMeterFactory.hasBeenSetup)
	{
		mirror = mirrorFactory.getLaserMirror(mirrorName);
		energyMeter = laserEnergyMeterFactory.getLaserEnergyMeter(energyMeterName);
		halfwavePlate = halfWavePlateFactory.getLaserHWP(halfWavePlateName);
		laserShutter01 = shutterFactory.getShutter(shutterNames.at(0));
		laserShutter02 = shutterFactory.getShutter(shutterNames.at(1));
		virtualCathodeCamera = cameraFactory.getCamera(virtualCathodeCameraName);
		wallCurrentMonitor = chargeFactory.getChargeDiagnostic(wallCurrentMonitorName);
		return true;
	}
	else
	{
		messenger.printMessage("COULD NOT SETUP PILaserSystem.");
		return false;
	}
}


PILaserSystem::~PILaserSystem() {}
