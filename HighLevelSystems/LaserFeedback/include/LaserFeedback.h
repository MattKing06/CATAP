#include <MagnetFactory.h>
#include <BPMFactory.h>
#include <CameraFactory.h>
#include <LaserMirrorFactory.h>
#include <ShutterFactory.h>
#include <LoggingSystem.h>
#include <GlobalTypeEnums.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <GlobalFunctions.h>

class LaserFeedback
{
public:
	LaserFeedback();
	LaserFeedback(STATE mode);
	LaserFeedback(const LaserFeedback& copyLaserFeedback);
	bool setup(const std::string& version);
	~LaserFeedback();
	LaserMirrorFactory mirrors;
	ShutterFactory shutters;
	CameraFactory cameras;
	MagnetFactory magnets;
	BPMFactory bpms;
	LoggingSystem messenger;
};