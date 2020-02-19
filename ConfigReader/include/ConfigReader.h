#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include <LoggingSystem.h>
#include <Magnet.h>
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
	bool checkForValidTemplate(const YAML::Node& hardwareTemplate, const YAML::Node& hardwareComponent) const;
	std::vector<std::string> findYAMLFilesInDirectory(const std::string& version);
	void initialiseFilenameAndParsedStatusMap();
	const std::pair<std::string, std::string> extractControlsInformationIntoPair(const YAML::Node& controlsInformationNode) const;
	const std::map<std::string, std::string> extractHardwareInformationIntoMap(const YAML::Node& hardwareInformationNode) const;
	bool hasMoreFilesToParse() const;


	template<typename HardwareType>
	void parseNextYamlFile(std::map<std::string, HardwareType>& hardwareMapToFill)
	{
		std::cout << "parseNextYamlFile() called " << std::endl;
		for (auto filename : this->yamlFilenamesAndParsedStatusMap)
		{
			// boolean check here for safety, even though we remove all parsed files
			// anyway, just didn't trust myself..

			std::cout << "parseNextYamlFile() called " << std::endl;
			if (!filename.second)
			{
				yamlFilename = filename.first;
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
			std::cout << "parseYamlFile try " << std::endl;
			fileInput = std::ifstream(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			std::cout << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << std::endl;
			YAML::Parser parser(fileInput);
			std::cout << "YAML::Parser parser(fileInput); " << std::endl;
			config = YAML::LoadFile(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
			std::cout << "got config " << std::endl;
			if (config.size() > 0)
			{
				std::cout << "config.size() > 0" << std::endl;
				std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + SEPARATOR + config["properties"]["hardware_type"].as<std::string>() + ".yaml";
				configTemplate = YAML::LoadFile(hardwareTemplateFilename);

				std::cout << "configTemplate Loaded;" << std::endl;

				if (!checkForValidTemplate(configTemplate, config))
				{

					std::cout << "throw YAML::BadFile();" << std::endl;
					throw YAML::BadFile();
				}
				std::cout << "Template is valid" << std::endl;

				auto hardwareParameterMap = extractHardwareInformationIntoMap(config);
				std::cout << "Got hardwareParameterMap" << std::endl;
				/*NEW FUNCTIONALITY ONLY IMPLMENTED FOR MAGNETS SO FAR */
				if (typeid(HardwareType) == typeid(Magnet) || typeid(HardwareType) == typeid(BPM) || typeid(HardwareType) == typeid(Charge) || typeid(HardwareType) == typeid(Screen))
				{
					std::cout << "HardwareType is Magnet or Valve or BPM or Charge or Screen;" << std::endl;
					auto recordsMap = extractRecordsIntoMap(config);
					parameters.insert(recordsMap.begin(), recordsMap.end());
					std::cout << "inserted recordsMap" << std::endl;
				}
				/*IF NOT A MAGNET, USE OLD FUNCTIONALITY*/
				else
				{
					auto pvAndRecordPair = extractControlsInformationIntoPair(config);
					std::cout << "Got Controls info" << std::endl;
					parameters.insert(pvAndRecordPair);
				}
				for (auto prop : hardwareParameterMap)
				{
					parameters.insert(prop);
				}
				HardwareType freshHardware = HardwareType(parameters, mode);

				// fill map via [] operator to construct IN-PLACE
				// if we use emplace/insert, the default constructor is called for the object
				// and HardwareType is set up with default constructor, instead of our params.

				
				hardwareMapToFill[freshHardware.getHardwareName()] = freshHardware;

				std::cout << "name  = " << freshHardware.getHardwareName() << ", mode = "
					<< ENUM_TO_STRING(mode) << std::endl;

				for (auto&& item : hardwareMapToFill)
				{
					std::cout << item.first << std::endl;
					std::cout << ENUM_TO_STRING(item.second.getMode()) << std::endl;

				}
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
		}
		catch (YAML::BadFile BadFileException)
		{
			messenger.printMessage("Could not find file (" + ConfigReader::yamlFileDestination, SEPARATOR,
				ConfigReader::yamlFilename, ")  or file is not compliant with template ",
				ConfigReader::yamlFileDestination, SEPARATOR, ConfigReader::hardwareFolder, ".yaml");
		}
		catch (YAML::ParserException EmptyFileException)
		{
			messenger.printMessage("Problem with file (" +
				ConfigReader::yamlFileDestination, SEPARATOR, ConfigReader::yamlFilename +
				"): " + std::string(EmptyFileException.what()));
		}
		catch (YAML::BadConversion ConvervsionException)
		{
			messenger.printMessage(std::string(ConvervsionException.what()));
		}
	}
};
#endif //CONFIG_READER_H_

//#ifndef CONFIG_READER_H_
//#define CONFIG_READER_H_
//
//#include "yaml-cpp/parser.h"
//#include "yaml-cpp/yaml.h"
//#include <LoggingSystem.h>
//#include <Magnet.h>
//#include <string>
//#include <fstream>
//#include <iostream>
//#include <map>
//
//#if defined(__unix__) ||  defined(_unix)
//const std::string HOME = getenv("HOME");
//const std::string MASTER_LATTICE_FILE_LOCATION = HOME + "/MasterLattice";
//const std::string SEPARATOR = "/";
//#endif
//#ifdef _WIN32
//const std::string MASTER_LATTICE_FILE_LOCATION = "\\\\apclara1\\ControlRoomApps\\Controllers\\bin\\CATAP 2.0 Tools and Libraries\\MasterLatticeYAMLFiles";
//const std::string SEPARATOR = "\\";
//#endif
//
//class ConfigReader
//{
//public:
//
//	ConfigReader(const std::string& hardwareType, const STATE& mode);
//	ConfigReader();
//
//	std::string yamlFileDestination;
//	std::string yamlFilename;
//	std::string hardwareFolder;
//	STATE mode;
//	std::map<std::string, bool> yamlFilenamesAndParsedStatusMap;
//	int numberOfParsesExpected;
//	// defining the allowed hardware types and their EPICS abbreviations
//	// these are currently hard-coded, we should get the folder names from
//	// MasterLattice directory to initialize the map
//	const static std::map<std::string, std::string> allowedHardwareTypes;
//
//
//	LoggingSystem messenger;
//	void debugMessagesOn();
//	void debugMessagesOff();
//	void messagesOn();
//	void messagesOff();
//	bool isMessagingOn();
//	bool isDebugOn();
//
//	std::string getHardwareTypeFromName(const std::string& fullPVName) const;
//	bool checkForValidTemplate(const YAML::Node& hardwareTemplate, const YAML::Node& hardwareComponent) const;
//	std::vector<std::string> findYAMLFilesInDirectory(const std::string& version);
//	void initialiseFilenameAndParsedStatusMap();
//	const std::pair<std::string, std::string> extractControlsInformationIntoPair(const YAML::Node& controlsInformationNode) const;
//
//	const std::map<std::string, std::string> extractHardwareInformationIntoMap(const YAML::Node& hardwareInformationNode) const;
//	bool hasMoreFilesToParse() const;
//
//	const std::pair<std::string, std::map<std::string, std::string>> extractPVRecordMap(const YAML::Node& configInformationNode) const;
//
//	template<typename HardwareType>
//	void parseNextYamlFile(std::map<std::string, HardwareType>& hardwareMapToFill)
//	{
//		std::cout << "parseNextYamlFile() called " << std::endl;
//		for (auto filename : this->yamlFilenamesAndParsedStatusMap)
//		{
//			// boolean check here for safety, even though we remove all parsed files
//			// anyway, just didn't trust myself..
//
//			std::cout << "parseNextYamlFile() called " << std::endl;
//			if (!filename.second)
//			{
//				yamlFilename = filename.first;
//				parseYamlFile(hardwareMapToFill);
//				yamlFilenamesAndParsedStatusMap[yamlFilename] = true;
//			}
//		}
//	}
//
//	template<typename HardwareType>
//	void parseYamlFile(std::map<std::string, HardwareType>& hardwareMapToFill)
//	{
//		std::cout << "parseYamlFile()" << std::endl;
//		std::ifstream fileInput;
//		YAML::Node config;
//		YAML::Node configTemplate;
//		std::map<std::string, std::string> parameters;
//		try
//		{
//			std::cout << "try" << std::endl;
//			fileInput = std::ifstream(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
//			YAML::Parser parser(fileInput);
//			config = YAML::LoadFile(ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename);
//			if (config.size() > 0)
//			{
//				std::cout << "try 1" << std::endl;
//				std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + SEPARATOR + config["properties"]["hardware_type"].as<std::string>() + ".yaml";
//				configTemplate = YAML::LoadFile(hardwareTemplateFilename);
//				if (!checkForValidTemplate(configTemplate, config))
//				{
//					throw YAML::BadFile();
//				}
//
//				// TODO i think this is the old method and should be phased out 
//				// THIS HAS TO GO, TO HANDLE ALL POTENTIAL WEIRDNESS WE NEED TO EXPLICITLY DEFINE EACH PV IN A MAP IN THE CONFIG
//				//  PLUS hardware should store them in a pvstructs map KEYED by ENUMS not strings
//				auto pvAndRecordPair = extractControlsInformationIntoPair(config);
//				parameters.insert(pvAndRecordPair);
//				std::cout << "try 2" << std::endl;
//				std::cout << "calling extractPVRecordMap" << std::endl;
//				std::pair<std::string, std::map<std::string, std::string>> pvRecordMap  = extractPVRecordMap(config);
//				// now we have to loop over each item in map and add it to paramters map, 
//				for(auto&& item : pvRecordMap.second)
//				{
//					std::pair<std::string, std::string>  pv_record_item;
//					pv_record_item.first = item.first;
//					pv_record_item.second = item.second;
//					std::cout << "Adding " << pv_record_item.first << " = " << pv_record_item.second << std::endl;
//					parameters.insert(pv_record_item);
//				}
//
//				auto hardwareParameterMap = extractHardwareInformationIntoMap(config);
//				for (auto prop : hardwareParameterMap)
//				{
//					parameters.insert(prop);
//				}
//				HardwareType freshHardware = HardwareType(parameters, mode);
//
//				// fill map via [] operator to construct IN-PLACE
//				// if we use emplace/insert, the default constructor is called for the object
//				// and HardwareType is set up with default constructor, instead of our params.
//				hardwareMapToFill[freshHardware.getHardwareName()] = freshHardware;
//
//				std::cout << "name  = " << freshHardware.getHardwareName() << ", mode = "
//					<< ENUM_TO_STRING(mode) << std::endl;
//
//				for (auto&& item : hardwareMapToFill)
//				{
//					std::cout << item.first << std::endl;
//					std::cout << ENUM_TO_STRING(item.second.getMode()) << std::endl;
//				}
//			}
//			else
//			{
//				std::cout << "config.size() > 0 is FALSE" << std::endl;
//				throw std::length_error("File contents were of length " + std::to_string(config.size()) + ", file must be empty!");
//			}
//		}
//		// POTENTIAL EXCEPTIONS //
//		catch (std::length_error EmptyFileException)
//		{
//			std::cout << "EmptyFileException" << std::endl;
//
//			messenger.printMessage("Problem with file (" + ConfigReader::yamlFileDestination,
//				SEPARATOR, ConfigReader::yamlFilename + "): " + std::string(EmptyFileException.what()));
//		}
//		catch (YAML::BadFile BadFileException)
//		{
//			std::cout << "BadFileException" << std::endl;
//
//			messenger.printMessage("Could not find file (" + ConfigReader::yamlFileDestination, SEPARATOR,
//				ConfigReader::yamlFilename, ")  or file is not compliant with template ",
//				ConfigReader::yamlFileDestination, SEPARATOR, ConfigReader::hardwareFolder, ".yaml");
//		}
//		catch (YAML::ParserException EmptyFileException)
//		{
//			std::cout << "EmptyFileException" << std::endl;
//			messenger.printMessage("Problem with file (" +
//				ConfigReader::yamlFileDestination, SEPARATOR, ConfigReader::yamlFilename +
//				"): " + std::string(EmptyFileException.what()));
//		}
//		catch (YAML::BadConversion ConvervsionException)
//		{
//			std::cout << "ConvervsionException" << std::endl;
//
//			messenger.printMessage(std::string(ConvervsionException.what()));
//		}
//	}
//};
//#endif //CONFIG_READER_H_