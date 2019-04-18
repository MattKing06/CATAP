#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include "LoggingSystem.h"
#include <vector>
#include <string>

class ConfigReader
{
public:
	ConfigReader(const std::string filename);
	std::string yamlFileDestination;
	std::string yamlFilename;
	std::vector<std::tuple<std::string, std::string> > PVs;
	std::vector<std::tuple<std::string, std::string> > parseYamlFile();
};
#endif //CONFIG_READER_H_