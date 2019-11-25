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
#define WINDOWS 1
#elif defined(_unix) || defined(__unix__)
#define WINDOWS 0
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
		if (WINDOWS)
		{
			if (messageOn)
			{
				std::cout << t << std::endl;
			}
		}
		else
		{
			if (messageOn)
			{
				std::cout << t << std::endl;
			}
		}
	}

	template<typename T, typename... Args>
	void printMessage(T t, Args... args) // recursive variadic function
	{
		if (WINDOWS)
		{
			if (messageOn)
			{
				std::cout << t;
				printMessage(args...);
			}
		}
		else
		{
			if (messageOn)
			{
				std::cout << t;
				printMessage(args...);
			}
		}
	}

	template <typename T>
	void printDebugMessage(T t)
	{
		if (WINDOWS)
		{
			if (debugOn)
			{
				std::cout << t << std::endl;
			}
		}
		else
		{
			if (debugOn)
			{
				std::cout << t << std::endl;
			}
		}

	}

	template<typename T, typename... Args>
	void printDebugMessage(T t, Args... args) // recursive variadic function
	{
		if (WINDOWS)
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
		else
		{
			if (debugOn)
			{
				std::cout << t;
				printDebugMessage(args...);
			}
		}
	}

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
