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
    bool isMessagingOn() const;
    bool isDebugOn() const;
    void printDebugMessage(const std::string &debugMessage);
    void printMessage(const std::string &message);
	void printMessageWithEPICSTimestampString(const std::string& epicsTimeString, const std::string& message);
	void printDebugMessageWithEPICSTimestampString(const std::string& epicsTimeString, const std::string& debugMessage);
private:
    std::string getCurrentDateAndTimeString();
    bool debugOn;
    bool messageOn;
};


#endif // LOGGING_SYSTEM_H_
