#include "BPMFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <utility>
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif

BPMFactory::BPMFactory() : BPMFactory(STATE::OFFLINE)
{
}
BPMFactory::BPMFactory(STATE mode):
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("BPM", mode))
{
	messenger = LoggingSystem(true, true);
	//hasBeenSetup = false;
//	messenger = LoggingSystem(false, false);
	messenger.printDebugMessage("BPM Factory constructed");
	//mode = mode;
	//reader = ConfigReader("BPM", mode);
}
BPMFactory::BPMFactory(const BPMFactory& copyBPMFactory)
	: hasBeenSetup(copyBPMFactory.hasBeenSetup),
	mode(copyBPMFactory.mode),
	messenger(copyBPMFactory.messenger),
	reader(copyBPMFactory.reader)
{
	bpmMap.insert(copyBPMFactory.bpmMap.begin(), copyBPMFactory.bpmMap.end());
}

BPMFactory::~BPMFactory()
{
	messenger.printDebugMessage("BPMFactory Destructor Called");
	if (hasBeenSetup)
	{
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
}

void BPMFactory::populateBPMMap()
{
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive cOnfiguratiOn parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		reader.parseNextYamlFile(bpmMap);
	}
}

void BPMFactory::retrievemonitorStatus(pvStruct& pvStruct)
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
		pvStruct.pvRecord == "RDY")
	{
		pvStruct.monitor = true;
	}
}

bool BPMFactory::setup(const std::string& VERSION)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in BPM constructor
	//// but we have a lot of PV informatiOn to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateBPMMap();
	for (auto& bpm : bpmMap)
	{
		std::map<std::string, pvStruct>& bpmPVStructs = bpm.second.getPVStructs();
		for (auto& pv : bpmPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			retrievemonitorStatus(pv.second);
			bpm.second.epicsInterface->retrieveCHID(pv.second);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				bpm.second.epicsInterface->retrieveCHTYPE(pv.second);
				bpm.second.epicsInterface->retrieveCOUNT(pv.second);
				bpm.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					bpm.second.epicsInterface->createSubscription(bpm.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(bpm.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	EPICSInterface::sendToEPICS();
	hasBeenSetup = true;
	return hasBeenSetup;
}

std::map<std::string, BPM> BPMFactory::getBPMs(std::vector<std::string> bpmNames)
{
	std::map<std::string, BPM> selectedBPMs;
	for (auto& bpmName : bpmNames)
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
		messenger.printDebugMessage("BPMS HAVE ALREADY BEEN constRUCTED.");
	}
	return bpmMap;
}

std::string BPMFactory::getBPMName(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getBPMName();
	}
	return "0";
}

void BPMFactory::monitorForNShots(const std::string& name, const size_t& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		bpmMap.find(name)->second.monitorForNShots(value);
	}
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

std::vector< double > BPMFactory::getData(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getData();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::pair<double, double> BPMFactory::getXYPositiOn(const std::string& name)
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

double BPMFactory::getPositiOn(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getPositiOn();
	}
	return std::numeric_limits<double>::min();;
}

double BPMFactory::getResolutiOn(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getResolutiOn();
	}
	return std::numeric_limits<double>::min();;
}

std::vector< double > BPMFactory::getXPVVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getXPVVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::vector< double > BPMFactory::getYPVVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getYPVVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::vector< double > BPMFactory::getQVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getQVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::vector< std::vector< double > > BPMFactory::getDataVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getDataVector();
	}
	std::vector< double > vector2(9, std::numeric_limits<double>::min());
	std::vector< std::vector< double > > vector3(9, vector2);
	return vector3;
}

boost::circular_buffer< double > BPMFactory::getXPVBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getXPVBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< double > BPMFactory::getYPVBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getYPVBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< double > BPMFactory::getQBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getQBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< std::vector< double > > BPMFactory::getDataBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getDataBuffer();
	}
	std::vector< double > vector2(9, std::numeric_limits<double>::min());
	boost::circular_buffer< std::vector< double > > vector3(9, vector2);
	return vector3;
}

