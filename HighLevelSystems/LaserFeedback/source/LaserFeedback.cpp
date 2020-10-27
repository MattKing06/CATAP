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

LaserFeedback::~LaserFeedback() {}