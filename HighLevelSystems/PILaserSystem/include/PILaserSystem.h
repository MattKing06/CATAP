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
	/***OBJECT NAMES FOR FACTORY CALLS**/
	std::string wallCurrentMonitorName = "CLA-S01-DIA-WCM-01";
	std::string halfWavePlateName = "EBT-LAS-OPT-HWP-2";
	std::string energyMeterName = "CLA-LAS-DIA-EM-06";
	std::string mirrorName = "CLA-LAS-OPT-PICO-4C-PM-4";
	std::vector<std::string> shutterNames = { "EBT-INJ-LSR-SHUT-01","EBT-INJ-LSR-SHUT-02" };
	std::string virtualCathodeCameraName = ENUM_TO_STRING(TYPE::CLARA_LASER);
	/***Laser Half Wave Plate Functions***/
	LaserHWP& getLaserHWP();
	double getHWPRead();
	double getHWPSet();
	STATE setLaserHWPPosition(double position);
	/***Laser Mirror Functions***/
	void setHStep(const double& value);
	void setVStep(const double& value);
	STATE moveLeft(const double& value);
	STATE moveRight(const double& value);
	STATE moveUp(const double& value);
	STATE moveDown(const double& value);
	double getCurrentHorizontalPosition();
	double getCurrentVerticalPosition();
	double getMaximumStepSize();
	double getLeftSense();
	double getRightSense();
	double getUpSense();
	double getDownSense();
	/***Virtual Cathode Camera Functions***/
	std::vector<std::string> getCameraName();
	boost::python::list getCameraName_Py();
	/****WCM Functions***/
	double getQ();
	/*** Laser Energy Meter Functions***/
	double getEnergy();
	std::string getEnergyRange();
	STATE setEnergyRange20nJ();
	STATE setEnergyRange20uJ();
	STATE setEnergyRange2uJ();
	STATE setEnergyRange200nJ();
	/***Shutter Functions***/
	bool openLaserShutter(const std::string& shutterName);
	bool closeLaserShutter(const std::string& shutterName);



};