bool BPMFactory::ismonitoringXPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoringXPV();
	}
	return false;
}

bool BPMFactory::ismonitoring(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoring();
	}
	return false;
}

bool BPMFactory::ismonitoringYPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoringYPV();
	}
	return false;
}

bool BPMFactory::ismonitoringData(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoringData();
	}
	return false;
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

bool BPMFactory::reCalAttenuatiOn(const std::string& name, const double& charge)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call BPMFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.reCalAttenuatiOn(charge);
	}
	return false;
}

std::map<std::string, double> BPMFactory::getXs(const std::vector<std::string>& names)
{
	std::map<std::string, double> xpositiOns;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getX();
		xpositiOns[name] = x;
	}
	return xpositiOns;
}

std::map<std::string, double> BPMFactory::getYs(const std::vector<std::string>& names)
{
	std::map<std::string, double> ypositiOns;
	for (auto name : names)
	{
		double y = bpmMap.find(name)->second.getY();
		ypositiOns[name] = y;
	}
	return ypositiOns;
}

std::map<std::string, double> BPMFactory::getXsFromPV(const std::vector<std::string>& names)
{
	std::map<std::string, double> xpositiOns;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getXFromPV();
		xpositiOns[name] = x;
	}
	return xpositiOns;
}

std::map<std::string, double> BPMFactory::getYsFromPV(const std::vector<std::string>& names)
{
	std::map<std::string, double> ypositiOns;
	for (auto name : names)
	{
		double y = bpmMap.find(name)->second.getYFromPV();
		ypositiOns[name] = y;
	}
	return ypositiOns;
}

std::map<std::string, double> BPMFactory::getQs(const std::vector<std::string>& names)
{
	std::map<std::string, double> qmap;
	for (auto name : names)
	{
		double q = bpmMap.find(name)->second.getQ();
		qmap[name] = q;
	}
	return qmap;
}

std::map<std::string, std::vector< double > > BPMFactory::getDatas(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > datamap;
	for (auto name : names)
	{
		std::vector< double > data = bpmMap.find(name)->second.getData();
		datamap[name] = data;
	}
	return datamap;
}

std::map<std::string, std::pair<double, double>> BPMFactory::getXYPositiOns(const std::vector<std::string>& names)
{
	std::map<std::string, std::pair<double, double>> bpmsAndPositiOnsMap;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getXFromPV();
		double y = bpmMap.find(name)->second.getXFromPV();
		std::pair<double, double> positiOnsPair = std::make_pair(x, y);
		std::pair<std::string, std::pair<double, double>> nameAndPositiOnsPair = std::make_pair(bpmMap.find(name)->first, positiOnsPair);
		bpmsAndPositiOnsMap.insert(nameAndPositiOnsPair);
	}
	return bpmsAndPositiOnsMap;
}

std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > BPMFactory::getXYPositiOnVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > bpmsAndPositiOnsMap;
	for (auto name : names)
	{
		std::vector< double > x = bpmMap.find(name)->second.getXPVVector();
		std::vector< double > y = bpmMap.find(name)->second.getYPVVector();
		std::pair<std::vector< double >, std::vector< double > > positiOnsPair = std::make_pair(x, y);
		std::pair<std::string, std::pair<std::vector< double >, std::vector< double > > > nameAndPositiOnsPair = std::make_pair(bpmMap.find(name)->first, positiOnsPair);
		bpmsAndPositiOnsMap.insert(nameAndPositiOnsPair);
	}
	return bpmsAndPositiOnsMap;
}

std::map<std::string, double> BPMFactory::getPositiOns(const std::vector<std::string>& names)
{
	std::map<std::string, double> positiOnmap;
	for (auto name : names)
	{
		double positiOn = bpmMap.find(name)->second.getPositiOn();
		positiOnmap[name] = positiOn;
	}
	return positiOnmap;
}

