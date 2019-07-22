#include "ConfigReader.h"
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include "boost/algorithm/string.hpp"
#include "yaml-cpp/exceptions.h"

const std::map<std::string, std::string> ConfigReader::allowedHardwareTypes = {
	{ "MAG", "Magnet" },
	{ "BPM", "Beam Position Monitor" }
};


std::string ConfigReader::getHardwareTypeFromName(std::string fullPVName)
{
	for (auto type : this->allowedHardwareTypes)
	{
		if (fullPVName.find(type.first) != std::string::npos)
		{
			return type.second;
		}
	}
	return "UNDEFINED";
}

ConfigReader::ConfigReader(const std::string hardwareComponentName, bool isVirtual){
	#if defined(__unix__) ||  defined(_unix)
	  const std::string HOME =  getenv("HOME");
	  const std::string MASTER_LATTICE_FILE_LOCATION = HOME +"/MasterLattice";
	  hardwareFolder = getHardwareTypeFromName(hardwareComponentName);
	  yamlFileDestination = MASTER_LATTICE_FILE_LOCATION + "/" + hardwareFolder;
	  yamlFilename = hardwareComponentName + ".yaml";
	#endif
	#ifdef _WIN32
	  yamlFilename = hardwareComponentName + ".yaml";
	  hardwareFolder = getHardwareTypeFromName(hardwareComponentName);
	  yamlFileDestination = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles\\" + hardwareFolder;
	#endif
    loadVirtualHardware = isVirtual;
}

ConfigReader::ConfigReader()
{
	// since we have not specified a hardware component
	// we assume that we want to load all hardware yaml files.
	// So we set up the directory of the master lattice files, and nothing else.
	#if defined(__unix__) ||  defined(_unix)
	  const std::string HOME =  getenv("HOME");
	  const std::string MASTER_LATTICE_FILE_LOCATION = HOME + "/MasterLattice";
    #endif
	#ifdef _WIN32
		yamlFileDestination = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	#endif
	yamlFilename = "";
}

bool checkForValidTemplate(YAML::Node hardwareTemplate, YAML::Node hardwareComponent)
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

std::map<std::string, std::string> ConfigReader::parseYamlFile()
{	
	std::ifstream fileInput;
	YAML::Node config;
	YAML::Node configTemplate;
	#if defined(_unix_) || defined(_unix)
		const std::string separator = "/";
	#endif
    #ifdef _WIN32
	  const std::string separator = "\\";
	#endif
	// before we start, config reader is usually a member to the Hardware classes
	// and only gets initialized once, we need to reset PVs when parsing
	// otherwise we just keep adding values to the multimap.
	parameters.clear();
	try{
		std::cout << "SEPARATOR FOR UNIX: " << separator << std::endl;
		fileInput = std::ifstream(ConfigReader::yamlFileDestination + separator + ConfigReader::yamlFilename);
		YAML::Parser parser(fileInput);
		config = YAML::LoadFile(ConfigReader::yamlFileDestination + separator + ConfigReader::yamlFilename);
	}
	catch (YAML::BadFile BadFileException)
	{
		std::cout << "I could not find the file (" << ConfigReader::yamlFileDestination + separator + ConfigReader::yamlFilename << ") you were looking for..." << "\n";
		return parameters;
	}
	try
	{
		if (config.size() > 0)
		{
			std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + separator + config["properties"]["hardware_type"].as<std::string>() + ".yaml";
			configTemplate = YAML::LoadFile(hardwareTemplateFilename);
			if (!checkForValidTemplate(configTemplate, config))
			{
				throw YAML::BadFile();
			}
			// dealing with controls_information specifically
			auto controls_information = config["controls_information"];
			auto hardwareProperties = config["properties"];
			if (controls_information["PV"].as<bool>())
			{
				std::string control_records = controls_information["records"].as<std::string>();
				boost::trim_left(control_records);
				std::pair<std::string, std::string> pvAndRecordPair;
				if (this->loadVirtualHardware)
				{
					pvAndRecordPair = std::make_pair(config["properties"]["virtual_name"].as<std::string>(), control_records);
				}
				else
				{
					pvAndRecordPair = std::make_pair(config["properties"]["name"].as<std::string>(), control_records);
				}
				parameters.insert(pvAndRecordPair);
			}
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
			for (auto prop : hardwarePropertyAndValueVector)
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
	catch (std::length_error EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + separator + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return parameters;
	}
	catch (YAML::BadFile BadFileException)
	{
		std::cout << "File (" << ConfigReader::yamlFileDestination + separator + ConfigReader::yamlFilename << ") is not compliant with template "
				  << ConfigReader::yamlFileDestination + separator + ConfigReader::hardwareFolder << ".yaml" << "\n";
		return parameters;
	}
	catch (YAML::ParserException EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + separator + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return parameters;
	}
	catch (YAML::BadConversion ConvervsionException)
	{
		std::cout << ConvervsionException.what() << std::endl;
	}
}
