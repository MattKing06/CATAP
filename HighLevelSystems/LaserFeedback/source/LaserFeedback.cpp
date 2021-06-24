#include <LaserFeedback.h>

LaserFeedback::LaserFeedback(STATE mode) :
	cameras(CameraFactory(mode)),
	mirrors(LaserMirrorFactory(mode)),
	shutters(ShutterFactory(mode)),
	halfWavePlate(LaserHWPFactory(mode)),
	charge(ChargeFactory(mode)),
	laserEnergyMeter(LaserEnergyMeterFactory(mode)),
	messenger(LoggingSystem(true, true))
{

}


bool LaserFeedback::setup(const std::string& version)
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
		messenger.printMessage("COULD NOT SETUP LaserFeedback.");
		return false;
	}
}


LaserFeedback::~LaserFeedback() {}

std::vector<std::string> LaserFeedback::getCameraName()
{
	return cameras.getCameraNames();
}

boost::python::list LaserFeedback::getCameraName_Py()
{
	return to_py_list(getCameraName());
}