std::map<std::string, double> BPMFactory::getResolutiOns(const std::vector<std::string>& names)
{
	std::map<std::string, double> resolutiOnmap;
	for (auto name : names)
	{
		double resolutiOn = bpmMap.find(name)->second.getResolutiOn();
		resolutiOnmap[name] = resolutiOn;
	}
	return resolutiOnmap;
}

std::map<std::string, std::vector< double > > BPMFactory::getXPVVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > xpvmap;
	for (auto name : names)
	{
		std::vector< double > xpv = bpmMap.find(name)->second.getXPVVector();
		xpvmap[name] = xpv;
	}
	return xpvmap;
}

std::map<std::string, std::vector< double > > BPMFactory::getYPVVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > ypvmap;
	for (auto name : names)
	{
		std::vector< double > ypv = bpmMap.find(name)->second.getYPVVector();
		ypvmap[name] = ypv;
	}
	return ypvmap;
}

std::map<std::string, std::vector< double > > BPMFactory::getQVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > qmap;
	for (auto name : names)
	{
		std::vector< double > q = bpmMap.find(name)->second.getQVector();
		qmap[name] = q;
	}
	return qmap;
}

std::map<std::string, std::vector< std::vector< double > > > BPMFactory::getDataVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< std::vector< double > > > datamap;
	for (auto name : names)
	{
		std::vector< std::vector< double > > datapv = bpmMap.find(name)->second.getDataVector();
		datamap[name] = datapv;
	}
	return datamap;
}

std::map<std::string, boost::circular_buffer< double > > BPMFactory::getXPVBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< double > > xpvmap;
	for (auto name : names)
	{
		boost::circular_buffer< double > xpv = bpmMap.find(name)->second.getXPVBuffer();
		xpvmap[name] = xpv;
	}
	return xpvmap;
}

std::map<std::string, boost::circular_buffer< double > > BPMFactory::getYPVBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< double > > ypvmap;
	for (auto name : names)
	{
		boost::circular_buffer< double > ypv = bpmMap.find(name)->second.getYPVBuffer();
		ypvmap[name] = ypv;
	}
	return ypvmap;
}

std::map<std::string, boost::circular_buffer< double > > BPMFactory::getQBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< double > > qmap;
	for (auto name : names)
	{
		boost::circular_buffer< double > q = bpmMap.find(name)->second.getQBuffer();
		qmap[name] = q;
	}
	return qmap;
}

std::map<std::string, boost::circular_buffer< std::vector< double > > > BPMFactory::getDataBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > datamap;
	for (auto name : names)
	{
		boost::circular_buffer< std::vector< double > > data = bpmMap.find(name)->second.getDataBuffer();
		datamap[name] = data;
	}
	return datamap;
}

std::map<std::string, bool> BPMFactory::reCalAttenuatiOns(const std::vector<std::string>& names, const double& charge)
{
	std::map<std::string, bool> recalmap;
	for (auto name : names)
	{
		bool recal = bpmMap.find(name)->second.reCalAttenuatiOn(charge);
		recalmap[name] = recal;
	}
	return recalmap;
}

std::map<std::string, double> BPMFactory::getAllX()
{
	std::map<std::string, double> bpmsAndXMap;
	for (auto bpm : bpmMap)
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

std::map<std::string, std::vector< double > > BPMFactory::getAllData()
{
	std::map<std::string, std::vector< double > > bpmsAndDataMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndDataPair = std::make_pair(bpm.first, bpm.second.getData());
		bpmsAndDataMap.insert(nameAndDataPair);
	}
	return bpmsAndDataMap;
}

