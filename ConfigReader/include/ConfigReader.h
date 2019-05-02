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
	ConfigReader(const std::string filename);
	ConfigReader();
	//ConfigReader(const std::string directory);
	std::string yamlFileDestination;
	std::string yamlFilename;
	std::multimap<std::string, std::string> PVs;
	std::multimap<std::string, std::string> parseYamlFile();
	//std::multimap<std::string, std::string> parseYamlDirectory();
	std::vector<std::string> findYAMLFilesInDirectory();
};
typedef std::multimap< std::string, std::string> componentParameterMap;
typedef std::shared_ptr<ConfigReader> ConfigReader_sptr;
#endif //CONFIG_READER_H_