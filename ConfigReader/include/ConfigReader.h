#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include "LoggingSystem.h"
#include <map>
#include <string>


class ConfigReader
{
public:
	ConfigReader(const std::string filename, bool isVirtual);
	ConfigReader();
	std::string yamlFileDestination;
	std::string yamlFilename;
	std::string hardwareFolder;
	bool loadVirtualHardware;
	std::map<std::string, std::string> parameters;
	std::map<std::string, std::string> parseYamlFile();
	std::string getHardwareTypeFromName(std::string fullPVName);
	//std::multimap<std::string, std::string> parseYamlDirectory();
	std::vector<std::string> findYAMLFilesInDirectory();

	// defining the allowed hardware types and their EPICS abbreviations
	// these are currently hard-coded, we should get the folder names from
	// MasterLattice directory to initialize the map
	const static std::map<std::string, std::string> allowedHardwareTypes;
};
typedef std::multimap< std::string, std::string> componentParameterMap;
typedef std::shared_ptr<ConfigReader> ConfigReader_sptr;
#endif //CONFIG_READER_H_