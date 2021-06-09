#ifndef SNAPSHOT_FILE_MANAGER_H_
#define SNAPSHOT_FILE_MANAGER_H_

#include <boost/filesystem.hpp>

namespace SnapshotFileManager
{
	extern std::string const defaultMachineSnapshotLocation;
	extern std::vector<std::string> getAllFilesInDirectory(const std::string& dirPath, const std::vector<std::string> skipList = {});
	extern bool isLocationValid(const std::string& location);
	extern bool doesFileExist(const std::string& filePath);

}


#endif //! SNAPSHOT_FILE_MANAGER_H_