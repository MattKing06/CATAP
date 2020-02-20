#include "ScreenFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <utility>
#include "GlobalFunctions.h"
#include "GlobalConstants.h"
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif
#include "yaml-cpp/emitter.h"

ScreenFactory::ScreenFactory() : ScreenFactory(STATE::OFFLINE)
{
	std::cout << "ScreenFactory DEFAULT constRUCTOR called " << std::endl;
}
ScreenFactory::ScreenFactory(STATE mode):
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Screen", mode))
{
	messenger = LoggingSystem(true, true);
	//hasBeenSetup = false;
//	messenger = LoggingSystem(false, false);
	messenger.printDebugMessage("Screen Factory constructed");
	//mode = mode;
	//reader = ConfigReader("Screen", mode);
}
ScreenFactory::ScreenFactory(const ScreenFactory& copyScreenFactory)
	: hasBeenSetup(copyScreenFactory.hasBeenSetup),
	mode(copyScreenFactory.mode),
	messenger(copyScreenFactory.messenger),
	reader(copyScreenFactory.reader)
{
	messenger.printDebugMessage("ScreenFactory Copy cOntructor");
	bpmMap.insert(copyScreenFactory.bpmMap.begin(), copyScreenFactory.bpmMap.end());
}

ScreenFactory::~ScreenFactory()
{
	messenger.printDebugMessage("ScreenFactory Destructor Called");
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

void ScreenFactory::populateScreenMap()
{
	messenger.printDebugMessage("ScreenFactory is populating the bpm map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive cOnfiguratiOn parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Screen Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(bpmMap);
	}
	messenger.printDebugMessage("ScreenFactory has finished populating the bpm map");
}

void ScreenFactory::retrievemonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == ScreenRecords::X || 
		pvStruct.pvRecord == ScreenRecords::Y || 
		pvStruct.pvRecord == ScreenRecords::SA1 ||
		pvStruct.pvRecord == ScreenRecords::SA2 ||
		pvStruct.pvRecord == ScreenRecords::SD1 ||
		pvStruct.pvRecord == ScreenRecords::SD2 ||
		pvStruct.pvRecord == ScreenRecords::RA1 ||
		pvStruct.pvRecord == ScreenRecords::RA2 ||
		pvStruct.pvRecord == ScreenRecords::RD1 ||
		pvStruct.pvRecord == ScreenRecords::RD2 ||
		pvStruct.pvRecord == ScreenRecords::DATA ||
		pvStruct.pvRecord == ScreenRecords::AWAK ||
		pvStruct.pvRecord == ScreenRecords::RDY
		)
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
}

void ScreenFactory::setupChannels()
{
	for (auto& bpm : bpmMap)
	{
		std::map<std::string, pvStruct>& pvStructs = bpm.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			bpm.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

bool ScreenFactory::setup(const std::string& VERSION)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in Screen constructor
	//// but we have a lot of PV informatiOn to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateScreenMap();

	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto& bpm : bpmMap)
	{
		std::map<std::string, pvStruct>& bpmPVStructs = bpm.second.getPVStructs();
		for (auto& pv : bpmPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrievemonitorStatus(pv.second);
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
				EPICSInterface::sendToEPICS();
			}
			else
			{
				messenger.printMessage(bpm.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
	std::cout << "end" << std::endl;
}

std::map<std::string, Screen> ScreenFactory::getScreens(std::vector<std::string> bpmNames)
{
	std::map<std::string, Screen> selectedScreens;
	for (auto& bpmName : bpmNames)
	{
		selectedScreens[bpmName] = bpmMap.find(bpmName)->second;
	}
	return selectedScreens;
}

Screen& ScreenFactory::getScreen(const std::string& fullScreenName)
{
	return bpmMap.find(fullScreenName)->second;
}

std::map<std::string, Screen> ScreenFactory::getAllScreens()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("ScreenS HAVE ALREADY BEEN constRUCTED.");
	}
	return bpmMap;
}

std::string ScreenFactory::getScreenName(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getScreenName();
	}
	return "0";
}

void ScreenFactory::monitorForNShots(const std::string& name, const size_t& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		bpmMap.find(name)->second.monitorForNShots(value);
	}
}

double ScreenFactory::getX(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getX();
	}
	return std::numeric_limits<double>::min();;
}

