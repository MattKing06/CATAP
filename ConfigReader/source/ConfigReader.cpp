#include "ConfigReader.h"
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include "boost/algorithm/string.hpp"
#include "yaml-cpp/exceptions.h"

ConfigReader::ConfigReader(const std::string hardwareComponentName){
	yamlFileDestination = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	yamlFilename = hardwareComponentName + ".yaml";
	//create a string-vector pointer to empty string-vector
	// so we aren't writing to inaccessbile memory
	// have array of PVs as max size for now (that's what 0U is doing)
	// maybe use a different data structure or a resizing method later.
}

ConfigReader::ConfigReader()
{
	//since we have not specified a hardware component
	// we assume that we want to load all hardware yaml files.
	// So we set up the directory of the master lattice files, and nothing else.
	yamlFileDestination = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	yamlFilename = "";
}
/*
std::multimap<std::string, std::string> ConfigReader::parseYamlDirectory()
{
	std::multim
}
*/
std::map<std::string, std::string> ConfigReader::parseYamlFile()
{	
	std::ifstream fileInput;
	YAML::Node config;
	YAML::Node configTemplate;
	//before we start, config reader is usually a member to the Hardware classes
	// and only gets initialized once, we need to reset PVs when parsing
	// otherwise we just keep adding values to the multimap.
	PVs.clear();
	try{
		fileInput = std::ifstream(ConfigReader::yamlFileDestination + "\\" + ConfigReader::yamlFilename);
		YAML::Parser parser(fileInput);
		config = YAML::LoadFile(ConfigReader::yamlFileDestination + "\\" + ConfigReader::yamlFilename);
	}
	catch (YAML::BadFile BadFileException)
	{
		std::cout << "I could not find the file (" << ConfigReader::yamlFileDestination + "\\" + ConfigReader::yamlFilename << ") you were looking for..." << "\n";
		return PVs;
	}
	try
	{
		if (config.size() > 0)
		{
			std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + "\\" + config["properties"]["hardware_type"].as<std::string>() + ".yaml";
			configTemplate = YAML::LoadFile(hardwareTemplateFilename);
			// dealing with controls_information specifically
			auto controls_information = config["controls_information"];
			auto hardwareProperties = config["properties"];
			if (controls_information["PV"].as<bool>())
			{
				std::string control_records = controls_information["records"].as<std::string>();
				boost::trim_left(control_records);
				// add record to PV along with COUNT, MASK, CHID, and CHTYPE which could be retrieved from EPICS.
				//CHID;
				//COUNT;
				//MASK;
				//CHTYPE;
				// end of pv struct construction.
				std::pair<std::string, std::string> pvAndRecordPair = std::make_pair(config["properties"]["name"].as<std::string>(), control_records);
				PVs.insert(pvAndRecordPair);
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
			else
			{
				std::cout << "NOT ENTERING PROPERTIES LOOP, PROPERTIES UNDEFINED" << std::endl;
			}
			for (auto prop : hardwarePropertyAndValueVector)
			{
				PVs.insert(prop);
			}			{

				return PVs;
			}
		}
		else
		{
			
			throw std::length_error("File contents were of length " + std::to_string(config.size()) + ", file must be empty!");
			return PVs;
		}
	}
	catch (std::length_error EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return PVs;
	}
	catch (YAML::ParserException EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return PVs;
	}
	catch (YAML::BadConversion ConvervsionException)
	{
		std::cout << ConvervsionException.what() << std::endl;
	}
}
