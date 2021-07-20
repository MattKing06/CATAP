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
	downSense(std::stod(paramMap.find("down_sense")->second)),
	hStep(GlobalConstants::ten_double),
	vStep(GlobalConstants::ten_double)
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
	downSense(copyLaserMirror.downSense),
	hStep(copyLaserMirror.hStep),
	vStep(copyLaserMirror.vStep)
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

void LaserMirror::setHStep(const double& value)
{
	if (std::abs(value) <= maximumStepSize)
	{
		hStep = value;
	}
	else
	{
		if (value < GlobalConstants::zero_double)
		{
			hStep = -maximumStepSize;
		}
		else
		{
			hStep = maximumStepSize;
		}
	}
}

void LaserMirror::setVStep(const double& value)
{
	if (std::abs(value) <= maximumStepSize)
	{
		vStep = value;
	}
	else
	{
		if (value < GlobalConstants::zero_double)
		{
			vStep = -maximumStepSize;
		}
		else
		{
			vStep = maximumStepSize;
		}
	}
}

double LaserMirror::getHStep()const
{
	return hStep;
}

double LaserMirror::getVStep()const
{
	return vStep;
}

bool LaserMirror::moveHorizontally()
{
	messenger.printMessage(hardwareName, " moveHorizontally ", hStep);
	return epicsInterface->setNewHorizontalPosition(hStep, pvStructs.at(LaserMirrorRecords::H_MREL));
	//return epicsInterface->putValue2<double>(pvStructs.at(LaserMirrorRecords::H_MREL), hStep);
}

bool LaserMirror::moveVertically()
{
	// 
	messenger.printMessage(hardwareName, " moveVertically ", vStep);
	return epicsInterface->setNewVerticalPosition(vStep, pvStructs.at(LaserMirrorRecords::V_MREL));
	//return epicsInterface->putValue2<double>(pvStructs.at(LaserMirrorRecords::V_MREL), hStep);
}




bool LaserMirror::moveLeft(const double& value)
{
	setHStep(value * leftSense);
	messenger.printMessage(hardwareName, " moveLeft, ", hStep);
	return moveHorizontally();
}

bool LaserMirror::moveRight(const double& value)
{
	setHStep(value * rightSense);
	messenger.printMessage(hardwareName, " moveRight, ", hStep);
	return moveHorizontally();
}

bool LaserMirror::moveUp(const double& value)
{
	setVStep(value * upSense);
	messenger.printMessage(hardwareName, " moveUp, ", vStep);
	return moveVertically();
}

bool LaserMirror::moveDown(const double& value)
{
	setVStep(value * downSense);
	messenger.printMessage(hardwareName, " moveDown, ", vStep);
	return moveVertically();
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
