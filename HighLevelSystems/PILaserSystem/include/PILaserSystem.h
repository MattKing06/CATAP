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
	LaserMirrorFactory mirrorFactory;
	LaserEnergyMeterFactory laserEnergyMeterFactory;
	LaserHWPFactory halfWavePlateFactory;
	ShutterFactory shutterFactory;
	CameraFactory cameraFactory;
	ChargeFactory chargeFactory;
	LoggingSystem messenger;
	LaserMirror mirror;
	LaserEnergyMeter energyMeter;
	LaserHWP halfwavePlate;
	Shutter laserShutter01;
	Shutter laserShutter02;
	Camera virtualCathodeCamera;
	Charge wallCurrentMonitor;
	/***OBJECT NAMES FOR FACTORY CALLS**/
	std::string wallCurrentMonitorName = "CLA-S01-DIA-WCM-01";
	std::string halfWavePlateName = "EBT-LAS-OPT-HWP-2";
	std::string energyMeterName = "CLA-LAS-DIA-EM-06";
	std::string mirrorName = "CLA-LAS-OPT-PICO-4C-PM-4";
	std::vector<std::string> shutterNames = { "EBT-INJ-LSR-SHUT-01","EBT-INJ-LSR-SHUT-02" };
	std::string virtualCathodeCameraName = ENUM_TO_STRING(TYPE::CLARA_LASER);




};