#include <LaserEnergyMeterFactory.h>
#include <ChargeFactory.h>
#include <CameraFactory.h>
#include <LaserMirrorFactory.h>
#include <ShutterFactory.h>
#include <LaserHWPFactory.h>
#include <LoggingSystem.h>
#include <GlobalTypeEnums.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>

class PILaserSystem
{
public:
	PILaserSystem();
	PILaserSystem(STATE mode);
	PILaserSystem(const PILaserSystem& copyPILaserSystem);
	bool setup(const std::string& version);
	~PILaserSystem();
	LaserMirrorFactory mirrors;
	LaserEnergyMeterFactory laserEnergyMeter;
	LaserHWPFactory halfWavePlate;
	ShutterFactory shutters;
	CameraFactory cameras;
	ChargeFactory charge;
	LoggingSystem messenger;
	std::string wallCurrentMonitorName = "CLA-S01-DIA-WCM-01";
	std::string energyMeterName = "CLA-LAS-DIA-EM-06";
	std::vector<std::string> shutterNames = { "EBT-INJ-LSR-SHUT-01","EBT-INJ-LSR-SHUT-02" };
	std::string virtualCathodeCameraName = ENUM_TO_STRING(TYPE::CLARA_LASER);
	std::vector<std::string> getCameraName();
	boost::python::list getCameraName_Py();
	double getQ();
	double getEnergyMeterReadback();
	bool openLaserShutter(const std::string& shutterName);
	bool closeLaserShutter(const std::string& shutterName);

};