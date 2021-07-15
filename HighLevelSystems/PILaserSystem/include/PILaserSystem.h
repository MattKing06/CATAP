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
	CameraFactory cameraFactory;
	ChargeFactory chargeFactory;
	LaserMirrorFactory mirrorFactory;
	LaserEnergyMeterFactory laserEnergyMeterFactory;
	LaserHWPFactory halfWavePlateFactory;
	ShutterFactory shutterFactory;
	LoggingSystem messenger;
	Camera virtualCathodeCamera;
	Charge wallCurrentMonitor;
	LaserMirror mirror;
	LaserEnergyMeter energyMeter;
	LaserHWP halfwavePlate;
	Shutter laserShutter01;
	Shutter laserShutter02;

	/***OBJECT NAMES FOR FACTORY CALLS**/
	std::string wallCurrentMonitorName = "CLA-S01-DIA-WCM-01";
	std::string halfWavePlateName = "EBT-LAS-OPT-HWP-2";
	std::string energyMeterName = "CLA-LAS-DIA-EM-06";
	std::string mirrorName = "CLA-LAS-OPT-PICO-4C-PM-4";
	std::vector<std::string> shutterNames = { "EBT-INJ-LSR-SHUT-01","EBT-INJ-LSR-SHUT-02" };
	std::string virtualCathodeCameraName = "CLA-VCA-DIA-CAM-01";

	/***COMBINATION FUNCTIONS***/
	Camera& getVirtualCathodeCamera();
	Charge& getWCM();
	LaserMirror& getLaserMirror();
	LaserEnergyMeter& getEnergyMeter();
	LaserHWP& getHWP();

	// TODO these could be bad names, there are many shutters from the PIL to BA1, etc. 
	Shutter& getShutter01();
	Shutter& getShutter02();

	bool canMove(); 
	bool laserEnergyAutoRange();
	bool openShutters();



};