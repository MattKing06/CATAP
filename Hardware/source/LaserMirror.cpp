#include <LaserMirror.h>

LaserMirror::LaserMirror()
{
}

LaserMirror::LaserMirror(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	currentHorizontalPosition(std::pair<epicsTimeStamp, double>(epicsTimeStamp(), GlobalConstants::double_min)),
	currentVerticalPosition(std::pair<epicsTimeStamp, double>(epicsTimeStamp(), GlobalConstants::double_min)),
	maximumStepSize(std::stod(paramMap.find("step_max")->second)),
	leftSense(std::stod(paramMap.find("left_sense")->second)),
	rightSense(std::stod(paramMap.find("right_sense")->second)),
	upSense(std::stod(paramMap.find("up_sense")->second)),
	downSense(std::stod(paramMap.find("down_sense")->second))
{
	setPVStructs();
	epicsInterface = boost::make_shared<EPICSLaserMirrorInterface>(EPICSLaserMirrorInterface());
	epicsInterface->ownerName = hardwareName;
}

LaserMirror::LaserMirror(const LaserMirror& copyLaserMirror) :
	Hardware(copyLaserMirror),
	epicsInterface(copyLaserMirror.epicsInterface),
	currentHorizontalPosition(copyLaserMirror.currentHorizontalPosition),
	currentVerticalPosition(copyLaserMirror.currentVerticalPosition),
	maximumStepSize(copyLaserMirror.maximumStepSize),
	leftSense(copyLaserMirror.leftSense),
	rightSense(copyLaserMirror.rightSense),
	upSense(copyLaserMirror.upSense),
	downSense(copyLaserMirror.downSense)
{
}

LaserMirror::~LaserMirror()
{
}

void LaserMirror::setPVStructs()
{
	for (auto&& record : LaserMirrorRecords::laserMirrorRecordList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		std::string PV = specificHardwareParameters.find(record)->second.data();
		if (mode == STATE::PHYSICAL)
		{
			pvStructs[record].fullPVName = PV;
		}
		else
		{
			
			pvStructs[record].fullPVName = "VM-" + PV;
		}
	}
}

void LaserMirror::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void LaserMirror::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LaserMirror::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGING ON");
	epicsInterface->messagesOn();
}

void LaserMirror::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGING OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();

}

bool LaserMirror::moveHorizontalRelative(const double& delta)
{
	epicsInterface->setNewHorizontalPosition(delta, pvStructs.at(LaserMirrorRecords::H_MREL));
	epicsInterface->moveHorizontal(pvStructs.at(LaserMirrorRecords::POSBTN));
}

bool LaserMirror::moveVeritcalRelative(const double& delta)
{
	epicsInterface->setNewVerticalPosition(delta, pvStructs.at(LaserMirrorRecords::V_MREL));
	epicsInterface->moveVertical(pvStructs.at(LaserMirrorRecords::POSBTN));
}

double LaserMirror::getCurrentHorizontalPosition()
{
	return currentHorizontalPosition.second;
}

double LaserMirror::getCurrentVerticalPosition()
{
	return currentVerticalPosition.second;
}


bool LaserMirror::updatePositions()
{
	return false;
}

double LaserMirror::getMaximumStepSize()
{
	return maximumStepSize;
}

double LaserMirror::getLeftSense()
{
	return leftSense;
}

double LaserMirror::getRightSense()
{
	return rightSense;
}

double LaserMirror::getUpSense()
{
	return upSense;
}

double LaserMirror::getDownSense()
{
	return downSense;
}
