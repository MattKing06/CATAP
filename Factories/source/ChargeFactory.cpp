#include "ChargeFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <utility>
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif

ChargeFactory::ChargeFactory() : ChargeFactory(STATE::OFFLINE)
{
	std::cout << "ChargeFactory DEFAULT constRUCTOR called " << std::endl;
}

ChargeFactory::ChargeFactory(STATE mode) :
ChargeFactory(mode, MASTER_LATTICE_FILE_LOCATION)
{

}

ChargeFactory::ChargeFactory(STATE mode, const std::string& primeLatticeLocation) :
mode(mode),
hasBeenSetup(false),
reader(ConfigReader("Charge", mode, primeLatticeLocation))
{
	messenger = LoggingSystem(false, false);
	//hasBeenSetup = false;
//	messenger = LoggingSystem(false, false);
	messenger.printDebugMessage("Charge Factory constructed");
	//mode = mode;
	//reader = ConfigReader("Charge", mode);
}

ChargeFactory::ChargeFactory(const ChargeFactory& copyChargeFactory)
	: hasBeenSetup(copyChargeFactory.hasBeenSetup),
	mode(copyChargeFactory.mode),
	messenger(copyChargeFactory.messenger),
	reader(copyChargeFactory.reader)
{
	messenger.printDebugMessage("ChargeFactory Copy cOntructor");
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
				if (pvStruct.second.EVID)
				{
					charge.second.epicsInterface->removeSubscription(pvStruct.second);
					ca_flush_io();
				}
			}
			charge.second.epicsInterface->removeChannel(pvStruct.second);
			ca_pend_io(CA_PEND_IO_TIMEOUT);
		}
	}
}

void ChargeFactory::attachContext(const std::string& chargeName)
{
	if (GlobalFunctions::entryExists(chargeMap, chargeName))
	{
		chargeMap.at(chargeName).attachToInitialContext();
	}
	else
	{
		messenger.printMessage("Could not find ", chargeName, " in hardware map.");
	}
}

void ChargeFactory::attachContext(std::vector<std::string>& chargeNames)
{
	for (auto&& name : chargeNames)
	{
		attachContext(name);
	}
}

void ChargeFactory::attachContext_Py(boost::python::list chargeNames)
{
	std::vector<std::string> names = to_std_vector<std::string>(chargeNames);
	attachContext(names);
}

void ChargeFactory::attachContext()
{
	for (auto&& charge : chargeMap)
	{
		charge.second.attachToInitialContext();
	}
}

void ChargeFactory::populateChargeMap()
{
	messenger.printDebugMessage("ChargeFactory is populating the charge map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive cOnfiguratiOn parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Charge Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(chargeMap);
	}
	messenger.printDebugMessage("ChargeFactory has finished populating the charge map");
}

void ChargeFactory::retrievemonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == ChargeRecords::Q)
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
}

void ChargeFactory::setupChannels()
{
	for (auto& charge : chargeMap)
	{
		std::map<std::string, pvStruct>& pvStructs = charge.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			charge.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

bool ChargeFactory::setup(const std::string& VERSION)
{
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("ChargeFactory hasBeenSetup already");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	messenger.printDebugMessage("ChargeFactory setup populateChargeMap");
	//// epics magnet interface has been initialized in BPM constructor
	//// but we have a lot of PV informatiOn to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateChargeMap();

	setupChannels();
	EPICSInterface::sendToEPICSm("ChargeFactory connect PVs");
	for (auto& charge : chargeMap)
	{
		std::map<std::string, pvStruct>& chargePVStructs = charge.second.getPVStructs();
		for (auto& pv : chargePVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrievemonitorStatus(pv.second);
				charge.second.epicsInterface->retrieveCHTYPE(pv.second);
				charge.second.epicsInterface->retrieveCOUNT(pv.second);
				charge.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					charge.second.epicsInterface->createSubscription(charge.second, pv.second);
				}
				EPICSInterface::sendToEPICSm("ChargeFactory connect PVs");
			}
			else
			{
				messenger.printMessage(charge.first, " CANNOT CONNECT TO EPICS");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
	std::cout << "end" << std::endl;
}

std::map<std::string, Charge> ChargeFactory::getChargeDiagnostics(std::vector<std::string> chargeNames)
{
	std::map<std::string, Charge> selectedCharges;
	for (auto& chargeName : chargeNames)
	{
		selectedCharges[chargeName] = chargeMap.find(chargeName)->second;
	}
	return selectedCharges;
}

boost::python::list ChargeFactory::getAllChargeDiagnosticNames_Py()
{
	std::vector<std::string> names;
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		for (auto& it : chargeMap)
		{
			names.push_back(it.first);
		}
	}
	return to_py_list(names);
}

Charge& ChargeFactory::getChargeDiagnostic(const std::string& fullChargeName)
{
	return chargeMap.at(fullChargeName);
}

std::map<std::string, Charge> ChargeFactory::getAllChargeDiagnostics()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("CHARGE DIAGNOSTICS HAVE ALREADY BEEN constRUCTED.");
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

std::vector<std::string> ChargeFactory::getAllChargeDiagnosticNames()
{
	std::vector<std::string> chargenames;
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		for (auto& it : chargeMap)
		{
			chargenames.push_back(it.first);
		}
		return chargenames;
	}
	return chargenames;
}

void ChargeFactory::setBufferSize(const std::string& name, const size_t& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		chargeMap.find(name)->second.setBufferSize(value);
	}
}

