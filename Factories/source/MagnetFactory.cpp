#include "MagnetFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif

MagnetFactory::MagnetFactory() : MagnetFactory(false)
{
}
MagnetFactory::MagnetFactory(bool isVirtual)
{
	messenger = LoggingSystem(false, false);
	hasBeenSetup = false;
	messenger.printDebugMessage("Magnet Factory Constructed");
	isVirtual = isVirtual;
	reader = ConfigReader("Magnet", isVirtual);
}
MagnetFactory::MagnetFactory(const MagnetFactory& copyMagnetFactory)
	: hasBeenSetup(copyMagnetFactory.hasBeenSetup),
	isVirtual(copyMagnetFactory.isVirtual),
	messenger(copyMagnetFactory.messenger),
	reader(copyMagnetFactory.reader)
{
	magnetMap.insert(copyMagnetFactory.magnetMap.begin(), copyMagnetFactory.magnetMap.end());
}

MagnetFactory::~MagnetFactory()
{
	messenger.printDebugMessage("MagnetFactory Destructor Called");
	for (auto& magnet : magnetMap)
	{
		auto pvStructsList = magnet.second.getPVStructs();
		for (auto& pvStruct : pvStructsList)
		{
			if (pvStruct.second.monitor)
			{
				magnet.second.epicsInterface->removeSubscription(pvStruct.second);
				ca_flush_io();
			}
			magnet.second.epicsInterface->removeChannel(pvStruct.second);
			ca_pend_io(CA_PEND_IO_TIMEOUT);
		}
	}
}

void MagnetFactory::populateMagnetMap()
{
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		reader.parseNextYamlFile(magnetMap);
	}
}

void MagnetFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == "GETSETI" ||
		pvStruct.pvRecord == "RPOWER" ||
		pvStruct.pvRecord == "READI" ||
		pvStruct.pvRecord == "RILK")
	{
		pvStruct.monitor = true;
	}
}

bool MagnetFactory::setup(const std::string &version)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (this->isVirtual)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in Magnet constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateMagnetMap();
	for (auto &magnet : magnetMap)
	{
		std::map<std::string, pvStruct>& magPVStructs = magnet.second.getPVStructs();
		for (auto &pv : magPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			retrieveMonitorStatus(pv.second);
			magnet.second.epicsInterface->retrieveCHID(pv.second);
			magnet.second.epicsInterface->retrieveCHTYPE(pv.second);
			magnet.second.epicsInterface->retrieveCOUNT(pv.second);
			magnet.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
			// not sure how to set the mask from EPICS yet.
			pv.second.MASK = DBE_VALUE;
			messenger.printDebugMessage( pv.second.pvRecord,": read", std::to_string(ca_read_access(pv.second.CHID)),
				"write", std::to_string(ca_write_access(pv.second.CHID)),
				"state", std::to_string(ca_state(pv.second.CHID)));
			if (pv.second.monitor)
			{
				magnet.second.epicsInterface->createSubscription(magnet.second, pv.second);
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

std::map<std::string, Magnet> MagnetFactory::getMagnets(std::vector<std::string> magnetNames)
{
	std::map<std::string, Magnet> selectedMagnets;
	for (auto &magName : magnetNames)
	{
		selectedMagnets[magName] = magnetMap.find(magName)->second;
	}
	return selectedMagnets;
}

Magnet& MagnetFactory::getMagnet(const std::string& fullMagnetName)
{
	return magnetMap.find(fullMagnetName)->second;
}
std::map<std::string, Magnet> MagnetFactory::getAllMagnets()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("MAGNETS HAVE ALREADY BEEN CONSTRUCTED.");
	}
	return magnetMap;
}

double MagnetFactory::getCurrent(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
	}
	else
	{
		return magnetMap.find(name)->second.getCurrent();
	}
	return std::numeric_limits<double>::min();
}
std::map<std::string, double> MagnetFactory::getCurrents(const std::vector<std::string>& names)
{
	std::map<std::string, double> currents;
	for (auto name : names)
	{
		double current = magnetMap.find(name)->second.getCurrent();
		currents[name] = current;
	}
	return currents;
}

double MagnetFactory::getRICurrent(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
	}
	else
	{
		return magnetMap.find(name)->second.getRICurrent();
	}
	return std::numeric_limits<double>::min();
}

