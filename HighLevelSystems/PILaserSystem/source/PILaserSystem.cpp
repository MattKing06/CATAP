#include <PILaserSystem.h>
#include <iostream>

PILaserSystem::PILaserSystem(STATE mode) :
	cameraFactory(CameraFactory(mode)),
	mirrorFactory(LaserMirrorFactory(mode)),
	shutterFactory(ShutterFactory(mode)),
	halfWavePlateFactory(LaserHWPFactory(mode)),
	chargeFactory(ChargeFactory(mode)),
	laserEnergyMeterFactory(LaserEnergyMeterFactory(mode)),
	PILaserSystem_RS_size(0),
	messenger(LoggingSystem(true, true))
{

}


bool PILaserSystem::setup(const std::string& version)
{
	messenger.printMessage("PILaserSystem calling cameraFactory.setup.");
	cameraFactory.setup(version, TYPE::CLARA_LASER);
	messenger.printMessage("PILaserSystem calling mirrorFactory.setup.");
	mirrorFactory.setup(version);
	messenger.printMessage("PILaserSystem calling shutterFactory.setup.");
	shutterFactory.setup(version);
	messenger.printMessage("PILaserSystem calling halfWavePlateFactory.setup.");
	halfWavePlateFactory.setup(version);
	messenger.printMessage("PILaserSystem calling chargeFactory.setup.");
	chargeFactory.setup(version);
	messenger.printMessage("PILaserSystem calling laserEnergyMeterFactory.setup.");
	laserEnergyMeterFactory.setup(version);
	setAllRunningStatSize(0);
	return true;
}


PILaserSystem::~PILaserSystem() {}

Camera& PILaserSystem::getVirtualCathodeCamera()
{
	// WE HAVE TO GET THIS FROM THE FACTORY //
	// OTHERWISE WE ARE COPYING THE HARDWARE TO A NEW // 
	// SET OF MEMORY ADDRESS THAT ARE NEVER UPDATED //
	// BY EPICS.  //
	// ASK MATT OR DUNCAN WHY THIS WORKS. //
	return cameraFactory.getCamera(virtualCathodeCameraName);
}

Charge& PILaserSystem::getWCM()
{
	// WE HAVE TO GET THIS FROM THE FACTORY //
	// OTHERWISE WE ARE COPYING THE HARDWARE TO A NEW // 
	// SET OF MEMORY ADDRESS THAT ARE NEVER UPDATED //
	// BY EPICS.  //
	// ASK MATT OR DUNCAN WHY THIS WORKS. //
	return chargeFactory.getChargeDiagnostic(wallCurrentMonitorName);
}

LaserMirror& PILaserSystem::getLaserMirror()
{
	return mirrorFactory.getLaserMirror(mirrorName);
}

LaserEnergyMeter& PILaserSystem::getEnergyMeter()
{
	// WE HAVE TO GET THIS FROM THE FACTORY //
	// OTHERWISE WE ARE COPYING THE HARDWARE TO A NEW // 
	// SET OF MEMORY ADDRESS THAT ARE NEVER UPDATED //
	// BY EPICS.  //
	// ASK MATT OR DUNCAN WHY THIS WORKS. //
	return laserEnergyMeterFactory.getLaserEnergyMeter(energyMeterName);
}

LaserHWP& PILaserSystem::getHWP()
{
	// WE HAVE TO GET THIS FROM THE FACTORY //
	// OTHERWISE WE ARE COPYING THE HARDWARE TO A NEW // 
	// SET OF MEMORY ADDRESS THAT ARE NEVER UPDATED //
	// BY EPICS.  //
	// ASK MATT OR DUNCAN WHY THIS WORKS. //
	return halfWavePlateFactory.getLaserHWP(halfWavePlateName);
}

Shutter& PILaserSystem::getShutter01()
{
	// WE HAVE TO GET THIS FROM THE FACTORY //
	// OTHERWISE WE ARE COPYING THE HARDWARE TO A NEW // 
	// SET OF MEMORY ADDRESS THAT ARE NEVER UPDATED //
	// BY EPICS.  //
	// ASK MATT OR DUNCAN WHY THIS WORKS. //
	return shutterFactory.getShutter(shutterNames.at(0));
}