void ChargeFactory::setAllBufferSize(const size_t& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		for (auto& it : chargeMap)
		{
			chargeMap.find(it.first)->second.setBufferSize(value);
		}
	}
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

void ChargeFactory::monitorAllForNShots(const size_t& value)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		for (auto& it : chargeMap)
		{
			chargeMap.find(it.first)->second.monitorForNShots(value);
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


bool ChargeFactory::ismonitoring(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ChargeFactory.setup(VERSION)");
	}
	else
	{
		return chargeMap.find(name)->second.ismonitoring();
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
	std::map<std::string, double> positiOnmap;
	for (auto name : names)
	{
		double positiOn = chargeMap.find(name)->second.getPosition();
		positiOnmap[name] = positiOn;
	}
	return positiOnmap;
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
	for (auto charge : chargeMap)
	{
		std::pair<std::string, double> nameAndQPair = std::make_pair(charge.first, charge.second.getQ());
		chargeAndQMap.insert(nameAndQPair);
	}
	return chargeAndQMap;
}

std::map<std::string, double> ChargeFactory::getAllPosition()
{
	std::map<std::string, double> chargeAndPositiOnMap;
	for (auto charge : chargeMap)
	{
		std::pair<std::string, double> nameAndPositiOnPair = std::make_pair(charge.first, charge.second.getPosition());
		chargeAndPositiOnMap.insert(nameAndPositiOnPair);
	}
	return chargeAndPositiOnMap;
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
	std::map<std::string, double> positiOnvals;
	std::vector<std::string> chargeNamesVector = to_std_vector<std::string>(chargeNames);
	positiOnvals = getPositions(chargeNamesVector);
	boost::python::dict newPyDict = to_py_dict(positiOnvals);
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
	std::map<std::string, double> positiOnvals = getAllPosition();
	boost::python::dict newPyDict = to_py_dict(positiOnvals);
	return newPyDict;
}

void ChargeFactory::setRunningStatSize(const std::string& name, const size_t& size)
{
	if (GlobalFunctions::entryExists(chargeMap, name))
	{
		chargeMap.at(name).setRunningStatSize(size);
	}
}

void ChargeFactory::clearRunningStats(const std::string& name)
{
	if (GlobalFunctions::entryExists(chargeMap, name))
	{
		chargeMap.at(name).clearRunningStats();
	}
}

bool ChargeFactory::areAllRunningStatsFull(const std::string& name)
{
	if (GlobalFunctions::entryExists(chargeMap, name))
	{
		// TODO this is onky for the Qstat atm, and will need updating at a later date 
		return chargeMap.at(name).isRunningStatFull();
	}
	return false;
}
size_t ChargeFactory::getRunningStatNumDataValues(const std::string& name)const
{
	if (GlobalFunctions::entryExists(chargeMap, name))
	{
		return chargeMap.at(name).getRunningStatNumDataValues();
	}
	return GlobalConstants::size_zero;
}

void ChargeFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("CHARGE-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& charge : chargeMap)
	{
		charge.second.debugMessagesOn();
	}
}
void ChargeFactory::debugMessagesOff()
{
	messenger.printDebugMessage("CHARGE-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& charge : chargeMap)
	{
		charge.second.debugMessagesOff();
	}
}
void ChargeFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("CHARGE-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& charge : chargeMap)
	{
		charge.second.messagesOn();
	}
}
void ChargeFactory::messagesOff()
{
	messenger.printMessage("CHARGE-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& charge : chargeMap)
	{
		charge.second.messagesOff();
	}
}
bool ChargeFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool ChargeFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
