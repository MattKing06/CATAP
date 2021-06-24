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
#include <PythonTypeConversions.h>

class LaserFeedback
{
public:
	LaserFeedback();
	LaserFeedback(STATE mode);
	LaserFeedback(const LaserFeedback& copyLaserFeedback);
	bool setup(const std::string& version);
	~LaserFeedback();
	std::vector<std::string> getCameraName();
	boost::python::list getCameraName_Py();
	LaserMirrorFactory mirrors;
	ShutterFactory shutters;
	CameraFactory cameras;
	MagnetFactory magnets;
	BPMFactory bpms;
	LoggingSystem messenger;
};