#include "BPMFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <utility>
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
		{						\
		SEVCHK(status, NULL);	\
		exit(status);			\
		}						\
}								\

BPMFactory::BPMFactory() : BPMFactory(false)
{
}
BPMFactory::BPMFactory(bool isVirtual)
{
	messenger = LoggingSystem(false, false);
	hasBeenSetup = false;
	messenger.printDebugMessage("BPM Factory Constructed");
	isVirtual = isVirtual;
	reader = ConfigReader("BPM", isVirtual);
}
BPMFactory::BPMFactory(const BPMFactory& copyBPMFactory)
	: hasBeenSetup(copyBPMFactory.hasBeenSetup),
	isVirtual(copyBPMFactory.isVirtual),
	messenger(copyBPMFactory.messenger),
	reader(copyBPMFactory.reader)
{
	bpmMap.insert(copyBPMFactory.bpmMap.begin(), copyBPMFactory.bpmMap.end());
}

BPMFactory::~BPMFactory()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage("BPMFactory Destructor Called");
	for (auto& bpm : bpmMap)
	{
		auto pvStructsList = bpm.second.getPVStructs();
		for (auto& pvStruct : pvStructsList)
		{
			if (pvStruct.second.monitor)
			{
				bpm.second.epicsInterface->removeSubscription(pvStruct.second);
				ca_flush_io();
			}
			bpm.second.epicsInterface->removeChannel(pvStruct.second);
			ca_pend_io(CA_PEND_IO_TIMEOUT);
		}
	}
}

void BPMFactory::populateBPMMap()
{
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		reader.parseNextYamlFile(bpmMap);
	}
}

void BPMFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == "X" ||
		pvStruct.pvRecord == "Y" ||
		pvStruct.pvRecord == "SA1" ||
		pvStruct.pvRecord == "SA2" ||
		pvStruct.pvRecord == "SD1" ||
		pvStruct.pvRecord == "SD2" ||
		pvStruct.pvRecord == "RA1" ||
		pvStruct.pvRecord == "RA2" ||
		pvStruct.pvRecord == "RD1" ||
		pvStruct.pvRecord == "RD2" ||
		pvStruct.pvRecord == "DATA:B2V.VALA" ||
		pvStruct.pvRecord == "AWAK" || 
		pvStruct.pvRecord == "RDY" )
	{
		pvStruct.monitor = true;
	}
}

bool BPMFactory::setup(const std::string &version)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (this->isVirtual)
	{
		messenger.debugMessagesOff();
		messenger.printDebugMessage(" VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in BPM constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateBPMMap();
	for (auto &bpm : bpmMap)
	{
		std::map<std::string, pvStruct>& bpmPVStructs = bpm.second.getPVStructs();
		for (auto &pv : bpmPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			retrieveMonitorStatus(pv.second);
			bpm.second.epicsInterface->retrieveCHID(pv.second);
			bpm.second.epicsInterface->retrieveCHTYPE(pv.second);
			bpm.second.epicsInterface->retrieveCOUNT(pv.second);
			bpm.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
			// not sure how to set the mask from EPICS yet.
			pv.second.MASK = DBE_VALUE;
			messenger.printDebugMessage(pv.second.pvRecord + ": read" + std::to_string(ca_read_access(pv.second.CHID)) +
				"write" + std::to_string(ca_write_access(pv.second.CHID)) +
				"state" + std::to_string(ca_state(pv.second.CHID)) + "\n");
			if (pv.second.monitor)
			{
				bpm.second.epicsInterface->createSubscription(bpm.second, pv.second);
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

std::map<std::string, BPM> BPMFactory::getBPMs(std::vector<std::string> bpmNames)
{
	std::map<std::string, BPM> selectedBPMs;
	for (auto & bpmName : bpmNames)
	{
		selectedBPMs[bpmName] = bpmMap.find(bpmName)->second;
	}
	return selectedBPMs;
}

BPM& BPMFactory::getBPM(const std::string& fullBPMName)
{
	return bpmMap.find(fullBPMName)->second;
}

std::map<std::string, BPM> BPMFactory::getAllBPMs()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("BPMS HAVE ALREADY BEEN CONSTRUCTED.");
	}
	return bpmMap;
}

double BPMFactory::getX(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getX();
	}
	return std::numeric_limits<double>::min();;
}

double BPMFactory::getXFromPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getXFromPV();
	}
	return std::numeric_limits<double>::min();;
}