std::map<std::string, std::pair<double, double>> BPMFactory::getAllXYPositiOn()
{
	std::map<std::string, std::pair<double, double>> bpmsAndPositiOnsMap;
	for (auto bpm : bpmMap)
	{
		std::pair<double, double> positiOnsPair = std::make_pair(bpm.second.getXFromPV(), bpm.second.getYFromPV());
		std::pair<std::string, std::pair<double, double>> nameAndPositiOnsPair = std::make_pair(bpm.first, positiOnsPair);
		bpmsAndPositiOnsMap.insert(nameAndPositiOnsPair);
	}
	return bpmsAndPositiOnsMap;
}

std::map<std::string, bool> BPMFactory::reCalAllAttenuatiOn(const double& charge)
{
	std::map<std::string, bool> bpmsAndReCalMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndReCalPair = std::make_pair(bpm.first, bpm.second.reCalAttenuatiOn(charge));
		bpmsAndReCalMap.insert(nameAndReCalPair);
	}
	return bpmsAndReCalMap;
}

std::map<std::string, double> BPMFactory::getAllPositiOn()
{
	std::map<std::string, double> bpmsAndPositiOnMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndPositiOnPair = std::make_pair(bpm.first, bpm.second.getPositiOn());
		bpmsAndPositiOnMap.insert(nameAndPositiOnPair);
	}
	return bpmsAndPositiOnMap;
}

std::map<std::string, double> BPMFactory::getAllResolutiOn()
{
	std::map<std::string, double> bpmsAndResolutiOnMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndResolutiOnPair = std::make_pair(bpm.first, bpm.second.getResolutiOn());
		bpmsAndResolutiOnMap.insert(nameAndResolutiOnPair);
	}
	return bpmsAndResolutiOnMap;
}

std::map<std::string, std::vector< double > > BPMFactory::getAllXPVVector()
{
	std::map<std::string, std::vector< double > > bpmsAndXPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndXPVPair = std::make_pair(bpm.first, bpm.second.getXPVVector());
		bpmsAndXPVMap.insert(nameAndXPVPair);
	}
	return bpmsAndXPVMap;
}

std::map<std::string, std::vector< double > > BPMFactory::getAllYPVVector()
{
	std::map<std::string, std::vector< double > > bpmsAndYPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndYPVPair = std::make_pair(bpm.first, bpm.second.getYPVVector());
		bpmsAndYPVMap.insert(nameAndYPVPair);
	}
	return bpmsAndYPVMap;
}

std::map<std::string, std::vector< double > > BPMFactory::getAllQVector()
{
	std::map<std::string, std::vector< double > > bpmsAndQMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndQPair = std::make_pair(bpm.first, bpm.second.getQVector());
		bpmsAndQMap.insert(nameAndQPair);
	}
	return bpmsAndQMap;
}

std::map<std::string, std::vector< std::vector< double > > > BPMFactory::getAllDataVector()
{
	std::map<std::string, std::vector< std::vector< double > > > bpmsAndDataMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< std::vector< double > > > nameAndDataPair = std::make_pair(bpm.first, bpm.second.getDataVector());
		bpmsAndDataMap.insert(nameAndDataPair);
	}
	return bpmsAndDataMap;
}

std::map<std::string, boost::circular_buffer< double > > BPMFactory::getAllXPVBuffer()
{
	std::map<std::string, boost::circular_buffer< double > > bpmsAndXPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< double > > nameAndXPVPair = std::make_pair(bpm.first, bpm.second.getXPVBuffer());
		bpmsAndXPVMap.insert(nameAndXPVPair);
	}
	return bpmsAndXPVMap;
}

std::map<std::string, boost::circular_buffer< double > > BPMFactory::getAllYPVBuffer()
{
	std::map<std::string, boost::circular_buffer< double > > bpmsAndYPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< double > > nameAndYPVPair = std::make_pair(bpm.first, bpm.second.getYPVBuffer());
		bpmsAndYPVMap.insert(nameAndYPVPair);
	}
	return bpmsAndYPVMap;
}

