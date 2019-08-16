#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include "LoggingSystem.h"
#include <string>
#include <fstream>
#include <iostream>
#include <map>

#if defined(__unix__) ||  defined(_unix)
const std::string HOME = getenv("HOME");
const std::string MASTER_LATTICE_FILE_LOCATION = HOME + "/MasterLattice";
const std::string SEPARATOR = "/";
#endif
#ifdef _WIN32
const std::string MASTER_LATTICE_FILE_LOCATION = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\";
const std::string SEPARATOR = "\\";
#endif

class ConfigReader
{
public:
	ConfigReader(const std::string &hardwareType, const bool &isVirtual);
	ConfigReader();
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

	template<typename HardwareType>
	void parseNextYamlFile(std::map<std::string, HardwareType> &hardwareMapToFill)
	{
		for (auto filename : this->yamlFilenamesAndParsedStatusMap)
		{
			// boolean check here for safety, even though we remove all parsed files
			// anyway, just didn't trust myself..
			if (!filename.second)
			{
				yamlFilename = filename.first;
				parseYamlFile<HardwareType>(hardwareMapToFill);
				yamlFilenamesAndParsedStatusMap[yamlFilename] = true;
			}
		}
	}

	template<typename HardwareType>
	void parseYamlFile(std::map<std::string, HardwareType> &hardwareMapToFill)
	{
		std::ifstream fileInput;
		YAML::Node config;
		YAML::Node configTemplate;
		std::map<std::string, std::string> parameters;
		try
		{
			fileInput = std::ifstream(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			YAML::Parser parser(fileInput);
			config = YAML::LoadFile(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			if (config.size() > 0)
			{
				std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + SEPARATOR + config["properties"]["hardware_type"].as<std::string>() + ".yaml";
				configTemplate = YAML::LoadFile(hardwareTemplateFilename);
				if (!checkForValidTemplate(configTemplate, config))
				{
					throw YAML::BadFile();
				}
				auto pvAndRecordPair = extractControlsInformationIntoPair(config);
				auto hardwareParameterMap = extractHardwareInformationIntoMap(config);
				parameters.insert(pvAndRecordPair);
				for (auto prop : hardwareParameterMap)
				{
					parameters.insert(prop);
				}
				HardwareType freshHardware = HardwareType(parameters, loadVirtualHardware);
				hardwareMapToFill.emplace(freshHardware.getFullPVName(), HardwareType(parameters, loadVirtualHardware));
				//return parameters;
			}
			else
			{

				throw std::length_error("File contents were of length " + std::to_string(config.size()) + ", file must be empty!");
				//return parameters;
			}
		}
		// POTENTIAL EXCEPTIONS //
		catch (std::length_error EmptyFileException)
		{
			std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
			//return parameters;
		}
		catch (YAML::BadFile BadFileException)
		{
			std::cout << "Could not find file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "), or file is not compliant with template "
				<< ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::hardwareFolder << ".yaml" << "\n";
			//return parameters;
		}
		catch (YAML::ParserException EmptyFileException)
		{
			std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
			//return parameters;
		}
		catch (YAML::BadConversion ConvervsionException)
		{
			std::cout << ConvervsionException.what() << std::endl;
		}
	}
};
#endif //CONFIG_READER_H_