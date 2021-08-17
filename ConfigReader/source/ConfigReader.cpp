#include "ConfigReader.h"
#include <utility>
#include <vector>
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "yaml-cpp/exceptions.h"


ConfigReader::ConfigReader():
ConfigReader("", STATE::OFFLINE, MASTER_LATTICE_FILE_LOCATION)
{}

ConfigReader::ConfigReader(const std::string& hardwareType, const STATE& mode) :
ConfigReader(hardwareType, mode, MASTER_LATTICE_FILE_LOCATION)
{}

ConfigReader::ConfigReader(const std::string& hardwareType, const STATE& mode, const std::string& primeLatticeLocation) :
	messenger(LoggingSystem(true, true)),
	mode(mode),
	hardwareFolder(hardwareType)
{
	messenger.printDebugMessage("ConfigReader( " + hardwareType + ", " + ENUM_TO_STRING(mode) + ") Constructor called");
	if (primeLatticeLocation.empty())
	{
		yamlFileDestination = MASTER_LATTICE_FILE_LOCATION + SEPARATOR + hardwareFolder;
		initialiseFilenameAndParsedStatusMap();
	}
	else
	{
		yamlFileDestination = primeLatticeLocation + SEPARATOR + hardwareFolder;
		initialiseFilenameAndParsedStatusMap();
	}

}

void ConfigReader::initialiseFilenameAndParsedStatusMap()
{
	//messenger.printDebugMessage("ConfigReader called initialiseFilenameAndParsedStatusMap");
	std::vector<std::string> filenamesInDirectory = findYAMLFilesInDirectory("");
	std::string templateFilename = hardwareFolder + ".yaml";
	messenger.printDebugMessage("ConfigReader templateFilename: " + templateFilename);
	std::string all_file_names = "";
	for (const auto& filename : filenamesInDirectory)
	{
		if (filename != templateFilename)
		{
			yamlFilenamesAndParsedStatusMap.emplace(std::pair<std::string, bool>(filename, false));
			all_file_names += filename + " ";
		}
	}
	messenger.printDebugMessage("ConfigReader found these files: " + all_file_names);
}

std::vector<std::string> ConfigReader::findYAMLFilesInDirectory(const std::string& version)
{
	boost::filesystem::path directory(yamlFileDestination);//+ '//' + version);
	std::vector<std::string> filenames;
	for (auto i = boost::filesystem::directory_iterator(directory); i != boost::filesystem::directory_iterator(); i++)
	{
		if (!boost::filesystem::is_directory(i->path()))
		{
			if (i->path().extension() == ".YAML"
				|| i->path().extension() == ".YML"
				|| i->path().extension() == ".yaml"
				|| i->path().extension() == ".yml")
			{
				
				filenames.push_back(i->path().filename().string());
				messenger.printDebugMessage("ConfigReader found: " + filenames.back());
				if (i->path().filename().string() != hardwareFolder)
				{
					numberOfParsesExpected++;
				}
			}
		}
		else
		{
			continue;
		}
	}
	return filenames;
}


std::vector<std::string> ConfigReader::compareFileWithTemplate(const YAML::Node& hardwareTemplate,
	const YAML::Node& hardwareComponent) const
{
	// this vector will contain any keys from the template that are not present in the config file.
	std::vector<std::string> missingEntries;
	// flags so if an error is found print info from file to help fix it 
	bool print_properties = false;
	bool print_ctrlsinfo = false;
	for (const auto& keyAndValuePair : hardwareTemplate["properties"])
	{
		if (!hardwareComponent["properties"][keyAndValuePair.first.as<std::string>()])
		{
			messenger.printDebugMessage("Error missing property: " + keyAndValuePair.first.as<std::string>());
			missingEntries.push_back(keyAndValuePair.first.as<std::string>());
			//return false;
			print_properties = true;
		}
	}
	for (const auto& keyAndValuePair : hardwareTemplate["controls_information"])
	{
		if (!hardwareComponent["controls_information"][keyAndValuePair.first.as<std::string>()])
		{
			missingEntries.push_back(keyAndValuePair.first.as<std::string>());
			print_ctrlsinfo = true;
			//return false;
		}
	}
	if (print_properties)
	{
		messenger.printDebugMessage("property Errors found, printing all discovered properties ");		
		for (const auto& keyAndValuePair : hardwareComponent["properties"])
		{
			messenger.printDebugMessage(keyAndValuePair.first.as<std::string>() + " = " +
				keyAndValuePair.second.as<std::string>());
		}
	}
	if (print_ctrlsinfo)
	{
		messenger.printDebugMessage("controls_information Errors found, printing all discovered controls_information ");
		for (const auto& keyAndValuePair : hardwareComponent["controls_information"])
		{
			messenger.printDebugMessage(keyAndValuePair.first.as<std::string>() + " = " +
				keyAndValuePair.second.as<std::string>());
		}
	}
	return missingEntries;
}

