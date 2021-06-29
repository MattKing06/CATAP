#ifndef SNAPSHOT_FILE_MANAGER_H_
#define SNAPSHOT_FILE_MANAGER_H_

#include <boost/filesystem.hpp>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <GlobalStateEnums.h>

namespace SnapshotFileManager
{
	extern std::string const defaultMachineSnapshotLocation;   
	extern std::vector<std::string> getAllFilesInDirectory(const std::string& dirPath, const std::vector<std::string> skipList = {});
	extern bool isLocationValid(const std::string& location);
	extern bool isFormatValid(const std::string& location);
	extern bool doesFileExist(const std::string& filePath);
	extern bool writeSnapshotToYAML(const std::string& location, const std::string& filename, const YAML::Node& writeData, const STATE& mode);
	extern boost::system::error_code createMachineSnapshotDirectory();

//		 __   __        __  ___     __  ___       ___  ___ 
//		|  \ |__) |  | |__)  |     /__`  |  |  | |__  |__  
//		|__/ |__) \__/ |  \  |     .__/  |  \__/ |    |    
//		
// DONT use 
	extern const std::vector<std::string> dburt_extensions;
	extern bool containsDBURTExtension(const std::string& str);
	extern std::pair<bool, std::string> isDBURTFileAlias(const std::string& full_path);
	std::string trimToDelimiter(std::string const& str, const std::string& STOPDELIMITER);
	extern YAML::Node DBURT2YAMLNode(const boost::filesystem::path& full_path) ;
	extern std::vector<std::string> getDBURTKeyVal(const std::string& trimmedLine, const char delim);
}


#endif //! SNAPSHOT_FILE_MANAGER_H_