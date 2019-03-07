#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_
#include "Controller.h"
#include "LoggingSystem.h"
#include "yaml-cpp\parser.h"
#include "yaml-cpp\yaml.h"

class ConfigReader
{
public:
	ConfigReader(const std::string file);
	std::string yamlFileDestination;
	std::vector<std::string> *PVs;
	std::vector<std::string>* parseYamlFile();
};
#endif //CONFIG_READER_H_