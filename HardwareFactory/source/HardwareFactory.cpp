#include "HardwareFactory.h"
#include "Magnet.h"
#include <iostream>
#include <boost/algorithm/algorithm.hpp>
#include <boost/filesystem.hpp>
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE(CATApLib)
{
	
}

const std::string MASTER_LATTICE_FILE_LOCATION = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";

std::vector<std::string> findYAMLFilesInDirectory(std::string hardwareType, std::string version)
{
	boost::filesystem::path directory(MASTER_LATTICE_FILE_LOCATION);
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
		}
		else
		{
			continue;
		}
	}
	return filenames;
}

HardwareFactory::HardwareFactory(){
	messenger = LoggingSystem(true, true);
	messenger.printDebugMessage(std::string("Hardware Factory Constructed"));
	//reader = ConfigReader();
}
bool HardwareFactory::setup(std::string hardwareType, std::string version)
{
	std::vector<std::string> filenames = findYAMLFilesInDirectory(hardwareType, version);
	reader = ConfigReader();
	for (auto file : filenames)
	{
		//This is where we want to create a container of specific hardware
		//objects, which inherit from the base Hardware object, by passing
		// the paramsAndValuesMap to a specific Factory, i.e. MagnetFactory

		//need to decide what to do with template file,
		// we do not want to parse this here!
		if (file != "Magnet.yaml")
		{
			reader.yamlFilename = file;
			std::multimap<std::string, std::string> paramsAndValuesMap = reader.parseYamlFile();
			if (paramsAndValuesMap.find("hardware_type")->second == "Magnet")
			{
				Magnet *mag = new Magnet(paramsAndValuesMap);
				magnetVector.push_back(mag);
			}
		}
	}
	return true;
}

std::vector<Magnet*> HardwareFactory::getAllMagnets()
{
	return magnetVector;
}

Magnet* HardwareFactory::getMagnet(std::string fullMagnetName)
{
	for (auto magnet : magnetVector)
	{
		if (magnet->getFullPVName() == fullMagnetName)
		{
			return magnet;
		}
	}
	return NULL;
}

bool HardwareFactory::operator==(const HardwareFactory &HardwareFactory) const
{
	//return(HardwareFactory::HardwareFactory_name.compare(HardwareFactory.HardwareFactory_name)
	//	&& HardwareFactory::HardwareFactory_type.compare(HardwareFactory.HardwareFactory_type));
	return true;
}