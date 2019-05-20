#include "HardwareFactory.h"
#include <iostream>
#include <boost/algorithm/algorithm.hpp>
#include <boost/filesystem.hpp>
#include <boost/python.hpp>

const std::string MASTER_LATTICE_FILE_LOCATION = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
std::vector<std::string> findYAMLFilesInDirectory(std::string hardwareType, std::string version)
{
	boost::filesystem::path directory(MASTER_LATTICE_FILE_LOCATION+"\\"+hardwareType); // + '//' + hardwareType + '//' + version);
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
	magnetFactory = new MagnetFactory();
}
bool HardwareFactory::setup(std::string hardwareType, std::string version)
{
	bool setup = false;
	if (hardwareType == "Magnet")
	{
		if (!magnetFactory->hasBeenSetup)
		{
			setup = magnetFactory->setup(version);
		}
	}
	return setup;
}
MagnetFactory* HardwareFactory::getMagnetFactory()
{
	return this->magnetFactory;
}
bool HardwareFactory::operator==(const HardwareFactory &HardwareFactory) const
{
	//return(HardwareFactory::HardwareFactory_name.compare(HardwareFactory.HardwareFactory_name)
	//	&& HardwareFactory::HardwareFactory_type.compare(HardwareFactory.HardwareFactory_type));
	return true;
}