#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
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
    void printDebugMessage(std::string &debugMessage);
    void printMessage(std::string &message);
private:
    std::string getCurrentDateAndTimeString();
    bool debugOn;
    bool messageOn;
};


#endif // LOGGING_SYSTEM_H_
