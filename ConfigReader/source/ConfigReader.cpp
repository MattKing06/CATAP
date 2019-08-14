#include "ConfigReader.h"
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "yaml-cpp/exceptions.h"

#if defined(__unix__) ||  defined(_unix)
const std::string HOME = getenv("HOME");
const std::string MASTER_LATTICE_FILE_LOCATION = HOME + "/MasterLattice";
const std::string SEPARATOR = "/";
#endif
#ifdef _WIN32
const std::string MASTER_LATTICE_FILE_LOCATION = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\";
const std::string SEPARATOR = "\\";
#endif

const std::map<std::string, std::string> ConfigReader::allowedHardwareTypes = {
	{ "MAG", "Magnet" },
	{ "BPM", "Beam Position Monitor" }
};


ConfigReader::ConfigReader() : yamlFileDestination(MASTER_LATTICE_FILE_LOCATION), yamlFilename(""), loadVirtualHardware(false), hardwareFolder("")
{
	// since we have not specified a hardware component
	// we assume that we want to load all hardware yaml files.
	// So we set up the directory of the master lattice files, and nothing else.
	initialiseFilenameAndParsedStatusMap();
}

ConfigReader::ConfigReader(const std::string &hardwareType, const bool &isVirtual) : loadVirtualHardware(isVirtual),
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

std::vector<std::string> ConfigReader::findYAMLFilesInDirectory(std::string version)
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

const std::string ConfigReader::getHardwareTypeFromName(const std::string &fullPVName)
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

const bool ConfigReader::checkForValidTemplate(const YAML::Node &hardwareTemplate, const YAML::Node &hardwareComponent)
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

std::map<std::string, std::string> ConfigReader::parseNextYamlFile()
{
	for (const auto filename : yamlFilenamesAndParsedStatusMap)
	{
		// boolean check here for safety, even though we remove all parsed files
		// anyway, just didn't trust myself..
		if (!filename.second)
		{
			yamlFilename = filename.first;
			yamlFilenamesAndParsedStatusMap.erase(filename.first);
			return parseYamlFile();
		}
	}
}

bool ConfigReader::hasMoreFilesToParse()
{
	return !yamlFilenamesAndParsedStatusMap.empty();
}

const std::map<std::string, std::string> ConfigReader::parseYamlFile()
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
			return parameters;
		}
		else
		{

			throw std::length_error("File contents were of length " + std::to_string(config.size()) + ", file must be empty!");
			return parameters;
		}
	}
	// POTENTIAL EXCEPTIONS //
	catch (std::length_error EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return parameters;
	}
	catch (YAML::BadFile BadFileException)
	{
		std::cout << "Could not find file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "), or file is not compliant with template "
			<< ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::hardwareFolder << ".yaml" << "\n";
		return parameters;
	}
	catch (YAML::ParserException EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + SEPARATOR + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return parameters;
	}
	catch (YAML::BadConversion ConvervsionException)
	{
		std::cout << ConvervsionException.what() << std::endl;
	}
}

const std::map<std::string, std::string> ConfigReader::extractHardwareInformationIntoMap(const YAML::Node &configInformationNode)
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

const std::pair<std::string, std::string> ConfigReader::extractControlsInformationIntoPair(const YAML::Node &configInformationNode)
{
	YAML::Node controlsInformation = configInformationNode["controls_information"];
	std::map<std::string, std::string> controlsParameterMap;
	if (controlsInformation["PV"].as<bool>())
	{
		std::string controlRecords = controlsInformation["records"].as<std::string>();
		boost::trim_left(controlRecords);
		std::pair<std::string, std::string> pvAndRecordPair;
		if (this->loadVirtualHardware)
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