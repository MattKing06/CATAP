#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include <ConfigReaderExceptions.h>
#include <LoggingSystem.h>
#include <Magnet.h>
#include <Valve.h>
#include <BPM.h>
#include <Charge.h>
#include <Screen.h>
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
const std::string MASTER_LATTICE_FILE_LOCATION = "\\\\apclara1.dl.ac.uk\\ControlRoomApps\\Controllers\\bin\\CATAP 2.0 Tools and Libraries\\MasterLatticeYAMLFiles";
const std::string SEPARATOR = "\\";
#endif
/*! @defgroup configreader ConfigReader*/
/**@{*/
class ConfigReader
{
public:

	ConfigReader(const std::string& hardwareType, const STATE& mode);
	ConfigReader();

	std::string yamlFileDestination;
	std::string yamlFilename;
	std::string hardwareFolder;
	STATE mode;
	std::map<std::string, bool> yamlFilenamesAndParsedStatusMap;
	int numberOfParsesExpected;
	// defining the allowed hardware types and their EPICS abbreviations
	// these are currently hard-coded, we should get the folder names from
	// MasterLattice directory to initialize the map
	const static std::map<std::string, std::string> allowedHardwareTypes;


	LoggingSystem messenger;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();

	const std::map <std::string, std::string> extractRecordsIntoMap(const YAML::Node& configInformationNode) const;

	std::string getHardwareTypeFromName(const std::string& fullPVName) const;
	std::vector<std::string> compareFileWithTemplate(const YAML::Node& hardwareTemplate, const YAML::Node& hardwareComponent) const;
	std::vector<std::string> findYAMLFilesInDirectory(const std::string& version);
	void initialiseFilenameAndParsedStatusMap();
	const std::pair<std::string, std::string> extractControlsInformationIntoPair(const YAML::Node& controlsInformationNode) const;
	const std::map<std::string, std::string> extractHardwareInformationIntoMap(const YAML::Node& hardwareInformationNode) const;
	bool hasMoreFilesToParse() const;


	template<typename HardwareType>
	void parseNextYamlFile(std::map<std::string, HardwareType>& hardwareMapToFill)
	{
		for (auto filename : this->yamlFilenamesAndParsedStatusMap)
		{
			// boolean check here for safety, even though we remove all parsed files
			// anyway, just didn't trust myself..
			if (!filename.second)
			{
				yamlFilename = filename.first;
				//messenger.printDebugMessage("parseNextYamlFile is trying to parse ", yamlFilename);
				parseYamlFile(hardwareMapToFill);
				yamlFilenamesAndParsedStatusMap[yamlFilename] = true;
			}
		}
	}

	template<typename HardwareType>
	void parseYamlFile(std::map<std::string, HardwareType>& hardwareMapToFill)
	{
		std::ifstream fileInput;
		YAML::Node config;
		YAML::Node configTemplate;
		std::map<std::string, std::string> parameters;
		try
		{
			fileInput = std::ifstream(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			YAML::Parser parser(fileInput);
			messenger.printDebugMessage("Calling LoadFile");
			config = YAML::LoadFile(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			if (config.size() > 0)
			{
				messenger.printDebugMessage("LoadFile got config data, getting template");
				std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + SEPARATOR + config["properties"]["hardware_type"].as<std::string>() + ".yaml";
				/*****HARDCODED .yaml WHEN CONFIG FILE HAD EXTENSION .yml CAUSED FAIL TO LOAD TEMPLATE******/
				configTemplate = YAML::LoadFile(hardwareTemplateFilename);

				messenger.printDebugMessage("LoadFile got template");


				std::vector<std::string> missingEntriesFromFile = compareFileWithTemplate(configTemplate, config);
				if (!missingEntriesFromFile.empty())
				{
					throw InvalidFileException(ConfigReader::yamlFilename, missingEntriesFromFile);
				}

				auto hardwareParameterMap = extractHardwareInformationIntoMap(config);
				auto recordsMap = extractRecordsIntoMap(config);
				parameters.insert(recordsMap.begin(), recordsMap.end());
				parameters.insert(hardwareParameterMap.begin(), hardwareParameterMap.end());
				HardwareType freshHardware = HardwareType(parameters, mode);

				// fill map via [] operator to construct IN-PLACE
				// if we use emplace/insert, the default constructor is called for the object
				// and HardwareType is set up with default constructor, instead of our params.
				hardwareMapToFill[freshHardware.getHardwareName()] = freshHardware;

/* 				std::cout << "name  = " << freshHardware.getHardwareName() << ", mode = "
					<< ENUM_TO_STRING(mode) << std::endl;
 */
			}
			else
			{
				throw std::length_error("File contents were of length " + std::to_string(config.size()) + ", file must be empty!");
			}
		}
		// POTENTIAL EXCEPTIONS //
		catch (std::length_error EmptyFileException)
		{
			messenger.printMessage("Problem with file (" + ConfigReader::yamlFileDestination,
				SEPARATOR, ConfigReader::yamlFilename + "): " + std::string(EmptyFileException.what()));
			exit(0);
		}
		catch (InvalidFileException InvalidFileException)
		{
			InvalidFileException.printError();
		}
		catch (YAML::BadFile BadFileException)
		{
			messenger.printMessage("Could not find file (" + ConfigReader::yamlFileDestination, SEPARATOR,
				ConfigReader::yamlFilename, ")  ");
			exit(0);
		}
		catch (YAML::ParserException EmptyFileException)
		{
			messenger.printMessage("Problem with file (" +
				ConfigReader::yamlFileDestination, SEPARATOR, ConfigReader::yamlFilename +
				"): " + std::string(EmptyFileException.what()));
			exit(0);
		}
		catch (YAML::BadConversion ConvervsionException)
		{
			messenger.printMessage("ConvervsionException " + std::string(ConvervsionException.what()));
			exit(0);
		} 
	}
};
/**@}*/
#endif //CONFIG_READER_H_