double ScreenFactory::getXFromPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getXFromPV();
	}
	return std::numeric_limits<double>::min();;
}

double ScreenFactory::getY(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getY();
	}
	return std::numeric_limits<double>::min();;
}

double ScreenFactory::getYFromPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getYFromPV();
	}
	return std::numeric_limits<double>::min();;
}

STATE ScreenFactory::getStatus(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getStatus();
	}
	return STATE::UNKNOWN;
}

boost::circular_buffer< STATE > ScreenFactory::getStatusBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getStatusBuffer();
	}
	boost::circular_buffer< STATE> statebuffer;
	statebuffer.push_back(STATE::UNKNOWN);
	return statebuffer;
}

std::vector< STATE > ScreenFactory::getStatusVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getStatusVector();
	}
	std::vector< STATE> statevector;
	statevector.push_back(STATE::UNKNOWN);
	return statevector;
}

std::vector< double > ScreenFactory::getData(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getData();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::pair<double, double> ScreenFactory::getXYPosition(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		double x = bpmMap.find(name)->second.getYFromPV();
		double y = bpmMap.find(name)->second.getYFromPV();
		return std::make_pair(x, y);
	}
	return std::make_pair(std::numeric_limits<double>::min(), std::numeric_limits<double>::min());
}

double ScreenFactory::getQ(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getQ();
	}
	return std::numeric_limits<double>::min();;
}

double ScreenFactory::getPosition(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getPosition();
	}
	return std::numeric_limits<double>::min();;
}

double ScreenFactory::getResolution(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getResolution();
	}
	return std::numeric_limits<double>::min();;
}

std::vector< double > ScreenFactory::getXPVVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getXPVVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::vector< double > ScreenFactory::getYPVVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getYPVVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::vector< double > ScreenFactory::getQVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getQVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

std::vector< std::vector< double > > ScreenFactory::getDataVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getDataVector();
	}
	std::vector< double > vector2(9, std::numeric_limits<double>::min());
	std::vector< std::vector< double > > vector3(9, vector2);
	return vector3;
}

boost::circular_buffer< double > ScreenFactory::getXPVBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getXPVBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< double > ScreenFactory::getYPVBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getYPVBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< double > ScreenFactory::getQBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getQBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< std::vector< double > > ScreenFactory::getDataBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getDataBuffer();
	}
	std::vector< double > vector2(9, std::numeric_limits<double>::min());
	boost::circular_buffer< std::vector< double > > vector3(9, vector2);
	return vector3;
}

bool ScreenFactory::ismonitoringXPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoringXPV();
	}
	return false;
}

bool ScreenFactory::ismonitoring(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoring();
	}
	return false;
}

bool ScreenFactory::ismonitoringYPV(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoringYPV();
	}
	return false;
}

bool ScreenFactory::ismonitoringData(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.ismonitoringData();
	}
	return false;
}

long ScreenFactory::getSA1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSA1();
	}
	return std::numeric_limits<long>::min();;
}

long ScreenFactory::getSA2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSA2();
	}
	return std::numeric_limits<long>::min();;
}

long ScreenFactory::getSD1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSD1();
	}
	return std::numeric_limits<long>::min();;
}

long ScreenFactory::getSD2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getSD2();
	}
	return std::numeric_limits<long>::min();;
}

bool ScreenFactory::setSA1(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSA1(value);
	}
	return false;
}

bool ScreenFactory::setSA2(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSA2(value);
	}
	return false;
}

bool ScreenFactory::setSD1(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSD1(value);
	}
	return false;
}

bool ScreenFactory::setSD2(const std::string& name, const long& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.setSD2(value);
	}
	return false;
}

long ScreenFactory::getRA1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRA1();
	}
	return std::numeric_limits<long>::min();;
}

long ScreenFactory::getRA2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRA2();
	}
	return std::numeric_limits<long>::min();;
}

long ScreenFactory::getRD1(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRD1();
	}
	return std::numeric_limits<long>::min();;
}

long ScreenFactory::getRD2(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.getRD2();
	}
	return std::numeric_limits<long>::min();;
}

bool ScreenFactory::reCalAttenuation(const std::string& name, const double& charge)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return bpmMap.find(name)->second.reCalAttenuation(charge);
	}
	return false;
}

std::map<std::string, double> ScreenFactory::getXs(const std::vector<std::string>& names)
{
	std::map<std::string, double> xpositiOns;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getX();
		xpositiOns[name] = x;
	}
	return xpositiOns;
}

