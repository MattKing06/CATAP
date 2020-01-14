#include "ChargeFactory.h"
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

ChargeFactory::ChargeFactory() : ChargeFactory(STATE::OFFLINE)
{
}

ChargeFactory::ChargeFactory(STATE mode):
mode(mode),
hasBeenSetup(false),
reader(ConfigReader("Charge", mode))
{
	messenger = LoggingSystem(false, false);
	//hasBeenSetup = false;
	messenger.printDebugMessage("Charge Factory Constructed");
	//mode = mode;
	//reader = ConfigReader("Charge", mode);
}
ChargeFactory::ChargeFactory(const ChargeFactory& copyChargeFactory)
	: hasBeenSetup(copyChargeFactory.hasBeenSetup),
	mode(copyChargeFactory.mode),
	messenger(copyChargeFactory.messenger),
	reader(copyChargeFactory.reader)
{
	chargeMap.insert(copyChargeFactory.chargeMap.begin(), copyChargeFactory.chargeMap.end());
}

ChargeFactory::~ChargeFactory()
{
	messenger.printDebugMessage("ChargeFactory Destructor Called");
	for (auto& charge : chargeMap)
	{
		auto pvStructsList = charge.second.getPVStructs();
		for (auto& pvStruct : pvStructsList)
		{
			if (pvStruct.second.monitor)
			{
				charge.second.epicsInterface->removeSubscription(pvStruct.second);
				ca_flush_io();
			}
			charge.second.epicsInterface->removeChannel(pvStruct.second);
			ca_pend_io(CA_PEND_IO_TIMEOUT);
		}
	}
}

void ChargeFactory::populateChargeMap()
{
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		reader.parseNextYamlFile(chargeMap);
	}
}

void ChargeFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == "Q"
		)
	{
		pvStruct.monitor = true;
	}
}

bool ChargeFactory::setup(const std::string &version)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage(" VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in BPM constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateChargeMap();
	for (auto &charge : chargeMap)
	{
		std::map<std::string, pvStruct>& chargePVStructs = charge.second.getPVStructs();
		for (auto& pv : chargePVStructs)
		{

			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			retrieveMonitorStatus(pv.second);
			charge.second.epicsInterface->retrieveCHID(pv.second);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				charge.second.epicsInterface->retrieveCHID(pv.second);
				charge.second.epicsInterface->retrieveCHTYPE(pv.second);
				charge.second.epicsInterface->retrieveCOUNT(pv.second);
				charge.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.debugMessagesOn();
				messenger.printDebugMessage(pv.second.pvRecord + ": read" + std::to_string(ca_read_access(pv.second.CHID)) +
					"write" + std::to_string(ca_write_access(pv.second.CHID)) +
					"state" + std::to_string(ca_state(pv.second.CHID)) + "\n");
				if (pv.second.monitor)
				{
					charge.second.epicsInterface->createSubscription(charge.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(charge.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

std::map<std::string, Charge> ChargeFactory::getChargeDiagnostics(std::vector<std::string> chargeNames)
{
	std::map<std::string, Charge> selectedCharges;
	for (auto & chargeName : chargeNames)
	{
		selectedCharges[chargeName] = chargeMap.find(chargeName)->second;
	}
	return selectedCharges;
}

Charge& ChargeFactory::getChargeDiagnostic(const std::string& fullChargeName)
{
	return chargeMap.find(fullChargeName)->second;
}

std::map<std::string, Charge> ChargeFactory::getAllChargeDiagnostics()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("CHARGE DIAGNOSTICS HAVE ALREADY BEEN CONSTRUCTED.");
	}
	return chargeMap;
}

std::string ChargeFactory::getChargeDiagnosticName(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		return chargeMap.find(name)->second.getName();
	}
	return "0";
}

void ChargeFactory::monitorForNShots(const std::string& name, const size_t& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		chargeMap.find(name)->second.monitorForNShots(value);
	}
}

void ChargeFactory::monitorMultipleForNShots(const std::vector< std::string >& names, const size_t& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		for (auto&& it : names)
		{
			chargeMap.find(it)->second.monitorForNShots(value);
		}
	}
}

double ChargeFactory::getQ(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		return chargeMap.find(name)->second.getQ();
	}
	return std::numeric_limits<double>::min();;
}

double ChargeFactory::getPosition(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		return chargeMap.find(name)->second.getPosition();
	}
	return std::numeric_limits<double>::min();;
}

std::vector< double > ChargeFactory::getQVector(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		return chargeMap.find(name)->second.getQVector();
	}
	std::vector<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}

boost::circular_buffer< double > ChargeFactory::getQBuffer(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		return chargeMap.find(name)->second.getQBuffer();
	}
	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
	return vector2;
}


bool ChargeFactory::isMonitoring(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		return chargeMap.find(name)->second.isMonitoring();
	}
	return false;
}

std::map<std::string, double> ChargeFactory::getQs(const std::vector<std::string>& names)
{
	std::map<std::string, double> qs;
	for (auto name : names)
	{
		double q = chargeMap.find(name)->second.getQ();
		qs[name] = q;
	}
	return qs;
}

std::map<std::string, double> ChargeFactory::getPositions(const std::vector<std::string>& names)
{
	std::map<std::string, double> positionmap;
	for (auto name : names)
	{
		double position = chargeMap.find(name)->second.getPosition();
		positionmap[name] = position;
	}
	return positionmap;
}

