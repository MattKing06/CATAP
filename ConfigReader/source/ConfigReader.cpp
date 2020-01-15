#include "ConfigReader.h"
#include <utility>
#include <vector>
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "yaml-cpp/exceptions.h"

const std::map<std::string, std::string> ConfigReader::allowedHardwareTypes = {
	{ "MAG", "Magnet" },
	{ "BPM", "Beam Position Monitor" }
};

ConfigReader::ConfigReader() : yamlFileDestination(MASTER_LATTICE_FILE_LOCATION), yamlFilename(""), isVirtual(false), hardwareFolder("")
{
	// since we have not specified a hardware component
	// we assume that we want to load all hardware yaml files.
	// So we set up the directory of the master lattice files, and nothing else.
	initialiseFilenameAndParsedStatusMap();
}

ConfigReader::ConfigReader(const std::string &hardwareType, const bool &isVirtual) : isVirtual(isVirtual),
hardwareFolder(hardwareType)
{
	yamlFileDestination = MASTER_LATTICE_FILE_LOCATION + SEPARATOR + hardwareFolder;
	initialiseFilenameAndParsedStatusMap();

}

void ConfigReader::initialiseFilenameAndParsedStatusMap()
{
	std::vector<std::string> filenamesInDirectory = findYAMLFilesInDirectory("");
	std::string templateFilename = hardwareFolder + ".yaml";
	for (const auto& filename : filenamesInDirectory)
	{
		if (filename != templateFilename)
		{
			yamlFilenamesAndParsedStatusMap.emplace(std::pair<std::string, bool>(filename, false));
		}

	}
}

std::vector<std::string> ConfigReader::findYAMLFilesInDirectory(const std::string &version)
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
				if (i->path().filename().string() != hardwareFolder)
				{
					numberOfParsesExpected++;
				}
			}
			else
			{
				std::cout << i->path().filename().string() << ": NOT YAML" << std::endl;
			}
		}
		else
		{
			continue;
		}
	}
	return filenames;
}

std::string ConfigReader::getHardwareTypeFromName(const std::string &fullPVName) const
{
	for (const auto &hardwareType : this->allowedHardwareTypes)
	{
		if (fullPVName.find(hardwareType.first) != std::string::npos)
		{
			return hardwareType.second;
		}
	}
	throw std::runtime_error{ "Could not find allowed hardware type for: "
		+ fullPVName + ", " +
		" Please check the PV name or contact support." };
}

bool ConfigReader::checkForValidTemplate(const YAML::Node &hardwareTemplate,
										 const YAML::Node &hardwareComponent) const
{
	for (const auto &keyAndValuePair : hardwareTemplate["properties"])
	{
		if (!hardwareComponent["properties"][keyAndValuePair.first.as<std::string>()])
		{
			return false;
		}
	}
	for (const auto &keyAndValuePair : hardwareTemplate["controls_information"])
	{
		if (!hardwareComponent["controls_information"][keyAndValuePair.first.as<std::string>()])
		{
			return false;
		}
	}
	return true;
}

bool ConfigReader::hasMoreFilesToParse() const
{
	for (const auto &file : yamlFilenamesAndParsedStatusMap)
	{
		if (file.second)
		{
			continue;
		}
		if (!file.second)
		{
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

const std::map<std::string, std::string> ConfigReader::extractHardwareInformationIntoMap(const YAML::Node &configInformationNode) const
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
	return hardwarePropertyAndValueVector;
}

const std::pair<std::string, std::string> ConfigReader::extractControlsInformationIntoPair(const YAML::Node &configInformationNode) const
{
	YAML::Node controlsInformation = configInformationNode["controls_information"];
	std::map<std::string, std::string> controlsParameterMap;
	if (controlsInformation["PV"].as<bool>())
	{
		std::string controlRecords = controlsInformation["records"].as<std::string>();
		boost::trim_left(controlRecords);
		std::pair<std::string, std::string> pvAndRecordPair;
		if (this->isVirtual)
		{
			pvAndRecordPair = std::make_pair(configInformationNode["properties"]["virtual_name"].as<std::string>(), controlRecords);
		}
		else
		{
			pvAndRecordPair = std::make_pair(configInformationNode["properties"]["name"].as<std::string>(), controlRecords);
		}
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