std::map<std::string, double> ScreenFactory::getYs(const std::vector<std::string>& names)
{
	std::map<std::string, double> ypositiOns;
	for (auto name : names)
	{
		double y = bpmMap.find(name)->second.getY();
		ypositiOns[name] = y;
	}
	return ypositiOns;
}

std::map<std::string, double> ScreenFactory::getXsFromPV(const std::vector<std::string>& names)
{
	std::map<std::string, double> xpositiOns;
	for (auto name : names)
	{
		double x = bpmMap.find(name)->second.getXFromPV();
		xpositiOns[name] = x;
	}
	return xpositiOns;
}

std::map<std::string, double> ScreenFactory::getYsFromPV(const std::vector<std::string>& names)
{
	std::map<std::string, double> ypositiOns;
	for (auto name : names)
	{
		double y = bpmMap.find(name)->second.getYFromPV();
		ypositiOns[name] = y;
	}
	return ypositiOns;
}

std::map<std::string, double> ScreenFactory::getQs(const std::vector<std::string>& names)
{
	std::map<std::string, double> qmap;
	for (auto name : names)
	{
		double q = bpmMap.find(name)->second.getQ();
		qmap[name] = q;
	}
	return qmap;
}

std::map<std::string, STATE> ScreenFactory::getStatuses(const std::vector<std::string>& names)
{
	std::map<std::string, STATE> statusmap;
	for (auto name : names)
	{
		STATE state = bpmMap.find(name)->second.getStatus();
		statusmap[name] = state;
	}
	return statusmap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getDatas(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > datamap;
	for (auto name : names)
	{
		std::vector< double > data = bpmMap.find(name)->second.getData();
		datamap[name] = data;
	}
	return datamap;
}

std::map<std::string, std::pair<double, double>> ScreenFactory::getXYPositions(const std::vector<std::string>& names)
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

std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > ScreenFactory::getXYPositionVectors(const std::vector<std::string>& names)
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

std::map<std::string, double> ScreenFactory::getPositions(const std::vector<std::string>& names)
{
	std::map<std::string, double> positiOnmap;
	for (auto name : names)
	{
		double positiOn = bpmMap.find(name)->second.getPosition();
		positiOnmap[name] = positiOn;
	}
	return positiOnmap;
}

std::map<std::string, double> ScreenFactory::getResolutions(const std::vector<std::string>& names)
{
	std::map<std::string, double> resolutiOnmap;
	for (auto name : names)
	{
		double resolutiOn = bpmMap.find(name)->second.getResolution();
		resolutiOnmap[name] = resolutiOn;
	}
	return resolutiOnmap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getXPVVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > xpvmap;
	for (auto name : names)
	{
		std::vector< double > xpv = bpmMap.find(name)->second.getXPVVector();
		xpvmap[name] = xpv;
	}
	return xpvmap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getYPVVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > ypvmap;
	for (auto name : names)
	{
		std::vector< double > ypv = bpmMap.find(name)->second.getYPVVector();
		ypvmap[name] = ypv;
	}
	return ypvmap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getQVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > qmap;
	for (auto name : names)
	{
		std::vector< double > q = bpmMap.find(name)->second.getQVector();
		qmap[name] = q;
	}
	return qmap;
}

std::map<std::string, std::vector< STATE > > ScreenFactory::getStatusVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< STATE > > statusmap;
	for (auto name : names)
	{
		std::vector< STATE > statepv = bpmMap.find(name)->second.getStatusVector();
		statusmap[name] = statepv;
	}
	return statusmap;
}

std::map<std::string, std::vector< std::vector< double > > > ScreenFactory::getDataVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< std::vector< double > > > datamap;
	for (auto name : names)
	{
		std::vector< std::vector< double > > datapv = bpmMap.find(name)->second.getDataVector();
		datamap[name] = datapv;
	}
	return datamap;
}

std::map<std::string, boost::circular_buffer< double > > ScreenFactory::getXPVBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< double > > xpvmap;
	for (auto name : names)
	{
		boost::circular_buffer< double > xpv = bpmMap.find(name)->second.getXPVBuffer();
		xpvmap[name] = xpv;
	}
	return xpvmap;
}

