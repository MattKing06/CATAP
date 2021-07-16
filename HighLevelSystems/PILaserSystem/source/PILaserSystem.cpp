#include <PILaserSystem.h>

PILaserSystem::PILaserSystem(STATE mode) :
	cameraFactory(CameraFactory(mode)),
	mirrorFactory(LaserMirrorFactory(mode)),
	shutterFactory(ShutterFactory(mode)),
	halfWavePlateFactory(LaserHWPFactory(mode)),
	chargeFactory(ChargeFactory(mode)),
	laserEnergyMeterFactory(LaserEnergyMeterFactory(mode)),
	messenger(LoggingSystem(true, true))
{

}


bool PILaserSystem::setup(const std::string& version)
{
	cameraFactory.setup(version, TYPE::CLARA_LASER);
	mirrorFactory.setup(version);
	shutterFactory.setup(version);
	halfWavePlateFactory.setup(version);
	chargeFactory.setup(version);
	laserEnergyMeterFactory.setup(version);
	if (cameraFactory.hasBeenSetup && mirrorFactory.hasBeenSetup
		&& shutterFactory.hasBeenSetup && halfWavePlateFactory.hasBeenSetup
		&& chargeFactory.hasBeenSetup && laserEnergyMeterFactory.hasBeenSetup)
	{
		mirror = mirrorFactory.getLaserMirror(mirrorName);
		energyMeter = laserEnergyMeterFactory.getLaserEnergyMeter(energyMeterName);
		halfwavePlate = halfWavePlateFactory.getLaserHWP(halfWavePlateName);
		laserShutter01 = shutterFactory.getShutter(shutterNames.at(0));
		laserShutter02 = shutterFactory.getShutter(shutterNames.at(1));
		virtualCathodeCamera = cameraFactory.getCamera(virtualCathodeCameraName);
		wallCurrentMonitor = chargeFactory.getChargeDiagnostic(wallCurrentMonitorName);
		return true;
	}
	else
	{
		messenger.printMessage("COULD NOT SETUP PILaserSystem.");
		return false;
	}
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
void PILaserSystem::clearAllRunningStats()
{
	wallCurrentMonitor.Clear();
	energyMeter.Clear();
	virtualCathodeCamera.clearAllRunningStats();
}
void PILaserSystem::setAllRunningStatSizes(size_t new_val)
{
	PILaserSystem_RS_size = new_val;
	virtualCathodeCamera.setAllRunningStatSizes(PILaserSystem_RS_size);
	energyMeter.Clear();
	virtualCathodeCamera.Clear();
}




bool PILaserSystem::canMove()
{
	if (laserShutter01.isOpen() && laserShutter02.isOpen())
	{
		if (virtualCathodeCamera.isAnalysisUpdating())
		{
			if (virtualCathodeCamera.hasBeam())
			{
				return true;
			}
		}
	}
	return false;
}

bool PILaserSystem::openShutters()
{
	if (laserShutter01.isClosed())
	{
		laserShutter01.open();
	}
	if (laserShutter02.isClosed())
	{
		laserShutter02.open();
	}
	GlobalFunctions::pause_1000();
	if (laserShutter01.isOpen() && laserShutter02.isOpen())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PILaserSystem::laserEnergyAutoRange()
{
	openShutters();

	if (energyMeter.getStatus() == STATE::GOOD)
	{
		return true;
	}
	else
	{
		energyMeter.setRange(LaserEnergyMeterRecords::laserEnergyMeterRangeSettings[0]);
		bool shutterStatus = openShutters();
		for (auto&& it1 : LaserEnergyMeterRecords::laserEnergyMeterRangeSettings)
		{
			if (energyMeter.getStatus() == STATE::GOOD)
			{
				return true;
			}
			else
			{
				if (energyMeter.getOverRange() == 1)
				{
					energyMeter.setRange(it1 - 1);
					bool shutterStatus = openShutters();
				}
			}
		}
	}
	return false;
}
