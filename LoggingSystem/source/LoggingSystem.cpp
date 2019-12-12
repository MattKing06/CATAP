/******************************************************************************
* Class Name: LoggingSystem                                                   *
* Purpose: To enable debug messages from hardware controllers to be displayed *
*          on the command line using std::cout                                *
* Date Created: 21/11/2018                                                    *
* Author: Matt King                                                           *
*******************************************************************************/
#include "LoggingSystem.h"
void LoggingSystem::debugMessagesOn() { debugOn = true; }
void LoggingSystem::debugMessagesOff() { debugOn = false; }
void LoggingSystem::messagesOn() { messageOn = true; }
void LoggingSystem::messagesOff() { messageOn = false; }
bool LoggingSystem::isMessagingOn() { return messagesOn; }
bool LoggingSystem::isDebugOn() { return debugOn; }


