#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
#include <iostream>
#include <sstream>
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
	bool multiString = false;

#ifdef _WIN32
	template<typename T>
	void printMessage_oss(std::ostream& os, T t)
	{
		//single-value means no end-line char here..
		if (multiString)
		{
			os << t;
		}
		else
		{
			os << t << std::endl;
		}
	}
	template<typename T, typename... Args>
	void printMessage_oss(std::ostream& os, T t, Args... args)
	{
		printMessage_oss(os, t);
		printMessage_oss(os, args...);
	}
	template<typename... Args>
	void printMessage(Args... args)
	{
		if (messageOn)
		{
			multiString = true;
			std::ostringstream oss;
			printMessage_oss(oss, args...);
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
			std::cout << oss.str() << std::endl;
			std::cout.flush();
			multiString = false;
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
	template<typename T>
	void printDebugMessage_oss(std::ostream& os, T t)
	{
		//single-value means no end-line char here..
		if (multiString)
		{
			os << t;
		}
		else
		{
			os << t << std::endl;
		}
	}
	template<typename T, typename... Args>
	void printDebugMessage_oss(std::ostream& os, T t, Args... args)
	{
		printDebugMessage_oss(os, t);
		printDebugMessage_oss(os, args...);
	}
	template<typename... Args>
	void printDebugMessage(Args... args)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
		if (debugOn)
		{
			std::stringstream oss;
			multiString = true;
			printDebugMessage_oss(oss, args...);
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << oss.str() << std::endl;
			SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
			std::cout.flush();
			multiString = false;
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
