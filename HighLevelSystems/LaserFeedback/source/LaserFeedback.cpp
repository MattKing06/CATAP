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
	cameras.setup(version);
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