#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include <boost/filesystem.hpp>
#include <ConfigReaderExceptions.h>
#include <LoggingSystem.h>
#include <Magnet.h>
#include <Valve.h>
#include <BPM.h>
#include <Charge.h>
#include <Screen.h>
#include <LaserEnergyMeter.h>
#include <Lighting.h>
#include <LaserHWP.h>
#include <IMG.h>
#include <RFProtection.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

const std::string MASTER_LATTICE_FILE_LOCATION = MASTER_LATTICE_LOCATION;

#if defined(__unix__) ||  defined(_unix)
const std::string SEPARATOR = "/";
#endif
#ifdef _WIN32

const std::string SEPARATOR = "\\";
#endif

/*! @defgroup configreader ConfigReader*/
/**@{*/
class ConfigReader
{
public:

	ConfigReader(const std::string& hardwareType, const STATE& mode);
	ConfigReader(const std::string& hardwareType, const STATE& mode, const std::string& primeLatticeLocation);
	ConfigReader();

	std::string yamlFileDestination;
	std::string yamlFilename;
	std::string hardwareFolder;
	STATE mode;
	std::map<std::string, bool> yamlFilenamesAndParsedStatusMap;
	int numberOfParsesExpected;
	std::map<std::string, std::string> offlineProperties;
	std::map<std::string, std::string> onlineProperties;
	bool doesLocationExist(const boost::filesystem::path& location);
	bool isEmptyDirectory(const boost::filesystem::path& location);

	LoggingSystem messenger;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();

	const std::map <std::string, std::string> extractRecordsIntoMap(const YAML::Node& configInformationNode) const;

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
				messenger.printDebugMessage("parseNextYamlFile is trying to parse ", yamlFilename);
				parseYamlFile(hardwareMapToFill);
				yamlFilenamesAndParsedStatusMap[yamlFilename] = true;
			}
		}
	}

	// TODO can we have some more information printed to help correct Master Lattice errors 
	template<typename HardwareType>
	void parseYamlFile(std::map<std::string, HardwareType>& hardwareMapToFill)
	{
		messenger.printDebugMessage("parseYamlFile");
		std::ifstream fileInput;
		YAML::Node config;
		YAML::Node configTemplate;
		std::map<std::string, std::string> parameters;
		try
		{
			std::string fn = ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename;
			messenger.printDebugMessage("fn = ", fn);
			fileInput = std::ifstream(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			YAML::Parser parser(fileInput);
			messenger.printDebugMessage("Calling LoadFile fn = " + fn);
			config = YAML::LoadFile(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			if (config.size() > 0)
			{
				//TODO: we should only load the config template once per factory set-up ? 
				messenger.printDebugMessage("LoadFile got config data, getting template");
				std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + SEPARATOR + config["properties"]["hardware_type"].as<std::string>() + ".yaml";
				// TODO fix  ".yaml" is required 
				/*****HARDCODED .yaml WHEN CONFIG FILE HAD EXTENSION .yml CAUSED FAIL TO LOAD TEMPLATE******/
				configTemplate = YAML::LoadFile(hardwareTemplateFilename);

				messenger.printDebugMessage("LoadFile got template, comparing ");


				std::vector<std::string> missingEntriesFromFile = compareFileWithTemplate(configTemplate, config);
				if (!missingEntriesFromFile.empty())
				{
					throw InvalidFileException(ConfigReader::yamlFilename, missingEntriesFromFile);
				}
				messenger.printDebugMessage("extractHardwareInformationIntoMap");
				offlineProperties = extractHardwareInformationIntoMap(config);
				messenger.printDebugMessage("extractRecordsIntoMap");
				onlineProperties = extractRecordsIntoMap(config);
				parameters.insert(onlineProperties.begin(), onlineProperties.end());
				parameters.insert(offlineProperties.begin(), offlineProperties.end());
				messenger.printDebugMessage("Constuct Hardware, mode = ", ENUM_TO_STRING(mode));
				HardwareType freshHardware = HardwareType(parameters, mode);
				freshHardware.setOnlineProperties(onlineProperties);
				freshHardware.setOfflineProperties(offlineProperties);
				messenger.printDebugMessage("Add New harwdare to hardwareMapToFill");
				// fill map via [] operator to construct IN-PLACE
				// if we use emplace/insert, the default constructor is called for the object
				// and HardwareType is set up with default constructor, instead of our params.
				hardwareMapToFill[freshHardware.getHardwareName()] = freshHardware;
				messenger.printDebugMessage("Added " + freshHardware.getHardwareName() + " to hardwareMapToFill"
				 + " current size = ", hardwareMapToFill.size());

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
		// TODO can we have some more infomration about what these are and maybe print some hints on 
		// how to fix the issue if it is raised 
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
