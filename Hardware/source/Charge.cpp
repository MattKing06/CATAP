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
chargeType(paramsMap.find("charge_type")->second),
name(paramsMap.find("name")->second),
positiOn(std::stod(paramsMap.find("positiOn")->second))
{
	bufferSize = 10;
	qshots = 0;
	epicsInterface = boost::make_shared<EPICSChargeInterface>(EPICSChargeInterface());
	qBuffer.resize(bufferSize);
}
Charge::Charge(const Charge& copyCharge) :
Hardware(copyCharge),
chargeType(copyCharge.chargeType),
name(copyCharge.name),
positiOn(copyCharge.positiOn),
epicsInterface(copyCharge.epicsInterface)
{
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

//STATE BPM::getBPMState() const
//{
//	return bpmState;
//}

double Charge::getQ() const
{
	return this->q;
}

std::vector< double > Charge::getQVector() const
{
	//if (monitoringxpv)
	//{
	//	LoggingSystem::printDebugMessage("WARNING: STILL monitorING X PV -- VECTOR NOT FULL");
	//}
	return this->qVector;
}

double Charge::getPositiOn() const
{
	return this->positiOn;
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

bool Charge::setQ(const double& value)
{
	q = value;
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