#include <stdexcept>
#include <string>
#include <vector>
class InvalidFileException : std::runtime_error
{
public:
	InvalidFileException(std::string filename, std::string missingEntry);
	InvalidFileException(std::string filename,  std::vector<std::string> missingEntries);
	void printError();
	std::string filename;
	std::string missingEntry;
	std::vector<std::string> missingEntries;
};