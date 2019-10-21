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
const std::string MASTER_LATTICE_FILE_LOCATION = "\\\\apclara1\\ControlRoomApps\\Controllers\\bin\\CATAP 2.0 Tools and Libraries\\MasterLatticeYAMLFiles";
const std::string SEPARATOR = "\\";
#endif

class ConfigReader
{
public:
	std::string yamlFileDestination;
	std::string yamlFilename;
	std::string hardwareFolder;
	bool isVirtual;
	std::map<std::string, bool> yamlFilenamesAndParsedStatusMap;
	int numberOfParsesExpected;
	// defining the allowed hardware types and their EPICS abbreviations
	// these are currently hard-coded, we should get the folder names from
	// MasterLattice directory to initialize the map
	const static std::map<std::string, std::string> allowedHardwareTypes;

	ConfigReader(const std::string &hardwareType, const bool &isVirtual);
	ConfigReader();
	std::string getHardwareTypeFromName(const std::string &fullPVName) const;
	bool checkForValidTemplate(const YAML::Node &hardwareTemplate, const YAML::Node &hardwareComponent) const;
	std::vector<std::string> findYAMLFilesInDirectory(const std::string &version);
	void initialiseFilenameAndParsedStatusMap();
	const std::pair<std::string, std::string> extractControlsInformationIntoPair(const YAML::Node &controlsInformationNode) const;
	const std::map<std::string, std::string> extractHardwareInformationIntoMap(const YAML::Node &hardwareInformationNode) const;
	bool hasMoreFilesToParse() const;
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
				parseYamlFile(hardwareMapToFill);
				yamlFilenamesAndParsedStatusMap[yamlFilename] = true;
			}
		}
	}

	template<typename HardwareType>
	void parseYamlFile(std::map<std::string, HardwareType> &hardwareMapToFill) const
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
				HardwareType freshHardware = HardwareType(parameters, isVirtual);
				// fill map via [] operator to construct IN-PLACE
				// if we use emplace/insert, the default constructor is called for the object
				// and HardwareType is set up with default constructor, instead of our params.
				hardwareMapToFill[freshHardware.getHardwareName()] = freshHardware;
			}
			else
			{
				throw std::length_error("File contents were of length " + std::to_string(config.size()) + ", file must be empty!");
			}
		}
		// POTENTIAL EXCEPTIONS //
		catch (std::length_error EmptyFileException)
		{
			std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		}
		catch (YAML::BadFile BadFileException)
		{
			std::cout << "Could not find file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "), or file is not compliant with template "
				<< ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::hardwareFolder << ".yaml" << "\n";
		}
		catch (YAML::ParserException EmptyFileException)
		{
			std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		}
		catch (YAML::BadConversion ConvervsionException)
		{
			std::cout << ConvervsionException.what() << std::endl;
		}
	}
};
#endif //CONFIG_READER_H_