#include "MagnetFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#ifndef __CINT__
#include <cadef.h>
#endif

#if defined(__unix__) ||  defined(_unix)
  const std::string HOME =  getenv("HOME");
  const std::string MASTER_LATTICE_FILE_LOCATION = HOME +"/MasterLattice";//"~/MasterLattice";
  const std::string SEPARATOR = "/";
#endif
#ifdef _WIN32
  const std::string MASTER_LATTICE_FILE_LOCATION = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
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
MagnetFactory::MagnetFactory()
{
	messenger = LoggingSystem(false, false);
	hasBeenSetup = false;
	messenger.printDebugMessage(std::string("Magnet Factory Constructed"));

}


bool MagnetFactory::setup(std::string version)
{
	std::vector<std::string> filenames = findYAMLFilesInDirectory(version);
	reader = ConfigReader();
	reader.yamlFileDestination = MASTER_LATTICE_FILE_LOCATION;
	for (auto file : filenames)
	{
		if (file != "Magnet.yaml")
		{
			reader.yamlFilename = file;
			std::map<std::string, std::string> paramsAndValuesMap = reader.parseYamlFile();
			Magnet* mag = new Magnet(paramsAndValuesMap);
			// epics magnet interface has been initialized in Magnet constructor
			// but we have a lot of PV information to retrieve from EPICS first
			// so we will cycle through the PV structs, and set up their values.
			//
			// a PVStruct should also probably have an update function ptr as a member??
			std::vector<pvStruct>* magPVStructs = mag->getPVStructs();
			for (auto pv = magPVStructs->begin(); pv != magPVStructs->end(); pv++)
			{	
				int status;
				std::string pvAndRecordName = pv->fullPVName + ":" + pv->pvRecord;
				pv->CHID = mag->epicsInterface->retrieveCHID(pvAndRecordName);
				status = ca_pend_io(1.0);
				SEVCHK(status, "ca_pend_io failed");
				//pv->COUNT = mag->epicsInterface->retrieveCOUNT(pv->CHID);
				//pv->CHTYPE = mag->epicsInterface->retrieveCHTYPE(pv->CHID);
				// not sure how to set the mask from EPICS yet.
				//pv->MASK;
				messenger.printDebugMessage("read" + std::to_string(ca_read_access(pv->CHID)) +
											"write" + std::to_string(ca_write_access(pv->CHID)) +
											"state" + std::to_string(ca_state(pv->CHID)) + "\n");
			}
			magnetVec.push_back(mag);
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
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
			double current = magnet->getCurrent();
			return current;
		}
	}
	return std::numeric_limits<double>::min();
}
std::vector<double> MagnetFactory::getCurrents(std::vector<std::string> names)
{
	std::vector<double> currents;
	for (auto magnet : magnetVec)
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
	for (auto mag : magnetVec)
	{
		if (mag->getFullPVName() == name)
		{
			mag->setCurrent(value);
			return true;
		}
	}
	return false;
}
