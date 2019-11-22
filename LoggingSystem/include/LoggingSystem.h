#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
#include <iostream>
#include <initializer_list>
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
	void printMessageWithEPICSTimestampString(const std::string& epicsTimeString, const std::string& message);
	void printDebugMessageWithEPICSTimestampString(const std::string& epicsTimeString, const std::string& debugMessage);

	template <typename T>
	void printMessage(T t)
	{
		if (messageOn)
		{
			std::cout << t << std::endl;
		}
	}

	template<typename T, typename... Args>
	void printMessage(T t, Args... args) // recursive variadic function
	{
		if (messageOn)
		{
			std::cout << t;
			printMessage(args...);
		}
	}

	template <typename T>
	void printDebugMessage(T t)
	{
		if (debugOn)
		{
			std::cout << t << std::endl;
		}

	}

	template<typename T, typename... Args>
	void printDebugMessage(T t, Args... args) // recursive variadic function
	{
		if (debugOn)
		{
			std::cout << t;
			printMessage(args...);
		}
	}

private:
    std::string getCurrentDateAndTimeString();
    bool debugOn;
    bool messageOn;
};


#endif // LOGGING_SYSTEM_H_