std::map<std::string, boost::circular_buffer< double > > ScreenFactory::getYPVBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< double > > ypvmap;
	for (auto name : names)
	{
		boost::circular_buffer< double > ypv = bpmMap.find(name)->second.getYPVBuffer();
		ypvmap[name] = ypv;
	}
	return ypvmap;
}

std::map<std::string, boost::circular_buffer< double > > ScreenFactory::getQBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< double > > qmap;
	for (auto name : names)
	{
		boost::circular_buffer< double > q = bpmMap.find(name)->second.getQBuffer();
		qmap[name] = q;
	}
	return qmap;
}

std::map<std::string, boost::circular_buffer< STATE > > ScreenFactory::getStatusBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< STATE > > statusmap;
	for (auto name : names)
	{
		boost::circular_buffer< STATE > state = bpmMap.find(name)->second.getStatusBuffer();
		statusmap[name] = state;
	}
	return statusmap;
}

std::map<std::string, boost::circular_buffer< std::vector< double > > > ScreenFactory::getDataBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > datamap;
	for (auto name : names)
	{
		boost::circular_buffer< std::vector< double > > data = bpmMap.find(name)->second.getDataBuffer();
		datamap[name] = data;
	}
	return datamap;
}

std::map<std::string, bool> ScreenFactory::reCalAttenuations(const std::vector<std::string>& names, const double& charge)
{
	std::map<std::string, bool> recalmap;
	for (auto name : names)
	{
		bool recal = bpmMap.find(name)->second.reCalAttenuation(charge);
		recalmap[name] = recal;
	}
	return recalmap;
}

std::map<std::string, double> ScreenFactory::getAllX()
{
	std::map<std::string, double> bpmsAndXMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndXPair = std::make_pair(bpm.first, bpm.second.getX());
		bpmsAndXMap.insert(nameAndXPair);
	}
	return bpmsAndXMap;
}

std::map<std::string, double> ScreenFactory::getAllXFromPV()
{
	std::map<std::string, double> bpmsAndXMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndXPair = std::make_pair(bpm.first, bpm.second.getXFromPV());
		bpmsAndXMap.insert(nameAndXPair);
	}
	return bpmsAndXMap;
}

std::map<std::string, double> ScreenFactory::getAllY()
{
	std::map<std::string, double> bpmsAndYMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndYPair = std::make_pair(bpm.first, bpm.second.getY());
		bpmsAndYMap.insert(nameAndYPair);
	}
	return bpmsAndYMap;
}

std::map<std::string, double> ScreenFactory::getAllYFromPV()
{
	std::map<std::string, double> bpmsAndYMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndYPair = std::make_pair(bpm.first, bpm.second.getYFromPV());
		bpmsAndYMap.insert(nameAndYPair);
	}
	return bpmsAndYMap;
}

std::map<std::string, double> ScreenFactory::getAllQ()
{
	std::map<std::string, double> bpmsAndQMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndQPair = std::make_pair(bpm.first, bpm.second.getQ());
		bpmsAndQMap.insert(nameAndQPair);
	}
	return bpmsAndQMap;
}

std::map<std::string, STATE> ScreenFactory::getAllStatus()
{
	std::map<std::string, STATE> bpmsAndStatusMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, STATE> nameAndStatusPair = std::make_pair(bpm.first, bpm.second.getStatus());
		bpmsAndStatusMap.insert(nameAndStatusPair);
	}
	return bpmsAndStatusMap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getAllData()
{
	std::map<std::string, std::vector< double > > bpmsAndDataMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndDataPair = std::make_pair(bpm.first, bpm.second.getData());
		bpmsAndDataMap.insert(nameAndDataPair);
	}
	return bpmsAndDataMap;
}

std::map<std::string, std::pair<double, double>> ScreenFactory::getAllXYPosition()
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

std::map<std::string, bool> ScreenFactory::reCalAllAttenuation(const double& charge)
{
	std::map<std::string, bool> bpmsAndReCalMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndReCalPair = std::make_pair(bpm.first, bpm.second.reCalAttenuation(charge));
		bpmsAndReCalMap.insert(nameAndReCalPair);
	}
	return bpmsAndReCalMap;
}

std::map<std::string, double> ScreenFactory::getAllPosition()
{
	std::map<std::string, double> bpmsAndPositiOnMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndPositiOnPair = std::make_pair(bpm.first, bpm.second.getPosition());
		bpmsAndPositiOnMap.insert(nameAndPositiOnPair);
	}
	return bpmsAndPositiOnMap;
}

