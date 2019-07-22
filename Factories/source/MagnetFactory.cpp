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

#if defined(__unix__) ||  defined(_unix)
  const std::string HOME =  getenv("HOME");
  const std::string MASTER_LATTICE_FILE_LOCATION = HOME +"/MasterLattice";//"~/MasterLattice";
  const std::string SEPARATOR = "/";
#endif
#ifdef _WIN32
  const std::string MASTER_LATTICE_FILE_LOCATION = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\Magnet";
  const std::string SEPARATOR = "\\";
#endif

typedef void(*updateFunctionPtr)(struct event_handler_args args);

// NON-MEMBER HELPER FUNCTIONS //
std::vector<std::string> findYAMLFilesInDirectory(std::string version)
{
	boost::filesystem::path directory(MASTER_LATTICE_FILE_LOCATION);//+ '//' + version);
	std::vector<std::string> filenames;
	for (auto i = boost::filesystem::directory_iterator(directory); i != boost::filesystem::directory_iterator(); i++)
	{
		if (!boost::filesystem::is_directory(i->path()))
		{
			if (i->path().extension() == ".YAML"
				|| i->path().extension() == ".YML"
				|| i->path().extension() == ".yaml"
				|| i->path().extension() == ".yml")
			{
				filenames.push_back(i->path().filename().string());
			}
			else
			{
				std::cout << i->path().filename().string() << ": NOT YAML" << std::endl;
			}
		}
		else
		{
			continue;
		}
	}
	return filenames;
}


// MEMBER FUNCTIONS //
MagnetFactory::MagnetFactory() : MagnetFactory(false)
{
}
MagnetFactory::MagnetFactory(bool isVirtual)
{
	this->messenger = LoggingSystem(false, false);
	this->hasBeenSetup = false;
	this->messenger.printDebugMessage(std::string("Magnet Factory Constructed"));
	this->virtualMagnetFactory = isVirtual;
	this->reader = ConfigReader();
}


bool MagnetFactory::setup(std::string version)
{
	std::vector<std::string> filenames = findYAMLFilesInDirectory(version);
	for (auto file : filenames)
	{
		if (file != "Magnet.yaml")
		{
			reader.yamlFileDestination = MASTER_LATTICE_FILE_LOCATION;
			reader.yamlFilename = file;
			reader.loadVirtualHardware = this->virtualMagnetFactory;
			if (this->virtualMagnetFactory)
			{
				messenger.printDebugMessage(" VIRTUAL SETUP: TRUE");
			}

			std::map<std::string, std::string> paramsAndValuesMap = reader.parseYamlFile();
     		Magnet *mag = new Magnet(paramsAndValuesMap, this->virtualMagnetFactory);
			// epics magnet interface has been initialized in Magnet constructor
			// but we have a lot of PV information to retrieve from EPICS first
			// so we will cycle through the PV structs, and set up their values.
			//
			// a PVStruct should also probably have an update function ptr as a member??
			std::map<std::string, pvStruct*> magPVStructs = mag->getPVStructs();
			for (auto &pv : magPVStructs)
			{
				std::string pvAndRecordName = pv.second->fullPVName + ":" + pv.first;
				pv.second->CHID = mag->epicsInterface->retrieveCHID(pvAndRecordName);
				pv.second->CHTYPE = mag->epicsInterface->retrieveCHTYPE(pv.second->CHID);
				pv.second->COUNT = mag->epicsInterface->retrieveCOUNT(pv.second->CHID);
				pv.second->updateFunction = findUpdateFunctionForRecord(pv.first, mag);
				// not sure how to set the mask from EPICS yet.
				pv.second->MASK = DBE_VALUE;
				messenger.debugMessagesOn();
				messenger.printDebugMessage(pv.second->pvRecord + ": read" + std::to_string(ca_read_access(pv.second->CHID)) +
					"write" + std::to_string(ca_write_access(pv.second->CHID)) +
					"state" + std::to_string(ca_state(pv.second->CHID)) + "\n");
				mag->epicsInterface->createSubscription(*(mag), pv.second->pvRecord);
			}
			magnetVec.push_back(mag);
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

updateFunctionPtr MagnetFactory::findUpdateFunctionForRecord(std::string record, Magnet* mag)
{
	if (record == "GETSETI")
	{
		return mag->epicsInterface->updateCurrent;
	}
	return nullptr;
}

std::vector<Magnet*> MagnetFactory::getMagnets(std::vector<std::string> magnetNames)
{
	std::vector<Magnet*> selectedMagnets;
	for (auto &magnet : magnetVec)
	{
		for (auto &name : magnetNames)
		{
			if (magnet->getFullPVName() == name)
			{
				selectedMagnets.push_back(magnet);
			}
		}
	}
	return selectedMagnets;
}
Magnet* MagnetFactory::getMagnet(std::string fullMagnetName)
{
	for (auto &magnet : magnetVec)
	{
		if (magnet->getFullPVName() == fullMagnetName)
		{
			return magnet;
		}
	}
	return NULL;
}
std::vector<Magnet*> MagnetFactory::getAllMagnets()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("MAGNETS HAVE ALREADY BEEN CONSTRUCTED.");
	}
	return magnetVec;
}

double MagnetFactory::getCurrent(std::string name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call MagnetFactory.setup(VERSION)");
	}
	for (auto &magnet : magnetVec)
	{
		if (magnet->getFullPVName() == name)
		{
			return magnet->getCurrent();
		}
	}
	return std::numeric_limits<double>::min();;
}
std::vector<double> MagnetFactory::getCurrents(std::vector<std::string> names)
{
	std::vector<double> currents;
	for (auto &magnet : magnetVec)
	{
		for (auto name : names)
		{
			if (magnet->getFullPVName() == name)
			{
				currents.push_back(magnet->getCurrent());
			}
		}
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
boost::python::dict to_py_dict(std::map<key, value> map)
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
boost::python::list MagnetFactory::getCurrents_Py(boost::python::list magNames)
{
	std::vector<double> currents;
	std::vector<std::string> magNamesVector = to_std_vector<std::string>(magNames);
	currents = getCurrents(magNamesVector);
	boost::python::list newPyList = to_py_list(currents);
	return newPyList;
}
std::map<std::string, double> MagnetFactory::getAllMagnetCurrents()
{
	std::map<std::string, double> magnetsAndCurrentsMap;
	for (auto mag : magnetVec)
	{
		std::pair<std::string, double> nameAndCurrentPair = std::make_pair(mag->getFullPVName(),
			mag->getCurrent());
		magnetsAndCurrentsMap.insert(nameAndCurrentPair);
	}
	return magnetsAndCurrentsMap;
}

bool MagnetFactory::setCurrent(std::string name, double value)
{
	for (auto &mag : magnetVec)
	{
		if (mag->getFullPVName() == name)
		{
			mag->setEPICSCurrent(value);
			return true;
		}
	}
	return false;
}