std::map<std::string, boost::circular_buffer< double > > BPMFactory::getAllQBuffer()
{
	std::map<std::string, boost::circular_buffer< double > > bpmsAndQMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< double > > nameAndQPair = std::make_pair(bpm.first, bpm.second.getQBuffer());
		bpmsAndQMap.insert(nameAndQPair);
	}
	return bpmsAndQMap;
}

std::map<std::string, boost::circular_buffer< std::vector< double > > > BPMFactory::getAllDataBuffer()
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > bpmsAndDataMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< std::vector< double > > > nameAndDataPair = std::make_pair(bpm.first, bpm.second.getDataBuffer());
		bpmsAndDataMap.insert(nameAndDataPair);
	}
	return bpmsAndDataMap;
}

std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > BPMFactory::getAllXYPositiOnVectors()
{
	std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > bpmsAndPositiOnsMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::vector< double >, std::vector< double > > positiOnsPair = std::make_pair(bpm.second.getXPVVector(), bpm.second.getYPVVector());
		std::pair<std::string, std::pair<std::vector< double >, std::vector< double > > > nameAndPositiOnsPair = std::make_pair(bpm.first, positiOnsPair);
		bpmsAndPositiOnsMap.insert(nameAndPositiOnsPair);
	}
	return bpmsAndPositiOnsMap;
}

boost::python::list BPMFactory::getData_Py(const std::string& bpmName)
{
	std::vector< double > data;
	data = getData(bpmName);
	boost::python::list newPyList = to_py_list(data);
	return newPyList;
}

boost::python::list BPMFactory::getXPVVector_Py(const std::string& bpmName)
{
	std::vector< double > xpvvec;
	xpvvec = getXPVVector(bpmName);
	boost::python::list newPyList = to_py_list(xpvvec);
	return newPyList;
}

boost::python::list BPMFactory::getYPVVector_Py(const std::string& bpmName)
{
	std::vector< double > ypvvec;
	ypvvec = getYPVVector(bpmName);
	boost::python::list newPyList = to_py_list(ypvvec);
	return newPyList;
}

boost::python::list BPMFactory::getQVector_Py(const std::string& bpmName)
{
	std::vector< double > qvec;
	qvec = getQVector(bpmName);
	boost::python::list newPyList = to_py_list(qvec);
	return newPyList;
}

boost::python::list BPMFactory::getDataVector_Py(const std::string& bpmName)
{
	std::vector< std::vector< double > > data;
	data = getDataVector(bpmName);
	boost::python::list newPyList = to_py_list(data);
	return newPyList;
}

boost::python::list BPMFactory::getXPVBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< double > xbuf;
	xbuf = getXPVBuffer(bpmName);
	boost::python::list newPyList = to_py_list(xbuf);
	return newPyList;
}

boost::python::list BPMFactory::getYPVBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< double > ybuf;
	ybuf = getQBuffer(bpmName);
	boost::python::list newPyList = to_py_list(ybuf);
	return newPyList;
}

boost::python::list BPMFactory::getQBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< double > qbuf;
	qbuf = getQBuffer(bpmName);
	boost::python::list newPyList = to_py_list(qbuf);
	return newPyList;
}

boost::python::list BPMFactory::getDataBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< std::vector< double > > databuf;
	databuf = getDataBuffer(bpmName);
	boost::python::list newPyList = to_py_list(databuf);
	return newPyList;
}

//boost::python::list BPMFactory::getXYPositiOnVector_Py(const std::string bpmName)
//{
//	std::pair< std::vector< double >, std::vector< double > > data;
//	data = getXYPositiOnVector(bpmName);
//	boost::python::list newPyDict = to_py_list(data);
//	return newPyDict;
//}

