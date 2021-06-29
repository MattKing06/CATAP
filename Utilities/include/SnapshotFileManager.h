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
}


#endif //! SNAPSHOT_FILE_MANAGER_H_