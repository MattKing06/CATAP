#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#define DEBUG "[DEBUG]"
#define MESSAGE "[MESSAGE]"

class LoggingSystem
{
public:
	static void debugMessagesOn();
	static void debugMessagesOff();
	static void messagesOn();
	static void messagesOff();
	static bool isMessagingOn();
	static bool isDebugOn();
	static void dumpToFile(std::string filename);
	static std::string getCurrentDateAndTimeString();
	static std::ostringstream cache;

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
			cache << getCurrentDateAndTimeString().c_str() << MESSAGE << oss.str().c_str() << std::endl;
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
			cache << getCurrentDateAndTimeString().c_str() <<  DEBUG << " " <<  oss.str().c_str() << std::endl;
			fprintf(stdout, "%s %s %s\n", getCurrentDateAndTimeString().c_str(), DEBUG, oss.str().c_str());
		}
	}
	static bool debugOn;
	static bool messageOn;
};
#endif // LOGGING_SYSTEM_H_
