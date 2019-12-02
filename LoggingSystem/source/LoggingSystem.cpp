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

#ifndef TIME_DATE_BUFFER_SIZE
#define TIME_DATE_BUFFER_SIZE 80
#endif // TIME_DATE_BUFFER_SIZE

LoggingSystem::LoggingSystem(bool debugState, bool messageState){
    LoggingSystem::debugOn = debugState;
    LoggingSystem::messageOn = messageState;
}
// Get methods for debugging and message state
bool LoggingSystem::isDebugOn() const
{
    return debugOn;
}
bool LoggingSystem::isMessagingOn() const
{
    return messageOn;
}
// On and Off functions for debugging and messages
void LoggingSystem::debugMessagesOn(){
    debugOn = true;
}
void LoggingSystem::debugMessagesOff(){
    debugOn = false;
}
void LoggingSystem::messagesOn(){
    messageOn = true;
}
void LoggingSystem::messagesOff(){
    messageOn = false;
}

std::string LoggingSystem::getCurrentDateAndTimeString() const{
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
    strftime(buf, sizeof(buf), "<%d-%m-%Y %H:%M:%S> %t",&tstruct);
	//std::string dateAndTimeString = buf;
	return buf;
}