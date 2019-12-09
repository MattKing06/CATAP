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
	LoggingSystem() { debugOn = false; messageOn = false; }
	LoggingSystem(bool debugState, bool messageState);
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isMessagingOn() const;
	bool isDebugOn() const;
	void dumpToFile(std::string filename);
	std::string getCurrentDateAndTimeString() const;
	std::ostringstream cache;

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
	void printDebugMessage(Args... args)
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
private:
	bool debugOn;
	bool messageOn;
};
#endif // LOGGING_SYSTEM_H_
