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
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
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
	std::string getCurrentDateAndTimeString() const;
	bool timestampPrinted = false;

#ifdef _WIN32
	template <typename T>
	void printMessage(T t)
	{
		if (messageOn)
		{
			if (timestampPrinted)
			{
				std::cout << t << std::endl;
				timestampPrinted = false;
			}
			else
			{
				std::cout << getCurrentDateAndTimeString() << t << std::endl;
			}
			std::cout.flush();
		}
	}
	template<typename T, typename... Args>
	void printMessage(T t, Args... args) // recursive variadic function
	{
		if (messageOn)
		{
			if (timestampPrinted)
			{
				std::cout << t;
				printMessage(args...);
			}
			else
			{
				timestampPrinted = true;
				std::cout << getCurrentDateAndTimeString() << t;
				printMessage(args...);
			}
			timestampPrinted = false;
		}
	}
#elif defined(_unix) || defined(__unix__)
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

#endif


#ifdef _WIN32
	template <typename T>
	void printDebugMessage(T t)
	{
		if (debugOn)
		{
			if (timestampPrinted)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << t << std::endl;
				SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
				timestampPrinted = false;
			}
			else
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << getCurrentDateAndTimeString() << t << std::endl;
				SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
			}
		}
	}
	template<typename T, typename... Args>
	void printDebugMessage(T t, Args... args) // recursive variadic function
	{
		if (debugOn)
		{
			if (timestampPrinted)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << t;
				SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
				printDebugMessage(args...);
			}
			else
			{
				timestampPrinted = true;
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << getCurrentDateAndTimeString() << t;
				SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
				printDebugMessage(args...);
			}
			timestampPrinted = false;
		}
	}
#elif defined(_unix) || defined(__unix__)
	template <typename T>
	void printDebugMessage(T t)
	{
		if (debugOn)
		{
			std::cout << t << std::endl;
	}
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
    bool debugOn;
    bool messageOn;
#ifdef _WIN32
	WORD GetConsoleTextAttribute(HANDLE hCon)
	{
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		GetConsoleScreenBufferInfo(hCon, &console_info);
		return console_info.wAttributes;
	}
#endif
};
#endif // LOGGING_SYSTEM_H_
