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
	ConfigReader(const std::string &hardwareType, const bool &isVirtual);
	ConfigReader();
	std::map<std::string, std::string> parseNextYamlFile();
	const std::map<std::string, std::string> parseYamlFile();
	const std::string getHardwareTypeFromName(const std::string &fullPVName);
	const bool checkForValidTemplate(const YAML::Node &hardwareTemplate, const YAML::Node &hardwareComponent);
	std::vector<std::string> findYAMLFilesInDirectory(std::string version);
	void initialiseFilenameAndParsedStatusMap();
	const std::pair<std::string, std::string> extractControlsInformationIntoPair(const YAML::Node &controlsInformationNode);
	const std::map<std::string, std::string> extractHardwareInformationIntoMap(const YAML::Node &hardwareInformationNode);
	std::string yamlFileDestination;
	std::string yamlFilename;
	std::string hardwareFolder;
	bool loadVirtualHardware;
	std::map<std::string, bool> yamlFilenamesAndParsedStatusMap;
	int numberOfParsesExpected;
	bool hasMoreFilesToParse();
	// defining the allowed hardware types and their EPICS abbreviations
	// these are currently hard-coded, we should get the folder names from
	// MasterLattice directory to initialize the map
	const static std::map<std::string, std::string> allowedHardwareTypes;
};
#endif //CONFIG_READER_H_