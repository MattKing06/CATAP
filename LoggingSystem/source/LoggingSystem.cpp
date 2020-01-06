/******************************************************************************
* Class Name: LoggingSystem                                                   *
* Purpose: To enable debug messages from hardware controllers to be displayed *
*          on the command line using std::cout                                *
* Date Created: 21/11/2018                                                    *
* Author: Matt King                                                           *
*******************************************************************************/
#include "LoggingSystem.h"
#include <vector>
#include <iostream>
#include <time.h>

bool LoggingSystem::Switches::debugOn;
bool LoggingSystem::Switches::messageOn;
std::ostringstream LoggingSystem::Storage::cache;
// Get methods for debugging and message state
bool LoggingSystem::isDebugOn()
{
	return Switches::debugOn;
}
void LoggingSystem::dumpToFile(std::string filename)
{
	FILE* outFile;
	outFile = fopen(filename.c_str(), "w");
	fprintf(outFile, "%s \n", Storage::cache.str().c_str());
}
bool LoggingSystem::isMessagingOn()
{
	return Switches::messageOn;
}
// On and Off functions for debugging and messages
void LoggingSystem::debugMessagesOn()
{
	Switches::debugOn = true;
	printDebugMessage("CATAP: DEBUG ON");
}
void LoggingSystem::debugMessagesOff() 
{
	printDebugMessage("CATAP: DEBUG OFF");
	Switches::debugOn = false;
}
void LoggingSystem::messagesOn() 
{
	Switches::messageOn = true;
	printMessage("CATAP: MESSAGES ON");
}
void LoggingSystem::messagesOff()
{
	printDebugMessage("CATAP: MESSAGES OFF");
	Switches::messageOn = false;
}

std::string LoggingSystem::getCurrentDateAndTimeString() 
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[TIME_DATE_BUFFER_SIZE];
	#ifdef _WIN32
		localtime_s(&tstruct, &now);
	#endif //WIN32

	#if defined(__unix__) ||  defined(_unix)
		localtime_r(&now, &tstruct);
	#endif //UNIX

	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "<%d-%m-%Y %H:%M:%S> %t", &tstruct);
	//std::string dateAndTimeString = buf;
	return buf;
}