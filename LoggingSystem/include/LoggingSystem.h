#ifndef LOGGING_SYSTEM_H_
#define LOGGING_SYSTEM_H_
#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include <time.h>
#define DEBUG "[DEBUG]"
#define MESSAGE "[MESSAGE]"

namespace Caching
{
	extern std::ostringstream cache;
}
class LoggingSystem
{
public:
	LoggingSystem() { debugOn = false; messageOn = false; }
	LoggingSystem(bool debugState, bool messageState);
	LoggingSystem(const LoggingSystem& messenger);
	bool debugOn;
	bool messageOn;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn() const;
	bool isDebugOn() const;
	//static std::ostringstream cache;
	static void dumpToFile(std::string filename);
	std::string getCurrentDateAndTimeString() const;

	template<typename T>
	void generateStringStream(std::ostream& os, T t) const
	{
		os << t;
	}
	template<typename T, typename... Args>
	void generateStringStream(std::ostream& os, T t, Args... args) const
	{
		generateStringStream(os, t);
		generateStringStream(os, args...);
	}

	template<typename... Args>
	void printMessage(Args... args) const
	{
		std::ostringstream oss;
		generateStringStream(oss, args...);
		std::ios::sync_with_stdio(true);
		if (messageOn)
		{
			fprintf(stdout, "%s %s %s \n", getCurrentDateAndTimeString().c_str(), MESSAGE, oss.str().c_str());
		}
		Caching::cache << getCurrentDateAndTimeString().c_str() << MESSAGE << oss.str().c_str() << std::endl;
	}

	template<typename... Args>
	void printDebugMessage(Args... args) const
	{
		std::ostringstream oss;
		generateStringStream(oss, args...);
		std::ios::sync_with_stdio(true);
		if (debugOn)
		{
			fprintf(stdout, "%s %s %s\n", getCurrentDateAndTimeString().c_str(), DEBUG, oss.str().c_str());
		}
		Caching::cache << getCurrentDateAndTimeString().c_str() << DEBUG << " " << oss.str().c_str() << std::endl;
	}
};
#endif // LOGGING_SYSTEM_H_
