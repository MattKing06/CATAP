#include <stdexcept>
#include <string>
#include <vector>

/*! @defgroup exceptions
 *  @ingroup configreader*/
/*!@{*/
/*! @brief Exception thrown when a Master Lattice Configuration File
		   does not contain a parameter from its 
*	       associated hardware template file.
*
*   This exception should be thrown by the ConfigReader class when it finds a Master Lattice Configuration file
*   that does not match the specified template of that hardware-type's configuration files.
*/
class InvalidFileException : std::runtime_error
{
public:
	/*! Constructor taking associated filename and a single entry that was not present
	* @param[in] filename : The name of the file that did not match the template.
	* @param[in] missingEntry : The string of the parameter that the file was missing.*/
	InvalidFileException(std::string filename, std::string missingEntry);
    /*! Constructor taking associated filename and multiple entries that were not present
	* @param[in] filename : The name of the file that did not match the template.
	* @param[in] missingEntries : A list of the parameters that the file was missing.*/
	InvalidFileException(std::string filename,  std::vector<std::string> missingEntries);
	/*! Function to print a formatted message containing filename and missing entries.*/
	void printError();
	/*! The name of the file that did not match the template.*/
	std::string filename;
	/*! The string of the parameter that the file was missing if we are only missing one parameter.*/
	std::string missingEntry;
	/*! The list of the parameters that the file was missing if we are missing multiple parameters.*/
	std::vector<std::string> missingEntries;
};

class LatticeLocationDoesNotExistException : std::runtime_error
{
public:
	LatticeLocationDoesNotExistException(const std::string& location);
	void printError();
	std::string location;
};


class EmptyLatticeDirectoryException : std::runtime_error
{
public:
	EmptyLatticeDirectoryException(const std::string& location);
	void printError();
	std::string location;
};

/*!@}*/