std::map<std::string, double> ScreenFactory::getAllResolution()
{
	std::map<std::string, double> bpmsAndResolutiOnMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, double> nameAndResolutiOnPair = std::make_pair(bpm.first, bpm.second.getResolution());
		bpmsAndResolutiOnMap.insert(nameAndResolutiOnPair);
	}
	return bpmsAndResolutiOnMap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getAllXPVVector()
{
	std::map<std::string, std::vector< double > > bpmsAndXPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndXPVPair = std::make_pair(bpm.first, bpm.second.getXPVVector());
		bpmsAndXPVMap.insert(nameAndXPVPair);
	}
	return bpmsAndXPVMap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getAllYPVVector()
{
	std::map<std::string, std::vector< double > > bpmsAndYPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndYPVPair = std::make_pair(bpm.first, bpm.second.getYPVVector());
		bpmsAndYPVMap.insert(nameAndYPVPair);
	}
	return bpmsAndYPVMap;
}

std::map<std::string, std::vector< double > > ScreenFactory::getAllQVector()
{
	std::map<std::string, std::vector< double > > bpmsAndQMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< double > > nameAndQPair = std::make_pair(bpm.first, bpm.second.getQVector());
		bpmsAndQMap.insert(nameAndQPair);
	}
	return bpmsAndQMap;
}

std::map<std::string, std::vector< STATE > > ScreenFactory::getAllStatusVector()
{
	std::map<std::string, std::vector< STATE > > bpmsAndStatusMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< STATE > > nameAndStatusPair = std::make_pair(bpm.first, bpm.second.getStatusVector());
		bpmsAndStatusMap.insert(nameAndStatusPair);
	}
	return bpmsAndStatusMap;
}

std::map<std::string, std::vector< std::vector< double > > > ScreenFactory::getAllDataVector()
{
	std::map<std::string, std::vector< std::vector< double > > > bpmsAndDataMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, std::vector< std::vector< double > > > nameAndDataPair = std::make_pair(bpm.first, bpm.second.getDataVector());
		bpmsAndDataMap.insert(nameAndDataPair);
	}
	return bpmsAndDataMap;
}

std::map<std::string, boost::circular_buffer< double > > ScreenFactory::getAllXPVBuffer()
{
	std::map<std::string, boost::circular_buffer< double > > bpmsAndXPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< double > > nameAndXPVPair = std::make_pair(bpm.first, bpm.second.getXPVBuffer());
		bpmsAndXPVMap.insert(nameAndXPVPair);
	}
	return bpmsAndXPVMap;
}

std::map<std::string, boost::circular_buffer< double > > ScreenFactory::getAllYPVBuffer()
{
	std::map<std::string, boost::circular_buffer< double > > bpmsAndYPVMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< double > > nameAndYPVPair = std::make_pair(bpm.first, bpm.second.getYPVBuffer());
		bpmsAndYPVMap.insert(nameAndYPVPair);
	}
	return bpmsAndYPVMap;
}

std::map<std::string, boost::circular_buffer< double > > ScreenFactory::getAllQBuffer()
{
	std::map<std::string, boost::circular_buffer< double > > bpmsAndQMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< double > > nameAndQPair = std::make_pair(bpm.first, bpm.second.getQBuffer());
		bpmsAndQMap.insert(nameAndQPair);
	}
	return bpmsAndQMap;
}

std::map<std::string, boost::circular_buffer< STATE > > ScreenFactory::getAllStatusBuffer()
{
	std::map<std::string, boost::circular_buffer< STATE > > bpmsAndStatusMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< STATE > > nameAndStatusPair = std::make_pair(bpm.first, bpm.second.getStatusBuffer());
		bpmsAndStatusMap.insert(nameAndStatusPair);
	}
	return bpmsAndStatusMap;
}

std::map<std::string, boost::circular_buffer< std::vector< double > > > ScreenFactory::getAllDataBuffer()
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > bpmsAndDataMap;
	for (auto bpm : bpmMap)
	{
		std::pair<std::string, boost::circular_buffer< std::vector< double > > > nameAndDataPair = std::make_pair(bpm.first, bpm.second.getDataBuffer());
		bpmsAndDataMap.insert(nameAndDataPair);
	}
	return bpmsAndDataMap;
}

