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
bool LoggingSystem::isDebugOn(){
    return debugOn;
}
bool LoggingSystem::isMessagingOn()
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
void LoggingSystem::printDebugMessage(const std::string &debugMessage){
    if (debugOn){
        //get the current date and time in string format
        std::string currentDateAndTime = LoggingSystem::getCurrentDateAndTimeString();
        //print the debug message with timestamped value
        std::cout << "DEBUG LOG (" << currentDateAndTime << "): " << debugMessage << std::endl;
    }
}
void LoggingSystem::printMessage(const std::string &message){
    if(messageOn){
        //get the current date and time in string format
        std::string currentDateAndTime = LoggingSystem::getCurrentDateAndTimeString();
        //print the debug message with timestamped value
        std::cout << "MESSAGE LOG (" << currentDateAndTime << "): " << message << std::endl;
    }
}
std::string LoggingSystem::getCurrentDateAndTimeString(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[TIME_DATE_BUFFER_SIZE];
    localtime_r(&now, &tstruct);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H%M",&tstruct);
    return buf;
}
