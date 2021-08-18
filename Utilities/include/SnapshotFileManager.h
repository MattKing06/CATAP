#ifndef SNAPSHOT_FILE_MANAGER_H_
#define SNAPSHOT_FILE_MANAGER_H_

#include <boost/filesystem.hpp>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <GlobalStateEnums.h>

namespace SnapshotFileManager
{
	extern const std::string magnet_snapshot_default_path;
	extern const std::string camera_snapshot_default_path;
	extern const std::string defaultMachineSnapshotLocation;
	extern const std::string snapshot_file_reference;// = "SNAPSHOT_FILE_REFERENCE";
	extern const std::vector<std::string> dburt_extensions;
	extern const std::vector<std::string> extensions;

	extern std::vector<std::string> getAllFilesInDirectory(const std::string& dirPath, const std::vector<std::string> skipList = {});
	extern bool isLocationValid(const std::string& location);
	extern bool isFormatValid(const std::string& location);
	extern bool doesFileExist(const std::string& filePath);
	extern bool writeSnapshotToYAML(const std::string& location, const std::string& filename, const YAML::Node& writeData, const STATE& mode, const std::string& comments = "");
	extern boost::system::error_code createMachineSnapshotDirectory();

	extern bool isSnapshotFileReference(std::string& path_to_fill, std::string& filanme_to_fill);

	// WE ARE DEFINING THAT THE SNAPSHOTFILEMANAGER ONLY DEALS WITH YAML::NODES
	// SO PASS IN A YAML NODE TO WRIT ETO FILE, OR GET A YAML NODE FROM A FILE 
	extern YAML::Node readSnapshotFile(const std::string& location, const std::string& filename);
//		 __   __        __  ___     __  ___       ___  ___ 
//		|  \ |__) |  | |__)  |     /__`  |  |  | |__  |__  
//		|__/ |__) \__/ |  \  |     .__/  |  \__/ |    |    
//		
// DONT use 
	extern bool containsDBURTExtension(const std::string& str);
	extern std::pair<bool, std::string> isDBURTFileAlias(const std::string& full_path);
	std::string trimToDelimiter(std::string const& str, const std::string& STOPDELIMITER);
	extern YAML::Node DBURT2YAMLNode(const boost::filesystem::path& full_path) ;
	extern std::vector<std::string> getDBURTKeyVal(const std::string& trimmedLine, const char delim);
}
#endif //! SNAPSHOT_FILE_MANAGER_H_