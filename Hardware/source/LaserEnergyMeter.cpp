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
name(paramsMap.at("name")),
energyStats(RunningStats()),
calibration_factor(std::stod(paramsMap.at("calibration_factor"))),
position(std::stod(paramsMap.at("position"))),
start(std::make_pair(epicsTimeStamp(), GlobalConstants::int_min)),
stop(std::make_pair(epicsTimeStamp(), GlobalConstants::int_min)),
overrange(std::make_pair(epicsTimeStamp(), GlobalConstants::int_min)),
range(std::make_pair(epicsTimeStamp(), GlobalConstants::int_min)),
energy(std::make_pair(epicsTimeStamp(), GlobalConstants::int_min)),
bufferSize(10),
vectorSize(10),
shotsvector(0),
shotsbuffer(0),
acquiring(false),
monitoring(false),
epicsInterface(boost::make_shared<EPICSLaserEnergyMeterInterface>(EPICSLaserEnergyMeterInterface())),
laserEnergyMeterType(TYPE::UNKNOWN_TYPE),
status(STATE::UNKNOWN_STATE),
statusBuffer(boost::circular_buffer< STATE >()),
statusVector(std::vector<STATE >()),
aliases(std::vector<std::string >())
{
	messenger.printDebugMessage("constructor");
	epicsInterface->ownerName = hardwareName;
	setPVStructs();
}
LaserEnergyMeter::LaserEnergyMeter(const LaserEnergyMeter& copyLaser) :
Hardware(copyLaser),
laserEnergyMeterType(copyLaser.laserEnergyMeterType),
name(copyLaser.name),
position(copyLaser.position),
epicsInterface(copyLaser.epicsInterface),
energybuffer(copyLaser.energybuffer),
energyvector(copyLaser.energyvector),
energyStats(copyLaser.energyStats),
bufferSize(copyLaser.bufferSize),
vectorSize(copyLaser.vectorSize),
shotsvector(copyLaser.shotsvector),
shotsbuffer(copyLaser.shotsbuffer),
acquiring(copyLaser.acquiring),
monitoring(copyLaser.monitoring),
start(copyLaser.start),
stop(copyLaser.stop),
overrange(copyLaser.overrange),
range(copyLaser.range),
energy(copyLaser.energy),
statusBuffer(copyLaser.statusBuffer),
statusVector(copyLaser.statusVector),
aliases(copyLaser.aliases)
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

void LaserEnergyMeter::attachToInitialContext()
{
	epicsInterface->attachTo_thisCaContext();
}

void LaserEnergyMeter::detachFromInitialContext()
{
	epicsInterface->detachFrom_thisCaContext();
}

std::string LaserEnergyMeter::getLaserEnergyMeterName() const
{
	return this->name;
}

double LaserEnergyMeter::getCalibrationFactor() const
{
	return this->calibration_factor;
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

STATE LaserEnergyMeter::getStatus() const
{
	return status;
}

std::vector< STATE > LaserEnergyMeter::getStatusVector() const
{
	return statusVector;
}

boost::python::dict LaserEnergyMeter::getRunningStats_Py()
{
	boost::python::dict r;
	r["energy_rs"] = energyStats.getRunningStats();
	return r;
}

RunningStats& LaserEnergyMeter::getEnergyRunningStats()
{
	return energyStats;
}

boost::circular_buffer< STATE > LaserEnergyMeter::getStatusBuffer() const
{
	return statusBuffer;
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

bool LaserEnergyMeter::checkBuffer(boost::circular_buffer< double >& buf)
{
	if (buf[buf.size() - 1] == buf[buf.size()])
	{
		return true;
	}
	return false;
}

void LaserEnergyMeter::setRunningStatsSize(size_t new_size)
{
	//std::cout << "LaserEnergyMeter energyStats.setMaxCount " << new_size << std::endl;
	energyStats.setMaxCount(new_size);
}
void LaserEnergyMeter::clearRunningStats()
{
	//std::cout << "LaserEnergyMeter energyStats.Clear " << std::endl;
	energyStats.Clear();
}
bool LaserEnergyMeter::areRunningStatsFull()
{
	return energyStats.Full();
}
size_t LaserEnergyMeter::getRunningStatNumDataValues()const
{
	return energyStats.NumDataValues();
}

void LaserEnergyMeter::checkStatus()
{
	/*if (awak.first - rdy.first > 1.0)
	{
		status = STATE::BAD;
		statusBuffer.push_back(status);
	}*/
	if (shotsbuffer == 0)
	{
		status = STATE::BAD;
		statusBuffer.push_back(status);
	}
	else if (checkBuffer(energybuffer))
	{
		status = STATE::BAD;
		statusBuffer.push_back(status);
	}
	else if (shotsbuffer > 0)
		if (isnan(energybuffer.back()))
		{
			status = STATE::BAD;
			statusBuffer.push_back(status);
		}
		else if (energybuffer.back() != energybuffer[bufferSize - 2])
		{
			status = STATE::GOOD;
			statusBuffer.push_back(status);
		}
		else
		{
			status = STATE::UNKNOWN;
			statusBuffer.push_back(status);
		}
	if (monitoring)
	{
		statusVector.push_back(status);
	}
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