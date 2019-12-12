#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>

#include <sstream>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <time.h>

#ifndef TIME_DATE_BUFFER_SIZE
#define TIME_DATE_BUFFER_SIZE 80
#endif // TIME_DATE_BUFFER_SIZE

#define DEBUG "[DEBUG]"
#define MESSAGE "[MESSAGE]"
namespace LoggingSystem
{
	static void debugMessagesOn();
	static void debugMessagesOff();
	static void messagesOn();
	static void messagesOff();
	static bool isMessagingOn();
	static bool isDebugOn();
	static std::string getCurrentDateAndTimeString()
	{
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[TIME_DATE_BUFFER_SIZE];
		#ifdef _WIN32
			localtime_s(&tstruct, &now);
		#endif //WIN32

		#if defined(__unix__) ||  defined(_unix)
			localtime_r(&now, &tstruct);
		#endif //UNIX

		// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// for more information about date/time format
		strftime(buf, sizeof(buf), "<%d-%m-%Y %H:%M:%S> %t", &tstruct);
		//std::string dateAndTimeString = buf;
		return buf;
	}

	template<typename T>
	static void generateStringStream(std::ostream& os, T t)
	{
		os << t;
	}
	template<typename T, typename... Args>
	static void generateStringStream(std::ostream& os, T t, Args... args)
	{
		generateStringStream(os, t);
		generateStringStream(os, args...);
	}

	template<typename... Args>
	static void printMessage(Args... args)
	{
		if (messageOn)
		{
			std::ostringstream oss;
			generateStringStream(oss, args...);
			std::ios::sync_with_stdio(true);
			fprintf(stdout, "%s %s %s \n", getCurrentDateAndTimeString().c_str(), MESSAGE, oss.str().c_str());
		}
	}

	template<typename... Args>
	static void printDebugMessage(Args... args)
	{
		if (debugOn)
		{
			std::ostringstream oss;
			generateStringStream(oss, args...);
			std::ios::sync_with_stdio(true);
			fprintf(stdout, "%s %s %s\n", getCurrentDateAndTimeString().c_str(), DEBUG, oss.str().c_str());
		}
	}
	static bool debugOn;
	static bool messageOn;

}
#endif // LOGGING_SYSTEM_H_