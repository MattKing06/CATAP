#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <LaserEnergyMeter.h>
#include <map>
#include <vector>
#include <numeric>
#include <math.h>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>

LaserEnergyMeter::LaserEnergyMeter()
{}
LaserEnergyMeter::LaserEnergyMeter(const std::map<std::string, std::string> & paramsMap, STATE mode) :
Hardware(paramsMap, mode),
//laserEnergyMeterType(LaserEnergyMeterRecords::laserEnergyMeterTypeToEnum.at(paramsMap.find("laser_pv_type")->second)),
name(paramsMap.find("name")->second),
position(std::stod(paramsMap.find("position")->second))
{
messenger.printDebugMessage("constructor");
setPVStructs();
bufferSize = 10;
vectorSize = 10;
shotsvector = 0;
shotsbuffer = 0;
energybuffer.resize(bufferSize);
energyvector.resize(vectorSize);
acquiring = false;
monitoring = false;
epicsInterface = boost::make_shared<EPICSLaserEnergyMeterInterface>(EPICSLaserEnergyMeterInterface());
epicsInterface->ownerName = hardwareName;
}
LaserEnergyMeter::LaserEnergyMeter(const LaserEnergyMeter& copyLaser) :
Hardware(copyLaser),
laserEnergyMeterType(copyLaser.laserEnergyMeterType),
name(copyLaser.name),
position(copyLaser.position),
epicsInterface(copyLaser.epicsInterface)
{
}

void LaserEnergyMeter::setPVStructs()
{
	messenger.printDebugMessage("in setPVstructs");

	for (auto&& record : LaserEnergyMeterRecords::laserEnergyMeterRecordList)
	{
		messenger.printDebugMessage("in loop");
		pvStructs[record] = pvStruct();
		messenger.printDebugMessage(record);
		pvStructs[record].pvRecord = record;

		// TODO NO ERROR CHECKING! (we assum config file is good??? 
		std::string PV = specificHardwareParameters.find(record)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		messenger.printDebugMessage("Constructing PV information for ", record);

		/*TODO
		  This should be put into some general function: generateVirtualPV(PV) or something...
		  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
		  The config reader level if that is the case.
		  DJS maybe they should, how certian cna we be all virtual PVs will get a VM- prefix???
		  */
		if (mode == STATE::VIRTUAL)
		{
			pvStructs[record].fullPVName = "VM-" + PV;
		}
		else
		{
			pvStructs[record].fullPVName = PV;
		}
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}

std::vector<std::string> LaserEnergyMeter::getAliases() const
{
	return this->aliases;
}

TYPE LaserEnergyMeter::getLaserEnergyMeterType() const
{
	return this->laserEnergyMeterType;
}

std::string LaserEnergyMeter::getLaserEnergyMeterName() const
{
	return this->name;
}

bool LaserEnergyMeter::isMonitoring() const
{
	return monitoring;
}

bool LaserEnergyMeter::getAcquiring() const
{
	return this->acquiring;
}

bool LaserEnergyMeter::setStart()
{
	epicsInterface->setSTART(1, pvStructs.at(LaserEnergyMeterRecords::RUNSP));
	acquiring = true;
	return true;
}

bool LaserEnergyMeter::setStop()
{
	epicsInterface->setSTART(0, pvStructs.at(LaserEnergyMeterRecords::RUNSP));
	acquiring = false;
	return true;
}

bool LaserEnergyMeter::setRange(int set)
{
	epicsInterface->setRANGE(set, pvStructs.at(LaserEnergyMeterRecords::RANGESP));
	range.second = set;
	return true;
}

int LaserEnergyMeter::getRange() const
{
	return range.second;
}

int LaserEnergyMeter::getOverRange() const
{
	return overrange.second;
}

double LaserEnergyMeter::getEnergy() const
{
	return energy.second;
}

bool LaserEnergyMeter::setEnergy(const double& value)
{
	energy.second = value;
	energybuffer.push_back(value);
	shotsbuffer++;
	if (monitoring)
	{
		shotsvector++;
		if (shotsvector <= energyvector.size())
		{
			energyvector[shotsvector - 1] = value;
		}
		else
		{
			monitoring = false;
		}
	}
	return true;
}

std::vector< double > LaserEnergyMeter::getEnergyVector() const
{
	return energyvector;
}

boost::circular_buffer< double > LaserEnergyMeter::getEnergyBuffer() const
{
	return energybuffer;
}

bool LaserEnergyMeter::isEnergyVectorFull() const
{
	if (shotsvector >= bufferSize)
	{
		return true;
	}
	return false;
}

bool LaserEnergyMeter::isEnergyBufferFull() const
{
	if (shotsbuffer >= bufferSize)
	{
		return true;
	}
	return false;
}

void LaserEnergyMeter::monitorForNShots(const size_t& value)
{
	setVectorSize(value);
	monitoring = true;
}

void LaserEnergyMeter::setVectorSize(const size_t& value)
{
	clearVectors();
	energyvector.clear();
	vectorSize = value;
	energyvector.resize(vectorSize);
}

void LaserEnergyMeter::setBufferSize(const size_t& value)
{
	clearBuffers();
	energybuffer.clear();
	bufferSize = value;
	energybuffer.resize(bufferSize);
}

void LaserEnergyMeter::clearVectors()
{
	shotsvector = 0;
	energyvector.clear();
}

void LaserEnergyMeter::clearBuffers()
{
	shotsbuffer = 0;
	energybuffer.clear();
}

void LaserEnergyMeter::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LaserEnergyMeter::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG On");
	epicsInterface->debugMessagesOn();
}

void LaserEnergyMeter::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void LaserEnergyMeter::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES On");
	epicsInterface->messagesOn();
}