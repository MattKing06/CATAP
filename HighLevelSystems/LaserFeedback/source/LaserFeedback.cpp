#include <LaserFeedback.h>

LaserFeedback::LaserFeedback(STATE mode) :
	cameras(CameraFactory(mode)),
	mirrors(LaserMirrorFactory(mode)),
	shutters(ShutterFactory(mode)),
	magnets(MagnetFactory(mode)),
	bpms(BPMFactory(mode)),
	messenger(LoggingSystem(true, true))
{

}


bool LaserFeedback::setup(const std::string& version)
{
	cameras.setup(version, TYPE::CLARA_LASER);
	mirrors.setup(version);
	shutters.setup(version);
	magnets.setup(version);
	bpms.setup(version);
	if (cameras.hasBeenSetup && mirrors.hasBeenSetup && magnets.hasBeenSetup && bpms.hasBeenSetup)
	{
		return true;
	}
	else
	{
		messenger.printMessage("COULD NOT SETUP LaserFeedback.");
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