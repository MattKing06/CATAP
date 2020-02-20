#include <exception>
#include <string>
#include <vector>
class InvalidFileException : std::exception
{
public:
	InvalidFileException(std::string filename, std::string missingEntry);
	InvalidFileException(std::string filename,  std::vector<std::string> missingEntries);
	~InvalidFileException() throw() {}
	void printError();
	std::string filename;
	std::string missingEntry;
	std::vector<std::string> missingEntries;
};