std::map<std::string, std::vector< double > > ChargeFactory::getQVectors(const std::vector<std::string>& names)
{
	std::map<std::string, std::vector< double > > qvmap;
	for (auto name : names)
	{
		std::vector< double > qv = chargeMap.find(name)->second.getQVector();
		qvmap[name] = qv;
	}
	return qvmap;
}

std::map<std::string, boost::circular_buffer< double > > ChargeFactory::getQBuffers(const std::vector<std::string>& names)
{
	std::map<std::string, boost::circular_buffer< double > > qvmap;
	for (auto name : names)
	{
		boost::circular_buffer< double > qv = chargeMap.find(name)->second.getQBuffer();
		qvmap[name] = qv;
	}
	return qvmap;
}

std::map<std::string, double> ChargeFactory::getAllQ()
{
	std::map<std::string, double> chargeAndQMap;
	for (auto charge: chargeMap)
	{
		std::pair<std::string, double> nameAndQPair = std::make_pair(charge.first, charge.second.getQ());
		chargeAndQMap.insert(nameAndQPair);
	}
	return chargeAndQMap;
}

std::map<std::string, double> ChargeFactory::getAllPosition()
{
	std::map<std::string, double> chargeAndPositionMap;
	for (auto charge : chargeMap)
	{
		std::pair<std::string, double> nameAndPositionPair = std::make_pair(charge.first, charge.second.getPosition());
		chargeAndPositionMap.insert(nameAndPositionPair);
	}
	return chargeAndPositionMap;
}

std::map<std::string, std::vector< double > > ChargeFactory::getAllQVector()
{
	std::map<std::string, std::vector< double > > chargeAndQMap;
	for (auto charge : chargeMap)
	{
		std::pair<std::string, std::vector< double > > nameAndQPair = std::make_pair(charge.first, charge.second.getQVector());
		chargeAndQMap.insert(nameAndQPair);
	}
	return chargeAndQMap;
}

std::map<std::string, boost::circular_buffer< double > > ChargeFactory::getAllQBuffer()
{
	std::map<std::string, boost::circular_buffer< double > > chargeAndQMap;
	for (auto charge : chargeMap)
	{
		std::pair<std::string, boost::circular_buffer< double > > nameAndQPair = std::make_pair(charge.first, charge.second.getQBuffer());
		chargeAndQMap.insert(nameAndQPair);
	}
	return chargeAndQMap;
}

void ChargeFactory::monitorForNShots_Py(boost::python::list names, const size_t& value)
{
	std::vector<std::string> chargeNamesVector = to_std_vector<std::string>(names);
	monitorMultipleForNShots(chargeNamesVector, value);
}

boost::python::list ChargeFactory::getQVector_Py(const std::string& chargeName)
{
	std::vector< double > qvec;
	qvec = getQVector(chargeName);
	boost::python::list newPyList = to_py_list(qvec);
	return newPyList;
}

boost::python::list ChargeFactory::getQBuffer_Py(const std::string& chargeName)
{
	boost::circular_buffer< double > qbuf;
	qbuf = getQBuffer(chargeName);
	//std::vector< double > qvec = to_std_vector<double>(qbuf);
	boost::python::list newPyList = to_py_list(qbuf);
	return newPyList;
}

boost::python::dict ChargeFactory::getQs_Py(boost::python::list chargeNames)
{
	std::map<std::string, double> qvals;
	std::vector<std::string> chargeNamesVector = to_std_vector<std::string>(chargeNames);
	qvals = getQs(chargeNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ChargeFactory::getPositions_Py(boost::python::list chargeNames)
{
	std::map<std::string, double> positionvals;
	std::vector<std::string> chargeNamesVector = to_std_vector<std::string>(chargeNames);
	positionvals = getPositions(chargeNamesVector);
	boost::python::dict newPyDict = to_py_dict(positionvals);
	return newPyDict;
}

boost::python::dict ChargeFactory::getQVectors_Py(boost::python::list chargeNames)
{
	std::map<std::string, std::vector< double > > qvals;
	std::vector<std::string> chargeNamesVector = to_std_vector<std::string>(chargeNames);
	qvals = getQVectors(chargeNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ChargeFactory::getQBuffers_Py(boost::python::list chargeNames)
{
	std::map<std::string, boost::circular_buffer< double > > qvals;
	std::vector<std::string> chargeNamesVector = to_std_vector<std::string>(chargeNames);
	qvals = getQBuffers(chargeNamesVector);
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ChargeFactory::getAllQ_Py()
{
	std::map<std::string, double> qvals = getAllQ();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ChargeFactory::getAllQVector_Py()
{
	std::map<std::string, std::vector< double > > qvals = getAllQVector();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ChargeFactory::getAllQBuffer_Py()
{
	std::map<std::string, boost::circular_buffer< double > > qvals = getAllQBuffer();
	boost::python::dict newPyDict = to_py_dict(qvals);
	return newPyDict;
}

boost::python::dict ChargeFactory::getAllPosition_Py()
{
	std::map<std::string, double> positionvals = getAllPosition();
	boost::python::dict newPyDict = to_py_dict(positionvals);
	return newPyDict;
}