Shutter& PILaserSystem::getShutter02()
{
	// WE HAVE TO GET THIS FROM THE FACTORY //
	// OTHERWISE WE ARE COPYING THE HARDWARE TO A NEW // 
	// SET OF MEMORY ADDRESS THAT ARE NEVER UPDATED //
	// BY EPICS.  //
	// ASK MATT OR DUNCAN WHY THIS WORKS. //
	return shutterFactory.getShutter(shutterNames.at(1));
}

size_t PILaserSystem_RS_size;
/*! Clear all the values assoociated with the Running mean and variance stats.*/
void PILaserSystem::clearAllRunningStat()
{
	std::cout << "PILaserSystem wallCurrentMonitor.clearRunningStats " << std::endl;
	chargeFactory.clearRunningStats(wallCurrentMonitorName);
	//wallCurrentMonitor.clearRunningStats();
	std::cout << "PILaserSystem energyMeter.clearRunningStats " << std::endl;
	laserEnergyMeterFactory.clearRunningStats(energyMeterName);
	//energyMeter.clearRunningStats();
	std::cout << "PILaserSystem virtualCathodeCamera.clearRunningStats " << std::endl;
	cameraFactory.clearRunningStats(virtualCathodeCameraName);
}
void PILaserSystem::setAllRunningStatSize(size_t new_val)
{
	std::cout << "PILaserSystem setAllRunningStatSize passed " << new_val << std::endl;
	PILaserSystem_RS_size = new_val;
	std::cout << "PILaserSystem PILaserSystem_RS_size = " << PILaserSystem_RS_size << std::endl;
	std::cout << "PILaserSystem virtualCathodeCamera.setAllRunningStatSizes " << PILaserSystem_RS_size << std::endl;
	cameraFactory.setRunningStatSize(virtualCathodeCameraName, PILaserSystem_RS_size);
	std::cout << "PILaserSystem wallCurrentMonitor.setAllRunningStatSizes " << PILaserSystem_RS_size << std::endl;
	chargeFactory.setRunningStatSize(wallCurrentMonitorName, PILaserSystem_RS_size);
	std::cout << "PILaserSystem energyMeter.setAllRunningStatSizes " << PILaserSystem_RS_size << std::endl;
	laserEnergyMeterFactory.setRunningStatSize(energyMeterName, PILaserSystem_RS_size);
	clearAllRunningStat();
}
size_t PILaserSystem::getRunningStatSize()
{
	return PILaserSystem_RS_size;
}
//size_t PILaserSystem::getRunningStatCount()
//{
//	return wallCurrentMonitor.getRunningStatCount();
//}
//bool PILaserSystem::isRunningStatFull()
//{
//	return wallCurrentMonitor.isRunningStatFull();
//}
//
//
//bool PILaserSystem::canMove()
//{
//	if (shutterFactory.isOpen(shutterNames.at(0)) && shutterFactory.isOpen(shutterNames.at(1)))
//	{
//		if (cameraFactory.isAnalysisUpdating(virtualCathodeCameraName))
//		{
//			if (cameraFactory.hasBeam(virtualCathodeCameraName))
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}

bool PILaserSystem::openShutters()
{
	if (shutterFactory.isClosed(shutterNames.at(0)))
	{
		shutterFactory.open(shutterNames.at(0));
	}
	if (shutterFactory.isClosed(shutterNames.at(1)))
	{
		shutterFactory.open(shutterNames.at(1));
	}
	GlobalFunctions::pause_1000();
	if (shutterFactory.isOpen(shutterNames.at(0)) && shutterFactory.isOpen(shutterNames.at(1)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//bool PILaserSystem::laserEnergyAutoRange()
//{
//	openShutters();
//
//	if (energyMeter.getStatus() == STATE::GOOD)
//	{
//		return true;
//	}
//	else
//	{
//		energyMeter.setRange(LaserEnergyMeterRecords::laserEnergyMeterRangeSettings[0]);
//		bool shutterStatus = openShutters();
//		for (auto&& it1 : LaserEnergyMeterRecords::laserEnergyMeterRangeSettings)
//		{
//			if (energyMeter.getStatus() == STATE::GOOD)
//			{
//				return true;
//			}
//			else
//			{
//				if (energyMeter.getOverRange() == 1)
//				{
//					energyMeter.setRange(it1 - 1);
//					bool shutterStatus = openShutters();
//				}
//			}
//		}
//	}
//	return false;
//}