double BPMFactory::getY(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getY();
	}
	return std::numeric_limits<double>::min();;
}

double BPMFactory::getYFromPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getYFromPV();
	}
	return std::numeric_limits<double>::min();;
}

std::pair<double, double> BPMFactory::getXYPosition(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		double x = bpmMap.find(name)->second.getYFromPV();
		double y = bpmMap.find(name)->second.getYFromPV();
		return std::make_pair(x, y);
	}
	return std::make_pair(std::numeric_limits<double>::min(), std::numeric_limits<double>::min());
}

double BPMFactory::getQ(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getQ();
	}
	return std::numeric_limits<double>::min();;
}

double BPMFactory::getPosition(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getPosition();
	}
	return std::numeric_limits<double>::min();;
}

double BPMFactory::getResolution(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getResolution();
	}
	return std::numeric_limits<double>::min();;
}

long BPMFactory::getSA1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSA1();
	}
	return std::numeric_limits<long>::min();;
}

long BPMFactory::getSA2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSA2();
	}
	return std::numeric_limits<long>::min();;
}

long BPMFactory::getSD1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSD1();
	}
	return std::numeric_limits<long>::min();;
}

long BPMFactory::getSD2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSD2();
	}
	return std::numeric_limits<long>::min();;
}

bool BPMFactory::setSA1(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSA1(value);
	}
	return false;
}

bool BPMFactory::setSA2(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSA2(value);
	}
	return false;
}

bool BPMFactory::setSD1(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSD1(value);
	}
	return false;
}

bool BPMFactory::setSD2(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSD2(value);
	}
	return false;
}

long BPMFactory::getRA1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRA1();
	}
	return std::numeric_limits<long>::min();;
}

long BPMFactory::getRA2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRA2();
	}
	return std::numeric_limits<long>::min();;
}

long BPMFactory::getRD1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRD1();
	}
	return std::numeric_limits<long>::min();;
}

long BPMFactory::getRD2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRD2();
	}
	return std::numeric_limits<long>::min();;
}

bool BPMFactory::reCalAttenuation(const std::string& name, const double& charge)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.reCalAttenuation(charge);
	}
	return false;
}

std::map<std::string, double> BPMFactory::getX(const std::vector<std::string>& names)
{
	std::map<std::string, double> xpositions;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getX();
		xpositions[name] = x;
	}
	return xpositions;
}

std::map<std::string, double> BPMFactory::getY(const std::vector<std::string>& names)
{
	std::map<std::string, double> ypositions;
	for (auto name : names)
	{
		double y = bpmMap.find(name)->second.getY();
		ypositions[name] = y;
	}
	return ypositions;
}

std::map<std::string, double> BPMFactory::getXFromPV(const std::vector<std::string>& names)
{
	std::map<std::string, double> xpositions;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getXFromPV();
		xpositions[name] = x;
	}
	return xpositions;
}

std::map<std::string, double> BPMFactory::getYFromPV(const std::vector<std::string>& names)
{
	std::map<std::string, double> ypositions;
	for (auto name : names)
	{
		double y = bpmMap.find(name)->second.getYFromPV();
		ypositions[name] = y;
	}
	return ypositions;
}

std::map<std::string, double> BPMFactory::getQ(const std::vector<std::string>& names)
{
	std::map<std::string, double> qmap;
	for (auto name : names)
	{
		double q = bpmMap.find(name)->second.getQ();
		qmap[name] = q;
	}
	return qmap;
}

