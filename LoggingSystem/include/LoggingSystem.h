#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <time.h>
#define DEBUG "[DEBUG]"
#define MESSAGE "[MESSAGE]"
#ifndef TIME_DATE_BUFFER_SIZE
#define TIME_DATE_BUFFER_SIZE 80
#endif // TIME_DATE_BUFFER_SIZE
namespace LoggingSystem
{
	namespace Switches
	{
		// not to be used outside of LoggingSystem, use debug/messages On/Off functions
		extern bool debugOn;
		extern bool messageOn;
	}
	namespace Storage
	{
		// not to be used outside of LoggingSystem,
		// cache is written to using printMessage/printDebug
		// and used by dumpToFile.
		extern std::ostringstream cache;
	}
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn();
	bool isDebugOn();
	void dumpToFile(std::string filename);
	std::string getCurrentDateAndTimeString();
	template<typename T>
	void generateStringStream(std::ostream& os, T t)
	{
		os << t;
	}
	template<typename T, typename... Args>
	void generateStringStream(std::ostream& os, T t, Args... args)
	{
		generateStringStream(os, t);
		generateStringStream(os, args...);
	}

	template<typename... Args>
	void printMessage(Args... args)
	{
		if (Switches::messageOn)
		{
			std::ostringstream oss;
			generateStringStream(oss, args...);
			std::ios::sync_with_stdio(true);
			Storage::cache << getCurrentDateAndTimeString().c_str() << MESSAGE << " " << oss.str().c_str() << std::endl;
			fprintf(stdout, "%s %s %s\n", getCurrentDateAndTimeString().c_str(), MESSAGE, oss.str().c_str());
		}
	}

	template<typename... Args>
	void printDebugMessage(Args... args)
	{
		if (Switches::debugOn)
		{
			std::ostringstream oss;
			generateStringStream(oss, args...);
			std::ios::sync_with_stdio(true);
			Storage::cache << getCurrentDateAndTimeString().c_str() <<  DEBUG << " " <<  oss.str().c_str() << std::endl;
			fprintf(stdout, "%s %s %s\n", getCurrentDateAndTimeString().c_str(), DEBUG, oss.str().c_str());
		}
	}

};
#endif // LOGGING_SYSTEM_H_
