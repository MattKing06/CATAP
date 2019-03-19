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
			std::ifstream templateFileInput = std::ifstream(hardwareTemplateFilename);
			YAML::Parser templateParser(templateFileInput);
			YAML::Node configTemplate = YAML::LoadFile(hardwareTemplateFilename);
			for (auto templateIter = configTemplate.begin(); templateIter != configTemplate.end(); templateIter++)
			{
				//ConfigReader::PVs->push_back(configIter->first.as<std::string>());
				if (templateIter->first.IsDefined())
				{
					std::string key = templateIter->first.as<std::string>();
					if (config[key].IsScalar())
					{
						std::string result = config[key].as<std::string>();
						std::cout << key << std::endl;
					}
					else
					{
						std::cout << templateIter->first.as<std::string>() << std::endl;
						for (auto objectIter = templateIter->second.begin(); objectIter != templateIter->second.end(); objectIter++)
						{
							std::cout << "\t" << objectIter->first.as<std::string>() << std::endl;
							for (auto nestedObjectIter = objectIter->second.begin(); nestedObjectIter != objectIter->second.end(); nestedObjectIter++)
							{
								std::cout << "\t \t" << nestedObjectIter->first.as<std::string>() << std::endl;
							}
						}
					}
					//return NULL;
				}
				else
				{
					std::cout << " F A I L U R E " << std::endl;
					//return NULL;
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