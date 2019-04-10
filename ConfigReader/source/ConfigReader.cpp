#include "ConfigReader.h"
#include <fstream>
#include <iostream>
#include <vector>
ConfigReader::ConfigReader(const std::string fileDirectory, const std::string filename){
	yamlFileDestination = fileDirectory;
	yamlFilename = filename;
	//create a string-vector pointer to empty string-vector
	// so we aren't writing to inaccessbile memory
	PVs = new std::vector<std::string>();
	// have array of PVs as max size for now (that's what 0U is doing)
	// maybe use a different data structure or a resizing method later.
}

std::vector<std::string>* ConfigReader::parseYamlFile()
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
		return NULL;
	}
	try
	{
		if (config.size() > 0)
		{
			std::string hardwareTemplateFilename = ConfigReader::yamlFileDestination + "\\" + config["hardware_type"].as<std::string>() + ".yaml";
			configTemplate = YAML::LoadFile(hardwareTemplateFilename);
			for (auto element = configTemplate.begin(); element != configTemplate.end(); element++)
			{
				if (element->first.IsDefined())
				{ 
					YAML::Node key = element->first;
					YAML::Node value = element->second;
					std::string keyString;
					std::string valueString;
					//DEALING WITH FIRST LAYER OF PARAMETERS
					if (key.Type() == YAML::NodeType::Scalar)
					{
						keyString = key.as<std::string>();
					}
					if (value.Type() == YAML::NodeType::Scalar)
					{
						valueString = value.as<std::string>();
						keyString = key.as<std::string>();
						std::cout << keyString << " : " << valueString << std::endl;
					}
					// DEALING WITH SECOND LAYER OF PARAMETERS [Controls info, PSU (w\ controls info)]
					if (value.Type() == YAML::NodeType::Map)
					{
						std::cout  << keyString << ":" << std::endl;
						for (auto item : value)
						{
							key = item.first;
							value = item.second;
							if (value.Type() == YAML::NodeType::Scalar)
							{
								keyString = key.as<std::string>();
								valueString = value.as<std::string>();
								std::cout << "\t" << keyString << ":" << valueString << std::endl;
							}
							// DEALING WITH THIRD LAYER OF PARAMETERS [PSU Control info]
							if (value.Type() == YAML::NodeType::Map)
							{ 
								std::cout << "\t" << key.as<std::string>() << ":" << std::endl;
								for (auto nestedItem : value)
								{
									key = nestedItem.first;
									value = nestedItem.second;
									keyString = key.as<std::string>();
									valueString = value.as<std::string>();
									std::cout << "\t\t" << keyString << ":" << valueString << std::endl;
								}
							}
						}
					}
				}
				else
				{
					std::cout << " ELEMENT NOT DEFINED " << std::endl;
				}
			}
			return NULL;
		}
		else
		{
			
			throw std::length_error("File contents were of length " + std::to_string(config.size()) + ", file must be empty!");
			return NULL;
		}
	}
	catch (std::length_error EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return NULL;
	}
	catch (YAML::ParserException EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination + ConfigReader::yamlFilename << "): " << EmptyFileException.what() << std::endl;
		return NULL;
	}
}