std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > ScreenFactory::getAllXYPositionVectors()
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

boost::python::list ScreenFactory::getData_Py(const std::string& bpmName)
{
	std::vector< double > data;
	data = getData(bpmName);
	boost::python::list newPyList = to_py_list(data);
	return newPyList;
}

boost::python::list ScreenFactory::getXPVVector_Py(const std::string& bpmName)
{
	std::vector< double > xpvvec;
	xpvvec = getXPVVector(bpmName);
	boost::python::list newPyList = to_py_list(xpvvec);
	return newPyList;
}

boost::python::list ScreenFactory::getYPVVector_Py(const std::string& bpmName)
{
	std::vector< double > ypvvec;
	ypvvec = getYPVVector(bpmName);
	boost::python::list newPyList = to_py_list(ypvvec);
	return newPyList;
}

boost::python::list ScreenFactory::getQVector_Py(const std::string& bpmName)
{
	std::vector< double > qvec;
	qvec = getQVector(bpmName);
	boost::python::list newPyList = to_py_list(qvec);
	return newPyList;
}

boost::python::list ScreenFactory::getStatusVector_Py(const std::string& bpmName)
{
	std::vector< STATE > statevec;
	statevec = getStatusVector(bpmName);
	boost::python::list newPyList = to_py_list(statevec);
	return newPyList;
}

boost::python::list ScreenFactory::getDataVector_Py(const std::string& bpmName)
{
	std::vector< std::vector< double > > data;
	data = getDataVector(bpmName);
	boost::python::list newPyList = to_py_list(data);
	return newPyList;
}

boost::python::list ScreenFactory::getXPVBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< double > xbuf;
	xbuf = getXPVBuffer(bpmName);
	boost::python::list newPyList = to_py_list(xbuf);
	return newPyList;
}

boost::python::list ScreenFactory::getYPVBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< double > ybuf;
	ybuf = getQBuffer(bpmName);
	boost::python::list newPyList = to_py_list(ybuf);
	return newPyList;
}

boost::python::list ScreenFactory::getQBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< double > qbuf;
	qbuf = getQBuffer(bpmName);
	boost::python::list newPyList = to_py_list(qbuf);
	return newPyList;
}

boost::python::list ScreenFactory::getStatusBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< STATE > statebuf;
	statebuf = getStatusBuffer(bpmName);
	boost::python::list newPyList = to_py_list(statebuf);
	return newPyList;
}

boost::python::list ScreenFactory::getDataBuffer_Py(const std::string& bpmName)
{
	boost::circular_buffer< std::vector< double > > databuf;
	databuf = getDataBuffer(bpmName);
	boost::python::list newPyList = to_py_list(databuf);
	return newPyList;
}

//boost::python::list ScreenFactory::getXYPositiOnVector_Py(const std::string bpmName)
//{
//	std::pair< std::vector< double >, std::vector< double > > data;
//	data = getXYPositiOnVector(bpmName);
//	boost::python::list newPyDict = to_py_list(data);
//	return newPyDict;
//}

