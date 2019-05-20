#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
#include <boost\python.hpp>
class LoggingSystem
{
public:
    LoggingSystem(){debugOn=false;messageOn=false;}
    LoggingSystem(bool debugState, bool messageState);
    void debugMessagesOn();
    void debugMessagesOff();
    void messagesOn();
    void messagesOff();
    bool isMessagingOn();
    bool isDebugOn();
    void printDebugMessage(const std::string &debugMessage);
	void testPrint();
    void printMessage(const std::string &message);
private:
    std::string getCurrentDateAndTimeString();
    bool debugOn;
    bool messageOn;
};


#endif // LOGGING_SYSTEM_H_