boost::python::dict BPMFactory::getXs_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> xvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xvals = getXs(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getXsFromPV_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> xvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xvals = getXsFromPV(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getYs_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> yvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	yvals = getYs(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getYsFromPV_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> yvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	yvals = getYsFromPV(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getQs_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> qvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	qvals = getQs(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getXYPositiOns_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::pair<double, double>> xyvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xyvals = getXYPositiOns(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getXPVVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< double > > xpvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xpvvals = getXPVVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xpvvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getYPVVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< double > > ypvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	ypvvals = getYPVVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(ypvvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getQVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< double > > qvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	qvals = getQVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getDataVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< std::vector< double > > > datavals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	datavals = getDataVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict BPMFactory::getXPVBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< double > > xpvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xpvvals = getXPVBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xpvvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getYPVBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< double > > ypvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	ypvvals = getYPVBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(ypvvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getQBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< double > > qvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	qvals = getQBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getDataBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > datavals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	datavals = getDataBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict BPMFactory::getXYPositiOnVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::pair< std::vector< double >, std::vector< double > > > xyvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xyvals = getXYPositiOnVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict BPMFactory::reCalAttenuatiOns_Py(boost::python::list bpmNames, const double& charge)
{
	std::map<std::string, bool> recalvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	recalvals = reCalAttenuatiOns(bpmNamesVector, charge);
	boost::python::dict newPyDict = to_py_dict(recalvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getResolutiOns_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> resolutiOnvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	resolutiOnvals = getResolutiOns(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(resolutiOnvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getPositiOns_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> positiOnvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	positiOnvals = getResolutiOns(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(positiOnvals);
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

boost::python::dict BPMFactory::getAllData_Py()
{
	std::map<std::string, std::vector< double > > datavals = getAllData();
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllXPVVector_Py()
{
	std::map<std::string, std::vector< double > > xvals = getAllXPVVector();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllYPVVector_Py()
{
	std::map<std::string, std::vector< double > > yvals = getAllYPVVector();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllQVector_Py()
{
	std::map<std::string, std::vector< double > > qvals = getAllQVector();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllDataVector_Py()
{
	std::map<std::string, std::vector< std::vector< double > > > datavals = getAllDataVector();
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllXPVBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< double > > xvals = getAllXPVBuffer();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllYPVBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< double > > yvals = getAllYPVBuffer();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllQBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< double > > qvals = getAllQBuffer();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllDataBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > datavals = getAllDataBuffer();
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllXYPositiOn_Py()
{
	std::map<std::string, std::pair<double, double>> xyvals = getAllXYPositiOn();
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllXYPositiOnVectors_Py()
{
	std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > xyvals = getAllXYPositiOnVectors();
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllResolutiOn_Py()
{
	std::map<std::string, double> resolutiOnvals = getAllResolutiOn();
	boost::python::dict newPyDict = to_py_dict(resolutiOnvals);
	return newPyDict;
}

boost::python::dict BPMFactory::getAllPositiOn_Py()
{
	std::map<std::string, double> positiOnvals = getAllPositiOn();
	boost::python::dict newPyDict = to_py_dict(positiOnvals);
	return newPyDict;
}

boost::python::dict BPMFactory::reCalAllAttenuatiOn_Py(const double& charge)
{
	std::map<std::string, bool> recalvals = reCalAllAttenuatiOn(charge);
	boost::python::dict newPyDict = to_py_dict(recalvals);
	return newPyDict;
}

void BPMFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("BPM-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& bpm : bpmMap)
	{
		bpm.second.debugMessagesOn();
	}
}
void BPMFactory::debugMessagesOff()
{
	messenger.printDebugMessage("BPM-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& bpm : bpmMap)
	{
		bpm.second.debugMessagesOff();
	}
}
void BPMFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("BPM-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& bpm : bpmMap)
	{
		bpm.second.messagesOn();
	}
}
void BPMFactory::messagesOff()
{
	messenger.printMessage("BPM-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& bpm : bpmMap)
	{
		bpm.second.messagesOff();
	}
}
bool BPMFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool BPMFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}