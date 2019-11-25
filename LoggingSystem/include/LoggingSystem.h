#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
#include <iostream>
#include <initializer_list>
#ifdef _WIN32
// Windows.h defines macros min and max so numeric_limits::max fails.
// defining NOMINMAX removes the min max macros from Windows.h
#define NOMINMAX
#include <Windows.h>
#endif
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
#ifdef _WIN32
	template<typename T, typename... Args>
	void printMessage(T t, Args... args) // recursive variadic function
	{
		if (messageOn)
		{
			std::cout << t;
			printMessage(args...);
		}
	}
#elif define(_unix) || define(__unix__)
	template<typename T, typename... Args>
	void printMessage(T t, Args... args) // recursive variadic function
	{
		if (messageOn)
		{
			std::cout << t;
			printMessage(args...);
		}
	}

#endif

	template <typename T>
	void printDebugMessage(T t)
	{
		if (debugOn)
		{
			std::cout << t << std::endl;
		}

	}
#ifdef _WIN32
	template<typename T, typename... Args>
	void printDebugMessage(T t, Args... args) // recursive variadic function
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		const int previousConsoleColours = GetConsoleTextAttribute(hConsole);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		if (debugOn)
		{
			std::cout << t;
			printDebugMessage(args...);
		}
		SetConsoleTextAttribute(hConsole, previousConsoleColours);
	}
#elif defined(_unix) || (__unix__)
	template<typename T, typename... Args>
	void printDebugMessage(T t, Args... args)
	{
		if (debugOn)
		{
			std::cout << t;
			printDebugMessage(args...);
		}
	}
#endif

private:
    std::string getCurrentDateAndTimeString();
    bool debugOn;
    bool messageOn;
#ifdef _WIN32
	WORD GetConsoleTextAttribute(HANDLE hCon)
	{
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		GetConsoleScreenBufferInfo(hCon, &console_info);
		return console_info.wAttributes;
	}
};
#endif

#endif // LOGGING_SYSTEM_H_
