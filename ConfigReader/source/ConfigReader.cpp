#include "ConfigReader.h"
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include "boost\algorithm\string.hpp"
#include "yaml-cpp\exceptions.h"

ConfigReader::ConfigReader(const std::string hardwareComponentName){
	yamlFileDestination = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	yamlFilename = hardwareComponentName + ".yaml";
	//create a string-vector pointer to empty string-vector
	// so we aren't writing to inaccessbile memory
	typedef std::vector< std::tuple<std::string, std::string> > pvAndRecordVector;
	PVs = pvAndRecordVector();
	// have array of PVs as max size for now (that's what 0U is doing)
	// maybe use a different data structure or a resizing method later.
}

std::vector <std::tuple<std::string, std::string>> ConfigReader::parseYamlFile()
{	
	std::ifstream fileInput;
	YAML::Node config;
	YAML::Node configTemplate;
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
				std::cout << config["name"] << std::endl;
				std::vector<std::string> controls_records;
				boost::split(controls_records, controls_information["records"].as<std::string>(), [](char c){return c == ','; });
				for (auto record : controls_records)
				{
					boost::trim_left(record);
					std::tuple<std::string, std::string> pvAndRecordPair = std::make_tuple(config["properties"]["name"].as<std::string>(), record);
					PVs.push_back(pvAndRecordPair);
				}
			}
			std::vector<std::tuple <std::string, std::string> > hardwarePropertyAndValueVector;
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
						std::cout << properties.first.as<std::string>() << ":" << properties.second.as<std::string>() << std::endl;
						hardwarePropertyAndValueVector.push_back(std::make_tuple(key, value));
				}
			}
			else
			{
				std::cout << "NOT ENTERING PROPERTIES LOOP, PROPERTIES UNDEFINED" << std::endl;
			}
			for (auto prop : hardwarePropertyAndValueVector)
			{
				PVs.push_back(prop);
			}
			for (auto item : PVs)
			{
				std::cout << std::get<0>(item) << ":" << std::get<1>(item) << std::endl;
			}
			//for (auto element = configTemplate.begin(); element != configTemplate.end(); element++)
			//{
			//	if (element->first.IsDefined())
			//	{ 
			//		YAML::Node key = element->first;
			//		YAML::Node value = element->second;
			//		std::string keyString;
			//		std::string valueString;
			//		//DEALING WITH FIRST LAYER OF PARAMETERS
			//		if (key.Type() == YAML::NodeType::Scalar)
			//		{
			//			keyString = key.as<std::string>();
			//		}
			//		if (value.Type() == YAML::NodeType::Scalar && config[key].IsDefined())
			//		{
			//			valueString = config[key].as<std::string>();
			//			keyString = key.as<std::string>();
			//			auto keyValuePair = std::make_pair(keyString, valueString);
			//			PVs.insert(keyValuePair);
			//		}
			//		// DEALING WITH SECOND LAYER OF PARAMETERS [Controls info, PSU (w\ controls info)]
			//		else if (value.Type() == YAML::NodeType::Map)
			//		{
			//			for (auto item : value)
			//			{
			//				key = item.first;
			//				value = item.second;
			//				std::cout << key << ":" << config[key] << std::endl;
			//				if (config[key].IsDefined())
			//				{
			//					std::cout << "MADE IT" << std::endl;
			//					keyString = key.as<std::string>();
			//					valueString = config[key].as<std::string>();
			//					auto keyValuePair = std::make_pair(keyString, valueString);
			//					PVs.insert(keyValuePair);
			//				}
			//			}
			//		}
			//	}
			//else
			//{
			//	std::cout << " ELEMENT NOT DEFINED " << std::endl;
			//}
		//}
			//for (auto pv : PVs){
			//	std::cout << pv.first << ":" << pv.second << std::endl;

			//}
			return PVs;
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