bool ConfigReader::hasMoreFilesToParse() const
{
	for (const auto& file : yamlFilenamesAndParsedStatusMap)
	{
		messenger.printDebugMessage(file.first);
		if (file.second)
		{
			messenger.printDebugMessage(file.first, " parsed ");
			continue;
		}
		if (!file.second)
		{
			messenger.printDebugMessage(file.first, " NOT parsed ");
			return true;
		}
	}
	return false;
}

void ConfigReader::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("CONFIG-READER -", "DEBUG ON");
}

void ConfigReader::debugMessagesOff()
{
	messenger.printDebugMessage("CONFIG-READER -", "DEBUG OFF");
	messenger.debugMessagesOff();
}

void ConfigReader::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("CONFIG-READER - MESSAGES ON");
}

void ConfigReader::messagesOff()
{
	messenger.printMessage("CONFIG-READER - MESSAGES OFF");
	messenger.messagesOff();
}

bool ConfigReader::isMessagingOn()
{
	return messenger.isMessagingOn();
}

bool ConfigReader::isDebugOn()
{
	return messenger.isDebugOn();
}

const std::map<std::string, std::string> ConfigReader::extractRecordsIntoMap(const YAML::Node& configInformationNode) const
{
	auto controlsInformationNode = configInformationNode["controls_information"];
	std::map<std::string, std::string> pvRecordMap = controlsInformationNode["pv_record_map"].as<std::map<std::string, std::string> >();
	// for debugging print data in file 
	//for (auto&& it : pvRecordMap)
	//{
	//	messenger.printDebugMessage(it.first + " = " + it.second);
	//}
	return pvRecordMap;
}


const std::map<std::string, std::string> ConfigReader::extractHardwareInformationIntoMap(const YAML::Node& configInformationNode) const
{
	auto hardwareProperties = configInformationNode["properties"];

	std::map<std::string, std::string> hardwarePropertyAndValueVector;
	if (hardwareProperties.IsDefined())
	{
		for (auto properties : hardwareProperties)
		{
			std::string key = properties.first.as<std::string>();
			std::string value;
			if (properties.second.Type() == YAML::NodeType::Scalar)
			{
				value = properties.second.as<std::string>();
			}
			else
			{
				value = "UNDEFINED";
			}
			hardwarePropertyAndValueVector.insert(std::make_pair(key, value));
		}

	}
	// for debugging print data in file 
	//for (auto&& it : hardwarePropertyAndValueVector)
	//{
	//	messenger.printDebugMessage(it.first + " = " + it.second);
	//}
	return hardwarePropertyAndValueVector;
}

const std::pair<std::string, std::string> ConfigReader::extractControlsInformationIntoPair(const YAML::Node& configInformationNode) const
{
	YAML::Node controlsInformation = configInformationNode["controls_information"];
	std::map<std::string, std::string> controlsParameterMap;
	if (controlsInformation["PV"].as<bool>())
	{
		std::string controlRecords = controlsInformation["records"].as<std::string>();
		boost::trim_left(controlRecords);
		std::pair<std::string, std::string> pvAndRecordPair;

		// mode tells us if we are physical, virtual or offline
		// which tells us what the
		if (mode == STATE::VIRTUAL)
		{
			pvAndRecordPair = std::make_pair(configInformationNode["properties"]["virtual_name"].as<std::string>(), controlRecords);
		}
		else if (mode == STATE::PHYSICAL)
		{
			pvAndRecordPair = std::make_pair(configInformationNode["properties"]["name"].as<std::string>(), controlRecords);
		}
		//else
		//{
		//	mode = STATE::OFFLINE;
		//}
		return pvAndRecordPair;
	}
	else
	{
		// not comfortable returning empty pair.
		// Makes sense because we have no control info for this hardware
		// but should really be handled better maybe??
		return std::pair<std::string, std::string>();
	}
}