std::map<std::string, double> MagnetFactory::getRICurrents(const std::vector<std::string>& names)
{
	std::map<std::string, double> RICurrents;
	for (auto& name : names)
	{
		double current = magnetMap.find(name)->second.getRICurrent();
		RICurrents[name] = current;
	}
	return RICurrents;
}
boost::python::dict MagnetFactory::getCurrents_Py(boost::python::list magNames)
{
	std::map<std::string, double> currents;
	std::vector<std::string> magNamesVector = to_std_vector<std::string>(magNames);
	currents = getCurrents(magNamesVector);
	boost::python::dict newPyDict = to_py_dict(currents);
	return newPyDict;
}

boost::python::dict MagnetFactory::getRICurrents_Py(boost::python::list names)
{
	std::map<std::string, double> RICurrents;
	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
	RICurrents = getRICurrents(namesVector);
	boost::python::dict newPyDict = to_py_dict(RICurrents);
	return newPyDict;
}

boost::python::dict MagnetFactory::getAllMagnetCurrents_Py()
{
	std::map<std::string, double> allCurrents = getAllMagnetCurrents();
	boost::python::dict allCurrentsPyDict = to_py_dict(allCurrents);
	return allCurrentsPyDict;
}
std::map<std::string, double> MagnetFactory::getAllMagnetCurrents()
{
	std::map<std::string, double> magnetsAndCurrentsMap;
	for (const auto &mag : magnetMap)
	{
		double current = mag.second.getCurrent();
		magnetsAndCurrentsMap[mag.first] = current;
	}
	return magnetsAndCurrentsMap;
}

bool MagnetFactory::setCurrent(const std::string& name, const double& value)
{
	return magnetMap.at(name).setEPICSCurrent(value);
}
bool MagnetFactory::setCurrents(const std::map<std::string, double> &namesAndCurrentsMap)
{
	for (const auto& entry : namesAndCurrentsMap)
	{
		setCurrent(entry.first, entry.second);
	}
	return true;

}
bool MagnetFactory::turnOn(const std::string& name)
{
	return magnetMap.at(name).setEPICSPSUState(STATE::ON);
}
bool MagnetFactory::turnOn(const std::vector<std::string>& names)
{
	for (auto& name : names)
	{
		turnOn(name);
	}
	return true;
}
bool MagnetFactory::turnOff(const std::string& name)
{
	return magnetMap.at(name).setEPICSPSUState(STATE::OFF);
}
bool MagnetFactory::turnOff(const std::vector<std::string>& names)
{
	for (auto& name : names)
	{
		turnOff(name);
	}
	return true;
}
int MagnetFactory::getPSUState(const std::string& name) const
{
	return magnetMap.find(name)->second.getPSUState();
}

int MagnetFactory::getILKState(const std::string& name) const
{
	return magnetMap.find(name)->second.getILKState();
}
bool MagnetFactory::turnOn_Py(boost::python::list names)
{
	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
	return turnOn(namesVector);

}
bool MagnetFactory::turnOff_Py(boost::python::list names)
{
	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
	return turnOff(namesVector);

}
void MagnetFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("MAG-FAC - DEBUG ON");
	reader.debugMessagesOn();
	for (auto& magnet : magnetMap)
	{
		magnet.second.debugMessagesOn();
	}
}
void MagnetFactory::debugMessagesOff()
{
	messenger.printDebugMessage("MAG-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& magnet : magnetMap)
	{
		magnet.second.debugMessagesOff();
	}
}
void MagnetFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("MAG-FAC - MESSAGES ON");
	reader.messagesOn();
	for (auto& magnet : magnetMap)
	{
		magnet.second.messagesOn();
	}
}
void MagnetFactory::messagesOff()
{
	messenger.printMessage("MAG-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& magnet : magnetMap)
	{
		magnet.second.messagesOff();
	}
}
bool MagnetFactory::setCurrents_Py(boost::python::dict magnetNamesAndCurrents)
{
	std::map<std::string, double> magnetNamesAndCurrentsToSet;
	std::vector<std::string> magnetNames = to_std_vector<std::string>(magnetNamesAndCurrents.keys());
	std::vector<double> currentsToSet = to_std_vector<double>(magnetNamesAndCurrents.values());
	auto magnetNamesIterator = magnetNames.begin();
	auto currentsToSetIterator = currentsToSet.begin();
	while (magnetNamesIterator != magnetNames.end() && currentsToSetIterator != currentsToSet.end())
	{
		std::pair<std::string, double> entryToInsert = std::pair<std::string, double>(magnetNamesIterator->data(), *currentsToSetIterator);
		magnetNamesAndCurrentsToSet.emplace(entryToInsert);
		magnetNamesIterator++;
		currentsToSetIterator++;
	}
	return setCurrents(magnetNamesAndCurrentsToSet);
}
