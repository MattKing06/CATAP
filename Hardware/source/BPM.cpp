#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include "BPM.h"
#include <map>
#include <vector>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>

BPM::BPM()
{}

BPM::BPM(const std::map<std::string, std::string> &paramsMap, bool isVirtual) :
Hardware(paramsMap, isVirtual),
bpmType(paramsMap.find("bpm_type")->second)
{
	bufferSize = 10;
	epicsInterface = boost::make_shared<EPICSBPMInterface>(EPICSBPMInterface());
	xBuffer.resize(bufferSize);
	xPVBuffer.resize(bufferSize);
	yBuffer.resize(bufferSize);
	yPVBuffer.resize(bufferSize);
	qBuffer.resize(bufferSize);
}
BPM::BPM(const BPM& copyBPM) : 
Hardware(copyBPM),
bpmType(copyBPM.bpmType),
epicsInterface(copyBPM.epicsInterface)
{
}
std::vector<std::string> BPM::getAliases() const
{
	return this->aliases;
}
std::string BPM::getBPMType() const
{
	return this->bpmType;
}

//bool Magnet::setEPICSCurrent(const double &value)
//{
//	std::map<std::string, pvStruct>& pvData = getPVStructs();
//	for (auto &pv : pvData)
//	{
//		if (pv.second.pvRecord == "SETI")
//		{
//			messenger.printDebugMessage("SETTING TO VALUE: " + std::to_string(value) + " for [" + pv.second.fullPVName + "]");
//			this->epicsInterface->setNewCurrent(value, pv.second);
//		}
//	}
//	// subscription should sense current has changed and call 'updateCurrent' in MagnetEPICSInterface
//	return true;
//}

STATE BPM::getBPMState() const
{
	return bpmState;
}

double BPM::getX() const
{
	return this->x;
}

double BPM::getXFromPV() const
{
	return this->xPV;
}

double BPM::getY() const
{
	return this->y;
}

double BPM::getYFromPV() const
{
	return this->yPV;
}

double BPM::getQ() const
{
	return this->q;
}

double BPM::getResolution() const
{
	return this->resolution;
}

double BPM::getPosition() const
{
	return this->position;
}

size_t BPM::getBufferSize() const
{
	return this->bufferSize;
}

boost::circular_buffer< double > BPM::getBPMXBuffer() const
{
	return this->xBuffer;
}

boost::circular_buffer< double > BPM::getBPMXPVBuffer() const
{
	return this->xPVBuffer;
}

boost::circular_buffer< double > BPM::getBPMYBuffer() const
{
	return this->yBuffer;
}

boost::circular_buffer< double > BPM::getBPMYPVBuffer() const
{
	return this->yPVBuffer;
}

boost::circular_buffer< double > BPM::getBPMQBuffer() const
{
	return this->qBuffer;
}

long BPM::getSA1() const
{
	return this->sa1;
}

long BPM::getSA2() const
{
	return this->sa2;
}

long BPM::getSD1() const
{
	return this->sd1;
}

long BPM::getSD2() const
{
	return this->sd2;
}

long BPM::getRA1() const
{
	return this->ra1;
}

long BPM::getRA2() const
{
	return this->ra2;
}

long BPM::getRD1() const
{
	return this->rd1;
}

long BPM::getRD2() const
{
	return this->rd2;
}

bool BPM::setXPV(const double& value)
{
	xPV = value;
	return true;
}

bool BPM::setYPV(const double& value)
{
	yPV = value;
	return true;
}

bool BPM::setXPVBuffer(const double& value)
{
	xPVBuffer.push_back(value);
	return true;
}

bool BPM::setYPVBuffer(const double& value)
{
	yPVBuffer.push_back(value);
	return true;
}

bool BPM::setRA1(const long& value)
{
	ra1 = value;
	return true;
}

bool BPM::setRA2(const long& value)
{
	ra2 = value;
	return true;
}

bool BPM::setRD1(const long& value)
{
	rd1 = value;
	return true;
}

bool BPM::setRD2(const long& value)
{
	rd2 = value;
	return true;
}

bool BPM::setSA1(const long& value)
{
	sa1 = value;
	return true;
}

bool BPM::setSA2(const long& value)
{
	sa2 = value;
	return true;
}

bool BPM::setSD1(const long& value)
{
	sd1 = value;
	return true;
}

bool BPM::setSD2(const long& value)
{
	sd2 = value;
	return true;
}

void BPM::setBufferSize(const size_t& value)
{
	bufferSize = value;
	xBuffer.resize(bufferSize);
	xPVBuffer.resize(bufferSize);
	yBuffer.resize(bufferSize);
	yPVBuffer.resize(bufferSize);
	qBuffer.resize(bufferSize);
}

void BPM::clearBuffers()
{
	xBuffer.clear();
	xPVBuffer.clear();
	yBuffer.clear();
	yPVBuffer.clear();
	qBuffer.clear();
}