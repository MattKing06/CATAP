#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <Screen.h>
#include <map>
#include <vector>
#include <numeric>
#include <math.h>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>

Screen::Screen()
{}
Screen::Screen(const std::map<std::string, std::string> & paramsMap, STATE mode) :
Hardware(paramsMap, mode),
screenType(paramsMap.find("screen_type")->second),
has_camera(paramsMap.find("has_camera")->second),
name(paramsMap.find("name")->second),
position(std::stod(paramsMap.find("position")->second))
{
messenger.printDebugMessage("constructor");
setPVStructs();
epicsInterface = boost::make_shared<EPICSScreenInterface>(EPICSScreenInterface());
epicsInterface->ownerName = hardwareName;
}
Screen::Screen(const Screen & copyScreen) :
Hardware(copyScreen),
screenType(copyScreen.screenType),
name(copyScreen.name),
has_camera(copyScreen.has_camera),
position(copyScreen.position),
epicsInterface(copyScreen.epicsInterface)
{
}

void Screen::setPVStructs()
{
	messenger.printDebugMessage("in setPVstructs");

	for (auto&& record : ScreenRecords::bpmRecordList)
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

std::vector<std::string> Screen::getAliases() const
{
	return this->aliases;
}

std::string Screen::getScreenType() const
{
	return this->screenType;
}

std::string Screen::getScreenName() const
{
	return this->name;
}

double Screen::getPosition() const
{
	return this->position;
}

STATE Screen::getScreenState() const
{
	return this->state.second;
}

STATE Screen::getScreenSetState() const
{
	return this->setstate.second;
}

std::string Screen::getScreenType() const
{
	return this->screen.second;
}

bool Screen::isHOut() const
{

}

bool Screen::isVOut() const
{

}

bool Screen::isHIn() const
{

}

bool Screen::isVIn() const
{

}

bool Screen::is_HandV_OUT() const
{

}

bool Screen::isScreenIn() const
{

}

bool Screen::isHMoving() const
{

}

bool Screen::isVMoving() const
{

}

bool Screen::isPMoving() const
{

}

bool Screen::isMoving() const
{

}

bool Screen::isClearForBeam() const
{

}

bool Screen::isHOut() const
{

}

bool Screen::isVOut() const
{

}

bool Screen::isHIn() const
{

}

bool Screen::isVIn() const
{

}

bool Screen::isMover() const
{

}

bool Screen::isVMover() const
{

}

bool Screen::isHVMover() const
{

}

bool Screen::isPneumatic() const
{

}

bool Screen::isScreenInState(STATE sta) const
{

}

bool Screen::isYAGIn() const
{

}
bool Screen::isHElement(STATE e) const
{

}

bool Screen::isVElement(STATE e) const
{

}

bool Screen::isPElement(STATE e) const
{

}

bool Screen::isHEnabled() const
{

}

bool Screen::isVEnabled() const
{

}

double Screen::getACTPOS() const
{

}

double Screen::getJDiff() const
{

}

double Screen::getDevicePosition(STATE state) const
{

}

double Screen::getPosition() const
{

}

bool Screen::isVELAPneumatic() const
{

}

bool Screen::isVELAHVMover() const
{

}

bool Screen::isCLARAHVMover() const
{

}

bool Screen::isCLARAVMover() const
{

}

double Screen::get_H_ACTPOS() const
{

}

double Screen::get_V_ACTPOS() const
{

}

bool Screen::isXPVBufferFull() const
{
	if (xpvshots >= bufferSize)
	{
		return true;
	}
	return false;
}

void Screen::moveScreenTo(STATE& state)
{

}

void Screen::insertYAG()
{

}

void Screen::makeReadEqualSet()
{

}

void Screen::makeSetEqualRead()
{

}

void Screen::moveScreenOut()
{

}

void Screen::resetPosition()
{

}

void Screen::jogScreen(const double jog)
{

}

void Screen::setPosition(const double setPos)
{

}

bool Screen::setScreenSDEV(STATE& state)
{

}

bool Screen::setScreenTrigger()
{

}

bool Screen::setScreenTrigger(STATE& state)
{

}

bool Screen::setEX()
{

}

bool Screen::setEN(STATE direction)
{

}

bool Screen::isYPVBufferFull() const
{
	if (ypvshots >= bufferSize)
	{
		return true;
	}
	return false;
}

bool Screen::isDataBufferFull() const
{
	if (datashots >= bufferSize)
	{
		return true;
	}
	return false;
}

size_t Screen::getBufferSize() const
{
	return this->bufferSize;
}

boost::circular_buffer< double > Screen::getXBuffer() const
{
	return this->xBuffer;
}

boost::circular_buffer< double > Screen::getXPVBuffer() const
{
	return this->xPVBuffer;
}

boost::circular_buffer< double > Screen::getYBuffer() const
{
	return this->yBuffer;
}

boost::circular_buffer< double > Screen::getYPVBuffer() const
{
	return this->yPVBuffer;
}

boost::circular_buffer< double > Screen::getQBuffer() const
{
	return this->qBuffer;
}

boost::circular_buffer< std::vector< double > > Screen::getDataBuffer() const
{
	return this->dataBuffer;
}

long Screen::getSA1() const
{
	return this->sa1.second;
}

long Screen::getSA2() const
{
	return this->sa2.second;
}

long Screen::getSD1() const
{
	return this->sd1.second;
}

long Screen::getSD2() const
{
	return this->sd2.second;
}

long Screen::getRA1() const
{
	return this->ra1.second;
}

long Screen::getRA2() const
{
	return this->ra2.second;
}

long Screen::getRD1() const
{
	return this->rd1.second;
}

long Screen::getRD2() const
{
	return this->rd2.second;
}

bool Screen::setXPV(const double& value)
{
	xPV.second = value;
	xPVBuffer.push_back(value);
	xpvshots++;
	if (monitoringxpv)
	{
		if (xpvshots <= xPVVector.size())
		{
			xPVVector[xpvshots - 1] = value;
		}
		else
		{
			monitoringxpv = false;
		}
	}
	return true;
}

bool Screen::setYPV(const double& value)
{
	yPV.second = value;
	yPVBuffer.push_back(value);
	ypvshots++;
	if (monitoringypv)
	{
		if (ypvshots <= yPVVector.size())
		{
			yPVVector[ypvshots - 1] = value;
		}
		else
		{
			monitoringypv = false;
		}
	}
	return true;
}

bool Screen::ismonitoringXPV() const
{
	return monitoringxpv;
}

bool Screen::ismonitoringYPV() const
{
	return monitoringypv;
}

bool Screen::ismonitoringData() const
{
	return monitoringdata;
}

bool Screen::ismonitoring() const
{
	if (monitoringxpv || monitoringypv || monitoringdata)
	{
		return true;
	}
	return false;
}

void Screen::offlineSet(const long& value)
{
	epicsTimeGetCurrent(&sa1.first);
	sa1.first = sa1.first;
	sa1.second = value;
	sa1.second = value;
}

bool Screen::setRA1(const long& value)
{
	ra1.second = value;
	return true;
}

bool Screen::setRA2(const long& value)
{
	ra2.second = value;
	return true;
}

bool Screen::setRD1(const long& value)
{
	rd1.second = value;
	return true;
}

bool Screen::setRD2(const long& value)
{
	rd2.second = value;
	return true;
}

bool Screen::setSA1(const long& value)
{
	switch (mode)
	{
	case STATE::PHYSICAL:
		epicsInterface->setSA1(value, pvStructs.at(ScreenRecords::SA1));
		break;
	case STATE::VIRTUAL:
		epicsInterface->setSA1(value, pvStructs.at(ScreenRecords::SA1));
		break;
	default:
		offlineSet(value);
	}
	return true;
}

bool Screen::setSA2(const long& value)
{
	switch (mode)
	{
	case STATE::PHYSICAL:
		epicsInterface->setSA2(value, pvStructs.at(ScreenRecords::SA2));
		break;
	case STATE::VIRTUAL:
		epicsInterface->setSA2(value, pvStructs.at(ScreenRecords::SA2));
		break;
	default:
		offlineSet(value);
	}
	return true;
}

bool Screen::setSD1(const long& value)
{
	switch (mode)
	{
	case STATE::PHYSICAL:
		epicsInterface->setSD1(value, pvStructs.at(ScreenRecords::SD1));
		break;
	case STATE::VIRTUAL:
		epicsInterface->setSD1(value, pvStructs.at(ScreenRecords::SD1));
		break;
	default:
		offlineSet(value);
	}
	return true;
}

bool Screen::setSD2(const long& value)
{
	switch (mode)
	{
	case STATE::PHYSICAL:
		epicsInterface->setSD2(value, pvStructs.at(ScreenRecords::SD2));
		break;
	case STATE::VIRTUAL:
		epicsInterface->setSD2(value, pvStructs.at(ScreenRecords::SD2));
		break;
	default:
		offlineSet(value);
	}
	return true;
}

bool Screen::setAWAK(const double& value)
{
	awak.second = value;
	return true;
}

bool Screen::setAWAKTStamp(const double& value)
{
	awaktstamp = value;
	return true;
}

bool Screen::setRDY(const double& value)
{
	rdy.second = value;
	return true;
}

bool Screen::setRDYTStamp(const double& value)
{
	rdytstamp = value;
	return true;
}

bool Screen::setData(const std::vector< double >& value)
{
	data.second = value;
	pu1 = value[1];
	pu2 = value[2];
	c1 = value[3];
	p1 = value[4];
	pu3 = value[5];
	pu4 = value[6];
	c2 = value[7];
	p2 = value[8];
	dataBuffer.push_back(value);
	pu1Buffer.push_back(pu1);
	pu2Buffer.push_back(pu2);
	pu3Buffer.push_back(pu3);
	pu4Buffer.push_back(pu4);
	c1Buffer.push_back(c1);
	c2Buffer.push_back(c2);
	p1Buffer.push_back(p1);
	p2Buffer.push_back(p2);
	setResolution();
	setQ(value);
	qBuffer.push_back(q.second);
	datashots++;
	if (monitoringdata)
	{
		if (datashots <= dataVector.size())
		{
			dataVector[datashots - 1] = value;
			qVector[datashots - 1] = q.second;
		}
		else
		{
			monitoringdata = false;
		}
	}
	checkStatus();
	return true;
}

bool Screen::setQ(const std::vector< double >& rawData)
{
	double v1, v2, q1, q2;
	double u11, u12, u13, u14, u21, u22, u23, u24;
	u11 = rawData[1];
	u12 = rawData[2];
	u13 = rawData[3];
	u14 = rawData[4];
	u21 = rawData[5];
	u22 = rawData[6];
	u23 = rawData[7];
	u24 = rawData[8];

	v1 = (std::abs(u11 - u14) + std::abs(u12 - u14)) / 2;
	v2 = (std::abs(u21 - u24) + std::abs(u22 - u24)) / 2;
	q1 = (qcal * (v1 / v1cal)) * (pow(10, -((att1cal - ra1.second) / 20)));
	q2 = (qcal * (v2 / v2cal)) * (pow(10, -((att2cal - ra2.second) / 20)));
	q.second = ((q1 + q2) / 2);

	return true;
}

bool Screen::setResolution()
{
	double u11, u12, u13, u14, u21, u22, u23, u24, v11, v12, v21, v22;
	double rmsVals;
	u11 = std::accumulate(pu1Buffer.begin(), pu1Buffer.end(), 0.0) / pu1Buffer.size();
	u12 = std::accumulate(pu2Buffer.begin(), pu2Buffer.end(), 0.0) / pu2Buffer.size();
	u13 = std::accumulate(c1Buffer.begin(), c1Buffer.end(), 0.0) / c1Buffer.size();
	u14 = std::accumulate(p1Buffer.begin(), p1Buffer.end(), 0.0) / p1Buffer.size();
	u21 = std::accumulate(pu3Buffer.begin(), pu3Buffer.end(), 0.0) / pu3Buffer.size();
	u22 = std::accumulate(pu4Buffer.begin(), pu4Buffer.end(), 0.0) / pu4Buffer.size();
	u23 = std::accumulate(c2Buffer.begin(), c2Buffer.end(), 0.0) / c2Buffer.size();
	u24 = std::accumulate(p2Buffer.begin(), p2Buffer.end(), 0.0) / p2Buffer.size();
	v11 = u11 - u14;
	v12 = u12 - u14;
	v21 = u21 - u24;
	v22 = u22 - u24;
	if (v11 && v12 && v21 && v22 != 0)
	{
		rmsVals = ((v11 + v12) - (v21 + v22)) / ((v11 + v12) + (v21 + v22));
		resolution.second = rmsVals * sqrt(2) * (0.001 * mn);
	}
	return true;
}

bool Screen::checkBuffer(boost::circular_buffer< double >& buf)
{
	if (buf[buf.size() - 1] == buf[buf.size()])
	{
		return true;	
	}
	return false;
}

void Screen::checkStatus()
{
	/*if (awak.first - rdy.first > 1.0)
	{
		status = STATE::BAD;
		statusBuffer.push_back(status);
	}*/
	if (xpvshots == 0 || ypvshots == 0 || datashots == 0)
	{
		status = STATE::BAD;
		statusBuffer.push_back(status);
	}
	else if (checkBuffer(xPVBuffer) || checkBuffer(yPVBuffer))
	{
		status = STATE::BAD;
		statusBuffer.push_back(status);
	}
	else if (checkBuffer(pu1Buffer) || checkBuffer(pu2Buffer) || checkBuffer(pu3Buffer) || checkBuffer(pu4Buffer))
	{
		status = STATE::BAD;
		statusBuffer.push_back(status);
	}
	else if (xpvshots > 0 && ypvshots > 0)
		if(isnan(xPVBuffer.back()) || isnan(yPVBuffer.back()))
		{
			status = STATE::BAD;
			statusBuffer.push_back(status);
		}
	else if (abs(pu1Buffer.back()) > 1.0 || abs(pu2Buffer.back()) > 1.0 || abs(pu3Buffer.back()) > 1.0 || abs(pu4Buffer.back()) > 1.0)
	{
		status = STATE::NONLINEAR;
		statusBuffer.push_back(status);
	}
	else if (abs(pu1Buffer.back()) < 1.0 || abs(pu2Buffer.back()) < 1.0 || abs(pu3Buffer.back()) < 1.0 || abs(pu4Buffer.back()) < 1.0)
	{
		status = STATE::GOOD;
		statusBuffer.push_back(status);
	}
	else
	{
		status = STATE::UNKNOWN;
		statusBuffer.push_back(status);
	}
	if (ismonitoring())
	{
		statusVector.push_back(status);
	}
}

bool Screen::reCalAttenuation(const double& charge)
{
	double qqC = charge / qcal;

	long currAtt1 = ra1.second;
	long currAtt2 = ra2.second;

	long newAtt1 = (20 * log10(qqC)) + att1cal;
	long newAtt2 = (20 * log10(qqC)) + att2cal;

	if (0 <= newAtt1 && newAtt1 <= 20)
	{
		setSA1(newAtt1);
	}
	else if (charge < 20)
	{
		setSA1(0);
	}
	else
	{
		setSA1(currAtt1);
	}


	if (0 <= newAtt2 && newAtt2 <= 20)
	{
		setSA2(newAtt2);
	}
	else if (charge < 20)
	{
		setSA2(0);
		//message("New SA2 for ", name, " = ", getRA2());
	}
	else
	{
		setSA2(currAtt2);
		//message("Old SA2 for ", name, " = ", getRA2());
	}
	return true;
}

void Screen::monitorForNShots(const size_t& value)
{
	setVectorSize(value);
	monitoringxpv = true;
	monitoringypv = true;
	monitoringdata = true;
}

void Screen::setVectorSize(const size_t& value)
{
	clearBuffers();
	xPVVector.clear();
	yPVVector.clear();
	qVector.clear();
	statusVector.clear();
	dataVector.clear();
	vectorSize = value;
	xPVVector.resize(vectorSize);
	yPVVector.resize(vectorSize);
	qVector.resize(vectorSize);
	statusVector.resize(vectorSize);
	dataVector.resize(vectorSize);
	for (auto&& it2 : dataVector)
	{
		it2.resize(9);
	}
}

void Screen::setBufferSize(const size_t& value)
{
	clearBuffers();
	bufferSize = value;
	xBuffer.resize(bufferSize);
	xPVBuffer.resize(bufferSize);
	yBuffer.resize(bufferSize);
	yPVBuffer.resize(bufferSize);
	qBuffer.resize(bufferSize);
	statusBuffer.resize(bufferSize);
	dataBuffer.resize(bufferSize);
	pu1Buffer.resize(bufferSize);
	pu2Buffer.resize(bufferSize);
	pu3Buffer.resize(bufferSize);
	pu4Buffer.resize(bufferSize);
	c1Buffer.resize(bufferSize);
	c2Buffer.resize(bufferSize);
	p1Buffer.resize(bufferSize);
	p2Buffer.resize(bufferSize);
	for (auto&& it2 : dataBuffer)
	{
		it2.resize(9);
	}
}

void Screen::clearBuffers()
{
	xpvshots = 0;
	ypvshots = 0;
	datashots = 0;
	xBuffer.clear();
	xPVBuffer.clear();
	yBuffer.clear();
	yPVBuffer.clear();
	qBuffer.clear();
	dataBuffer.clear();
	statusBuffer.clear();
	pu1Buffer.clear();
	pu2Buffer.clear();
	pu3Buffer.clear();
	pu4Buffer.clear();
	c1Buffer.clear();
	c2Buffer.clear();
	p1Buffer.clear();
	p2Buffer.clear();
}

void Screen::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Screen::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG On");
	epicsInterface->debugMessagesOn();
}

void Screen::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Screen::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES On");
	epicsInterface->messagesOn();
}