std::map<std::string, std::pair<double, double>> BPMFactory::getXYPosition(const std::vector<std::string>& names)
{
	std::map<std::string, std::pair<double, double>> bpmsAndPositionsMap;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getXFromPV();
		double y = bpmMap.find(name)->second.getXFromPV();
		std::pair<double, double> positionsPair = std::make_pair(x, y);
		std::pair<std::string, std::pair<double, double>> nameAndPositionsPair = std::make_pair(bpmMap.find(name)->first, positionsPair);
		bpmsAndPositionsMap.insert(nameAndPositionsPair);
	}
	return bpmsAndPositionsMap;
}

std::map<std::string, double> BPMFactory::getAllX()
{
	std::map<std::string, double> bpmsAndXMap;
	for (auto bpm: bpmMap)
	{
		std::pair<std::string, double> nameAndXPair = std::make_pair(bpm.first, bpm.second.getX());
		bpmsAndXMap.insert(nameAndXPair);
	}
	return bpmsAndXMap;
}
std::map<std::string, double> BPMFactory::getAllXFromPV()
{
	std::map<std::string, double> bpmsAndXMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndXPair = std::make_pair(bpm.first, bpm.second.getXFromPV());
		bpmsAndXMap.insert(nameAndXPair);
	}
	return bpmsAndXMap;
}

std::map<std::string, double> BPMFactory::getAllY()
{
	std::map<std::string, double> bpmsAndYMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndYPair = std::make_pair(bpm.first, bpm.second.getY());
		bpmsAndYMap.insert(nameAndYPair);
	}
	return bpmsAndYMap;
}

std::map<std::string, double> BPMFactory::getAllYFromPV()
{
	std::map<std::string, double> bpmsAndYMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndYPair = std::make_pair(bpm.first, bpm.second.getYFromPV());
		bpmsAndYMap.insert(nameAndYPair);
	}
	return bpmsAndYMap;
}

std::map<std::string, double> BPMFactory::getAllQ()
{
	std::map<std::string, double> bpmsAndQMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndQPair = std::make_pair(bpm.first, bpm.second.getQ());
		bpmsAndQMap.insert(nameAndQPair);
	}
	return bpmsAndQMap;
}

std::map<std::string, std::pair<double, double>> BPMFactory::getAllXYPosition()
{
	std::map<std::string, std::pair<double, double>> bpmsAndPositionsMap;
	for (auto bpm : bpmMap)
	{
		std::pair<double, double> positionsPair = std::make_pair(bpm.second.getXFromPV(), bpm.second.getYFromPV());
		std::pair<std::string, std::pair<double, double>> nameAndPositionsPair = std::make_pair(bpm.first, positionsPair);
		bpmsAndPositionsMap.insert(nameAndPositionsPair);
	}
	return bpmsAndPositionsMap;
}

boost::python::dict BPMFactory::getX_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> xvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xvals = getX(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getXFromPV_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> xvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xvals = getXFromPV(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getY_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> yvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	yvals = getY(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getYFromPV_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> yvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	yvals = getYFromPV(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getQ_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> qvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	qvals = getQ(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getXYPosition_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::pair<double, double>> xyvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xyvals = getXYPosition(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllX_Py()
{
	std::map<std::string, double> xvals = getAllX();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllXFromPV_Py()
{
	std::map<std::string, double> xvals = getAllXFromPV();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllY_Py()
{
	std::map<std::string, double> yvals = getAllY();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllYFromPV_Py()
{
	std::map<std::string, double> yvals = getAllYFromPV();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllQ_Py()
{
	std::map<std::string, double> qvals = getAllQ();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllXYPositions_Py()
{
	std::map<std::string, std::pair<double, double>> xyvals = getAllXYPosition();
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}
