#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include "Charge.h"
#include <map>
#include <vector>
#include <numeric>
#include <math.h>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>

Charge::Charge()
{}

Charge::Charge(const std::map<std::string, std::string>& paramsMap, STATE mode) :
Hardware(paramsMap, mode),
chargeType(paramsMap.at("charge_type")),
name(paramsMap.at("name")),
position(std::stod(paramsMap.at("position"))),
qStats(RunningStats())
{
	messenger.printDebugMessage("constructor");
	setPVStructs();
	bufferSize = 10;
	qshots = 0;
	epicsInterface = boost::make_shared<EPICSChargeInterface>(EPICSChargeInterface());
	qBuffer.resize(bufferSize);
	monitoringq= false;
	epicsInterface->ownerName = hardwareName;
}
Charge::Charge(const Charge& copyCharge) :
Hardware(copyCharge),
chargeType(copyCharge.chargeType),
name(copyCharge.name),
position(copyCharge.position),
epicsInterface(copyCharge.epicsInterface),
qStats(copyCharge.qStats)
{
}

void Charge::attachToInitialContext()
{
	epicsInterface->attachTo_thisCaContext();
}

void Charge::detachFromInitialContext()
{
	epicsInterface->detachFrom_thisCaContext();
}

void Charge::setPVStructs()
{
	messenger.printDebugMessage("in setPVstructs");

	for (auto&& record : ChargeRecords::chargeRecordList)
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

std::vector<std::string> Charge::getAliases() const
{
	return this->aliases;
}

std::string Charge::getChargeDiagnosticType() const
{
	return this->chargeType;
}

std::string Charge::getName() const
{
	return this->name;
}

double Charge::getQ() const
{
	return this->q.second;
}

std::vector< double > Charge::getQVector() const
{
	//if (monitoringxpv)
	//{
	//	LoggingSystem::printDebugMessage("WARNING: STILL monitorING X PV -- VECTOR NOT FULL");
	//}
	return this->qVector;
}

double Charge::getPosition() const
{
	return this->position;
}

bool Charge::isQBufferFull() const
{
	if (qshots >= bufferSize)
	{
		return true;
	}
	return false;
}

size_t Charge::getBufferSize() const
{
	return this->bufferSize;
}

boost::circular_buffer< double > Charge::getQBuffer() const
{
	return this->qBuffer;
}

boost::python::list Charge::getQBuffer_Py() const
{
	boost::circular_buffer< double > qbuf;
	qbuf = getQBuffer();
	boost::python::list newPyList = to_py_list(qbuf);
	return newPyList;
}

boost::python::list Charge::getQVector_Py() const
{
	std::vector< double > qvec;
	qvec = getQVector();
	boost::python::list newPyList = to_py_list(qvec);
	return newPyList;
}

bool Charge::setQVirtual(const double& value)
{
	if (mode == STATE::VIRTUAL)
	{
		epicsInterface->setQ(value, pvStructs.at(ChargeRecords::Q));
		setQ(value);
		return true;
	}
	else
	{
		return false;
	}
}

bool Charge::setQ(const double& value)
{
	q.second = value;
	messenger.printMessage(q.second);
	qBuffer.push_back(value);
	qshots++;
	if (monitoringq)
	{
		if (qshots <= qVector.size())
		{
			qVector[qshots-1] = value;
		}
		else
		{
			monitoringq = false;
		}
	}
	return true;
}

bool Charge::ismonitoringQ() const
{
	return monitoringq;
}

bool Charge::ismonitoring() const
{
	return ismonitoringQ();
}

bool Charge::checkBuffer(boost::circular_buffer< double >& buf)
{
	if (buf[buf.size() - 1] == buf[buf.size()])
	{
		return true;
	}
	return false;
}

void Charge::monitorForNShots(const size_t& value)
{
	setVectorSize(value);
	monitoringq = true;
}

void Charge::setVectorSize(const size_t& value)
{
	clearBuffers();
	qVector.clear();
	vectorSize = value;
	qVector.resize(vectorSize);
}

void Charge::setBufferSize(const size_t& value)
{
	clearBuffers();
	bufferSize = value;
	qBuffer.resize(bufferSize);
}

void Charge::clearBuffers()
{
	qshots = 0;
	qBuffer.clear();
}

boost::python::dict Charge::getRunningStats_Py()
{
	boost::python::dict r;
	r["q_rs"] = qStats.getRunningStats();
	return r;
}

RunningStats& Charge::getQRunningStats()
{
	return qStats;
}

void Charge::clearRunningStats()
{
	std::cout << "Charge qStats.Clear " << std::endl;
	return qStats.Clear();
}
void Charge::setRunningStatSize(size_t new_size)
{
	std::cout << "Charge qStats.setMaxCount " << new_size << std::endl;
	return qStats.setMaxCount(new_size);
}
size_t Charge::getRunningStatSize()const 
{
	return qStats.getMaxCount();
}

size_t Charge::getRunningStatCount()
{
	return qStats.NumDataValues();
}
bool Charge::isRunningStatFull()
{
	return qStats.Full();
}
size_t Charge::getRunningStatNumDataValues()const
{
	return qStats.NumDataValues();
}

void Charge::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Charge::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG On");
	epicsInterface->debugMessagesOn();
}

void Charge::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Charge::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES On");
	epicsInterface->messagesOn();
}