#include <ConfigReaderExceptions.h>
#include <sstream>
#include <iostream>

InvalidFileException::InvalidFileException(std::string filename, std::string missingEntry) :
	exception(filename.c_str()),
	filename(filename),
	missingEntry(missingEntry),
	missingEntries(std::vector<std::string>())
{
	missingEntries.push_back(missingEntry);
}

InvalidFileException::InvalidFileException(std::string filename, std::vector<std::string> missingEntries) :
	exception(filename.c_str()),
	filename(filename),
	missingEntries(missingEntries)
{
}

void InvalidFileException::printError()
{
	std::ostringstream oss;
	for (auto entry : missingEntries)
	{
		oss << entry << "\n";
	}
	std::string padding = "*******************************\n";
	std::string errorMessage = "InvalidFileException: " + filename +" did not match the template." + 
							   " File did not contain entries for:\n" + oss.str() + "AS A RESULT, " + 
							   "YOU WILL NOT BE ABLE TO ACCESS THIS HARDWARE OBJECT. \n";
	printf("%s%s%s", padding.c_str(), errorMessage.c_str(), padding.c_str());
}


