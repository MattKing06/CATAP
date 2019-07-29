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

			//if the new magnet we build is not static, we cannot be sure we are
			// storing a proper reference to it in our magnetMap due to scope.
			// The static keyword is used here to avoid raw pointers, we store a reference to
			// the magnet in the magnetMap
     		static Magnet mag(paramsAndValuesMap, this->virtualMagnetFactory);
			// epics magnet interface has been initialized in Magnet constructor
			// but we have a lot of PV information to retrieve from EPICS first
			// so we will cycle through the PV structs, and set up their values.
			std::map<std::string, pvStruct*> magPVStructs = mag.getPVStructs();
			for (auto &pv : magPVStructs)
			{
				std::string pvAndRecordName = mag.getFullPVName() + ":" + pv.first;
				pv.second->CHID = mag.epicsInterface->retrieveCHID(pvAndRecordName);
				pv.second->CHTYPE = mag.epicsInterface->retrieveCHTYPE(pv.second->CHID);
				pv.second->COUNT = mag.epicsInterface->retrieveCOUNT(pv.second->CHID);
				pv.second->updateFunction = findUpdateFunctionForRecord(pv.first, &mag);
				// not sure how to set the mask from EPICS yet.
				pv.second->MASK = DBE_VALUE;
				messenger.debugMessagesOn();
				messenger.printDebugMessage(pv.second->pvRecord + ": read" + std::to_string(ca_read_access(pv.second->CHID)) +
					"write" + std::to_string(ca_write_access(pv.second->CHID)) +
					"state" + std::to_string(ca_state(pv.second->CHID)) + "\n");
				mag.epicsInterface->createSubscription(mag, pv.second->pvRecord);
			}
			// inserts new key-value per ONLY IF key is unique.
			magnetMap.emplace(mag.getFullPVName(), &mag);
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

std::map<std::string, Magnet*> MagnetFactory::getMagnets(std::vector<std::string> magnetNames)
{
	std::map<std::string, Magnet*> selectedMagnets;
	for (auto &magName : magnetNames)
	{
		selectedMagnets[magName] = magnetMap.find(magName)->second;
	}
	return selectedMagnets;
}
Magnet* MagnetFactory::getMagnet(std::string fullMagnetName)
{
	return magnetMap.find(fullMagnetName)->second;
}
std::map<std::string, Magnet*> MagnetFactory::getAllMagnets()
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
		double current = magnetMap.find(name)->second->getCurrent();
		return current;
	}
	return std::numeric_limits<double>::min();;
}
std::map<std::string, double> MagnetFactory::getCurrents(std::vector<std::string> names)
{
	std::map<std::string, double> currents;
	for (auto name : names)
	{
		double current = magnetMap.find(name)->second->getCurrent();
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
			mag.second->getCurrent());
		magnetsAndCurrentsMap.insert(nameAndCurrentPair);
	}
	return magnetsAndCurrentsMap;
}

bool MagnetFactory::setCurrent(std::string name, double value)
{
	auto mag = magnetMap.find(name)->second;
	mag->setEPICSCurrent(value);
	return true;
}
