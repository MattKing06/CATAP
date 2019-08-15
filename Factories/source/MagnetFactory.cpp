#include "MagnetFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
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

// MEMBER FUNCTIONS //
MagnetFactory::MagnetFactory() : MagnetFactory(false)
{
}
MagnetFactory::MagnetFactory(bool isVirtual)
{
	messenger = LoggingSystem(false, false);
	hasBeenSetup = false;
	messenger.printDebugMessage(std::string("Magnet Factory Constructed"));
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
	messenger.messagesOn();
	messenger.printMessage("MagnetFactory Destructor Called");
	/*CLEAN UP CODE FOR MAGNET FACTORY DOES NOT WORK IN PYTHON YET*/
	// The MagnetFactory object is destroyed by python too quickly
	// I believe this has something to do with python not managing the object
	//for (auto& magnet : magnetMap)
	//{
	//	auto pvStructsList = magnet.second->getPVStructs();
	//	for (auto& pvStruct : pvStructsList)
	//	{
	//		if (pvStruct.second->EVID)
	//		{
	//			magnet.second->epicsInterface->destroySubscription(pvStruct.second->EVID);
	//			ca_flush_io();
	//			
	//		}
	//		if (pvStruct.second->CHID)
	//		{
	//			magnet.second->epicsInterface->clearChannel(pvStruct.second->CHID);
	//		}
	//	}
	//}
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

bool MagnetFactory::setup(const std::string &version)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (this->isVirtual)
	{
		messenger.debugMessagesOn();
		messenger.printDebugMessage(" VIRTUAL SETUP: TRUE");
	}
		//if the new magnet we build is not static, we cannot be sure we are
		// storing a proper reference to it in our magnetMap due to scope.
		// The static keyword is used here to avoid raw pointers, we store a reference to
		// the magnet in the magnetMap

		//mag is retains its value even when we try to set it to a different constructor..
		// when we try to store a VCOR magnet, it still has the HCOR parameter values attached
		//Magnet mag = magnetMap.
		//// epics magnet interface has been initialized in Magnet constructor
		//// but we have a lot of PV information to retrieve from EPICS first
		//// so we will cycle through the PV structs, and set up their values.
	populateMagnetMap();
	for (auto &magnet : magnetMap)
	{
		std::map<std::string, pvStruct*> magPVStructs = magnet.second.getPVStructs();
		for (auto &pv : magPVStructs)
		{
			std::string pvAndRecordName = magnet.second.getFullPVName() + ":" + pv.first;
			pv.second->CHID = magnet.second.epicsInterface->retrieveCHID(pvAndRecordName);
			pv.second->CHTYPE = magnet.second.epicsInterface->retrieveCHTYPE(pv.second->CHID);
			pv.second->COUNT = magnet.second.epicsInterface->retrieveCOUNT(pv.second->CHID);
			pv.second->updateFunction = magnet.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.first);
			// not sure how to set the mask from EPICS yet.
			pv.second->MASK = DBE_VALUE;
			messenger.debugMessagesOn();
			messenger.printDebugMessage(pv.second->pvRecord + ": read" + std::to_string(ca_read_access(pv.second->CHID)) +
				"write" + std::to_string(ca_write_access(pv.second->CHID)) +
				"state" + std::to_string(ca_state(pv.second->CHID)) + "\n");
			magnet.second.epicsInterface->createSubscription(magnet.second, pv.second->pvRecord);
		}
	}

		//// inserts new key-value per ONLY IF key is unique.
		//magnetMap.emplace(mag->getFullPVName(), mag);
	hasBeenSetup = true;
	return hasBeenSetup;
}


updateFunctionPtr MagnetFactory::findUpdateFunctionForRecord(std::string record, Magnet mag)
{
	if (record == "GETSETI")
	{
		return mag.epicsInterface->updateCurrent;
	}
	return nullptr;
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
Magnet MagnetFactory::getMagnet(std::string fullMagnetName)
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

double MagnetFactory::getCurrent(std::string name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
	}
	else
	{
		return magnetMap.find(name)->second.getCurrent();
	}
	return std::numeric_limits<double>::min();;
}
std::map<std::string, double> MagnetFactory::getCurrents(std::vector<std::string> names)
{
	std::map<std::string, double> currents;
	for (auto name : names)
	{
		double current = magnetMap.find(name)->second.getCurrent();
		currents[name] = current;
	}
	return currents;
}
/*UTILITY FUNCTIONS [NEED TO BE MOVED SOMEWHERE ACCESSIBLE BY EVERYONE]*/
template< typename typeOfNewVector>
inline
std::vector<typeOfNewVector> MagnetFactory::to_std_vector(const boost::python::object& iterable)
{
	return std::vector<typeOfNewVector>(boost::python::stl_input_iterator<typeOfNewVector>(iterable),
		boost::python::stl_input_iterator<typeOfNewVector>());
}
template<class typeOfVectorToConvert>
inline
boost::python::list MagnetFactory::to_py_list(std::vector<typeOfVectorToConvert> vector)
{
	typename std::vector<typeOfVectorToConvert>::iterator iter;
	boost::python::list newList;
	for (iter = vector.begin(); iter != vector.end(); ++iter)
	{
		newList.append(*iter);
	}
	return newList;
}
template<class key, class value>
inline
boost::python::dict MagnetFactory::to_py_dict(std::map<key, value> map)
{
	typename std::map<key, value>::iterator iter;
	boost::python::dict newDictionary;
	for (iter = map.begin(); iter != map.end(); ++iter)
	{
		newDictionary[iter->first] = iter->second;
	}
	return newDictionary;
}
/*END OF UTILITY FUNCTIONS [NEED TO BE MOVED SOMEWHERE ACCESSBILE BY EVERYONE]*/
boost::python::dict MagnetFactory::getCurrents_Py(boost::python::list magNames)
{
	std::map<std::string, double> currents;
	std::vector<std::string> magNamesVector = to_std_vector<std::string>(magNames);
	currents = getCurrents(magNamesVector);
	boost::python::dict newPyDict = to_py_dict(currents);
	return newPyDict;
}
std::map<std::string, double> MagnetFactory::getAllMagnetCurrents()
{
	std::map<std::string, double> magnetsAndCurrentsMap;
	for (auto mag : magnetMap)
	{
		std::pair<std::string, double> nameAndCurrentPair = std::make_pair(mag.first,
			mag.second.getCurrent());
		magnetsAndCurrentsMap.insert(nameAndCurrentPair);
	}
	return magnetsAndCurrentsMap;
}

bool MagnetFactory::setCurrent(std::string name, double value)
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