boost::python::dict ScreenFactory::getXs_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> xvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xvals = getXs(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getXsFromPV_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> xvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xvals = getXsFromPV(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getYs_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> yvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	yvals = getYs(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getYsFromPV_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> yvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	yvals = getYsFromPV(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getQs_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> qvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	qvals = getQs(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getStatuses_Py(boost::python::list bpmNames)
{
	std::map<std::string, STATE> statevals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	statevals = getStatuses(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(statevals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getXYPositions_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::pair<double, double>> xyvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xyvals = getXYPositions(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getXPVVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< double > > xpvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xpvvals = getXPVVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xpvvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getYPVVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< double > > ypvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	ypvvals = getYPVVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(ypvvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getQVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< double > > qvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	qvals = getQVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getStatusVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< STATE > > statevals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	statevals = getStatusVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(statevals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getDataVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::vector< std::vector< double > > > datavals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	datavals = getDataVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getXPVBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< double > > xpvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xpvvals = getXPVBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xpvvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getYPVBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< double > > ypvvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	ypvvals = getYPVBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(ypvvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getQBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< double > > qvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	qvals = getQBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getStatusBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< STATE > > statevals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	statevals = getStatusBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(statevals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getDataBuffers_Py(boost::python::list bpmNames)
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > datavals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	datavals = getDataBuffers(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getXYPositionVectors_Py(boost::python::list bpmNames)
{
	std::map<std::string, std::pair< std::vector< double >, std::vector< double > > > xyvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	xyvals = getXYPositionVectors(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::reCalAttenuations_Py(boost::python::list bpmNames, const double& charge)
{
	std::map<std::string, bool> recalvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	recalvals = reCalAttenuations(bpmNamesVector, charge);
	boost::python::dict newPyDict = to_py_dict(recalvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getResolutions_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> resolutiOnvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	resolutiOnvals = getResolutions(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(resolutiOnvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getPositions_Py(boost::python::list bpmNames)
{
	std::map<std::string, double> positiOnvals;
	std::vector<std::string> bpmNamesVector = to_std_vector<std::string>(bpmNames);
	positiOnvals = getResolutions(bpmNamesVector);
	boost::python::dict newPyDict = to_py_dict(positiOnvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllX_Py()
{
	std::map<std::string, double> xvals = getAllX();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllXFromPV_Py()
{
	std::map<std::string, double> xvals = getAllXFromPV();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllY_Py()
{
	std::map<std::string, double> yvals = getAllY();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllYFromPV_Py()
{
	std::map<std::string, double> yvals = getAllYFromPV();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllQ_Py()
{
	std::map<std::string, double> qvals = getAllQ();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllStatus_Py()
{
	std::map<std::string, STATE> statevals = getAllStatus();
	boost::python::dict newPyDict = to_py_dict(statevals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllData_Py()
{
	std::map<std::string, std::vector< double > > datavals = getAllData();
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllXPVVector_Py()
{
	std::map<std::string, std::vector< double > > xvals = getAllXPVVector();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllYPVVector_Py()
{
	std::map<std::string, std::vector< double > > yvals = getAllYPVVector();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllQVector_Py()
{
	std::map<std::string, std::vector< double > > qvals = getAllQVector();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllStatusVector_Py()
{
	std::map<std::string, std::vector< STATE > > statevals = getAllStatusVector();
	boost::python::dict newPyDict = to_py_dict(statevals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllDataVector_Py()
{
	std::map<std::string, std::vector< std::vector< double > > > datavals = getAllDataVector();
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllXPVBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< double > > xvals = getAllXPVBuffer();
	boost::python::dict newPyDict = to_py_dict(xvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllYPVBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< double > > yvals = getAllYPVBuffer();
	boost::python::dict newPyDict = to_py_dict(yvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllQBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< double > > qvals = getAllQBuffer();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllStatusBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< STATE > > statevals = getAllStatusBuffer();
	boost::python::dict newPyDict = to_py_dict(statevals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllDataBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< std::vector< double > > > datavals = getAllDataBuffer();
	boost::python::dict newPyDict = to_py_dict(datavals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllXYPosition_Py()
{
	std::map<std::string, std::pair<double, double>> xyvals = getAllXYPosition();
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllXYPositionVectors_Py()
{
	std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > xyvals = getAllXYPositionVectors();
	boost::python::dict newPyDict = to_py_dict(xyvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllResolution_Py()
{
	std::map<std::string, double> resolutiOnvals = getAllResolution();
	boost::python::dict newPyDict = to_py_dict(resolutiOnvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllPosition_Py()
{
	std::map<std::string, double> positiOnvals = getAllPosition();
	boost::python::dict newPyDict = to_py_dict(positiOnvals);
	return newPyDict;
}

boost::python::dict ScreenFactory::reCalAllAttenuation_Py(const double& charge)
{
	std::map<std::string, bool> recalvals = reCalAllAttenuation(charge);
	boost::python::dict newPyDict = to_py_dict(recalvals);
	return newPyDict;
}

void ScreenFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Screen-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& bpm : bpmMap)
	{
		bpm.second.debugMessagesOn();
	}
}
void ScreenFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Screen-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& bpm : bpmMap)
	{
		bpm.second.debugMessagesOff();
	}
}
void ScreenFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Screen-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& bpm : bpmMap)
	{
		bpm.second.messagesOn();
	}
}
void ScreenFactory::messagesOff()
{
	messenger.printMessage("Screen-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& bpm : bpmMap)
	{
		bpm.second.messagesOff();
	}
}
bool ScreenFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool ScreenFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}