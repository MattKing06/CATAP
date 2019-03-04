#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "LoggingSystem.h"

class ConfigReader
{
	public:
		ConfigReader();
		ConfigReader(const std::string& configFile,
					 const LoggingSystem logging,
					 const bool usingVM);
		bool readConfig();
		//need to define shutter object and create array of them here.
		// for getShutterObjects() method
	private:
		const std::string& configFile;
		const LoggingSystem logging;
		const bool usingVM;

};


#endif