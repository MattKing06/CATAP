#include "ConfigReader.h"
#include <fstream>
#include <iostream>
#include <vector>
ConfigReader::ConfigReader(const std::string filename){
	yamlFileDestination = filename;
	//have array of PVs as max size for now (that's what 0U is doing)
	// maybe use a different data structure or a resizing method later.
}

std::vector<std::string>* ConfigReader::parseYamlFile()
{	
	std::ifstream fileInput;
	YAML::Node config;
	try{
		fileInput = std::ifstream(ConfigReader::yamlFileDestination);
		YAML::Parser parser(fileInput);
		config = YAML::LoadFile(ConfigReader::yamlFileDestination);
	}
	catch (YAML::BadFile BadFileException)
	{
		std::cout << "I could not find the file (" << ConfigReader::yamlFileDestination <<") you were looking for..." << "\n";
		return NULL;
	}
	try
	{
		if (config.size() > 0)
		{
			YAML::Node elements = config["elements"];
			//here we go through the elements to find PV and PV_method tags
			// instead of printing these, we should probably put them into a map/array ready to talk to epics
			for (auto iter = elements.begin(); iter != elements.end(); iter++)
			{
				for (auto methodIter = iter->second["PV_methods"].begin(); methodIter != iter->second["PV_methods"].end(); methodIter++)
				{
					ConfigReader::PVs->push_back(iter->second["PV"].as<std::string>() + ":" + methodIter->as<std::string>());
					std::cout << iter->second["PV"] << ":" << methodIter->as<std::string>() << "\n";
				}
				
			}
			return ConfigReader::PVs;
		}
		else
		{
			throw std::length_error("File contents were of length 0, file must be empty!");
			return NULL;
		}
	}
	catch (std::length_error EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination << "): " << EmptyFileException.what() << std::endl;
		return NULL;
	}
	catch (YAML::ParserException EmptyFileException)
	{
		std::cout << "Problem with file (" << ConfigReader::yamlFileDestination << "): " << EmptyFileException.what() << std::endl;
		return NULL;
	}
}