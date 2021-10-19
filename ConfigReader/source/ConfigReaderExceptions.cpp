#include <ConfigReaderExceptions.h>
#include <sstream>
#include <iostream>

InvalidFileException::InvalidFileException(std::string filename, std::string missingEntry) :
	runtime_error(filename.c_str()),
	filename(filename),
	missingEntry(missingEntry),
	missingEntries(std::vector<std::string>())
{
	missingEntries.push_back(missingEntry);
}

InvalidFileException::InvalidFileException(std::string filename, std::vector<std::string> missingEntries) :
	runtime_error(filename.c_str()),
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


LatticeLocationDoesNotExistException::LatticeLocationDoesNotExistException(const std::string& location) :
	runtime_error(location.c_str()),
	location(location)
{
}

void LatticeLocationDoesNotExistException::printError()
{
	std::ostringstream oss;
	std::string padding = "*******************************\n";
	std::string errorMessage = "Lattice Location Does Not Exist Exception: " + location + " cannot be found." +
		" Using default: " + MASTER_LATTICE_LOCATION + " as lattice location.";
	printf("%s%s%s", padding.c_str(), errorMessage.c_str(), padding.c_str());
}

EmptyLatticeDirectoryException::EmptyLatticeDirectoryException(const std::string& location) :
	std::runtime_error(location),
	location(location)
{
}

void EmptyLatticeDirectoryException::printError()
{
	std::ostringstream oss;
	std::string padding = "*******************************\n";
	std::string errorMessage = "Empty Lattice Directory Exception: " + location + " was empty." +
		" Using default: " + MASTER_LATTICE_LOCATION + " as lattice location.";
	printf("%s%s%s", padding.c_str(), errorMessage.c_str(), padding.c_str());
}
