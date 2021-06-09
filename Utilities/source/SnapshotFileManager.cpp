#include <SnapshotFileManager.h>
#include <iostream>
namespace SnapshotFileManager
{
	std::string const defaultMachineSnapshotLocation = "\\\\claraserv3\\claranet";
	
	std::vector<std::string> getAllFilesInDirectory(const std::string& dirPath, const std::vector<std::string> skipList)
	{
		std::vector<std::string> fileList;
		boost::filesystem::path path(dirPath);
		const std::vector<std::string> extensions = { ".yml", ".yaml", ".YML", ".YAML" };
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
					else if (std::find(extensions.begin(), extensions.end(),
						iter->path().extension().string()) != extensions.end())
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