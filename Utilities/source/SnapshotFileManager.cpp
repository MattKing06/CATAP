#include <SnapshotFileManager.h>
#include <iostream>
namespace SnapshotFileManager
{
	std::string const defaultMachineSnapshotLocation = "\\\\claraserv3\\claranet\\MachineSnapshots";
	const std::vector<std::string> extensions = { ".yml", ".yaml", ".YML", ".YAML" };

	bool isFormatValid(const std::string& fileExtension) 
	{
		if (std::find(SnapshotFileManager::extensions.begin(), SnapshotFileManager::extensions.end(),
			fileExtension) != SnapshotFileManager::extensions.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool writeSnapshotToYAML(const std::string& location, const std::string& filename, const YAML::Node& outputNode, const STATE& mode)
	{
		const std::string fullpath = location + "/" + filename;
		std::ofstream outFile(fullpath.c_str());
		if (!outFile) { return false; }
		// COULD GET THE HARDWARE TYPE FROM OutputNode, NEED TO ENFORCE HARDWARE TYPE AS FIRST ELEMENT.
		outFile << "# YAML VELA/CLARA VALVE SETTINGS SAVE FILE: VERSION 1" << std::endl;
		outFile << "# THIS SNAPSHOT WAS CREATED IN: " << ENUM_TO_STRING(mode) << " MODE." << std::endl;
		outFile << outputNode << std::endl;
		return true;
	}

	boost::system::error_code createMachineSnapshotDirectory()
	{
		return boost::system::error_code();
	}
	
	std::vector<std::string> getAllFilesInDirectory(const std::string& dirPath, const std::vector<std::string> skipList)
	{
		std::vector<std::string> fileList;
		boost::filesystem::path path(dirPath);
		try
		{
			if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path))
			{
				boost::filesystem::directory_iterator iter(path);
				boost::filesystem::directory_iterator end;
				while (iter != end)
				{
					if (boost::filesystem::is_directory(iter->path()) &&
						(std::find(skipList.begin(), skipList.end(), iter->path().filename()) != skipList.end()))
					{
						boost::system::error_code ec;
						iter.increment(ec);
						if (ec)
						{
							std::cout << " COULD NOT ACCESS : " <<  iter->path().string() << std::endl;
						}
					}
					else if (isFormatValid(iter->path().extension().string()))
					{
						fileList.push_back(iter->path().string());
						boost::system::error_code ec;
						iter.increment(ec);
						if (ec)
						{
							std::cout << " COULD NOT ACCESS : " << iter->path().string() << std::endl;
						}
					}
					else
					{
						boost::system::error_code ec;
						iter.increment(ec);
						std::cout << iter->path().string() << " is in the wrong format (no yaml files found...)" << std::endl;
					}
				}
			}
		}
		catch (std::system_error& e)
		{
			std::cout << e.what() << std::endl;
		}
		return fileList;
	}
}