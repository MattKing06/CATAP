#include "GlobalFunctions.h"

namespace GlobalFunctions {

	//bool stringIsSubString(const std::string& stringToCheck, const std::string& stringToLookFor)
	//{
	//	return stringToCheck.find(stringToLookFor) != std::string::npos;
	//}
	//bool stringIsSubString_char(const std::string& stringToCheck, const char * stringToLookFor)
	//{
	//	return stringIsSubString(stringToCheck, std::string(stringToLookFor));
	//}

	//std::pair<bool,std::string> findSubstringInStringvector(const std::vector<std::string>& stringvector, const std::string& substring)
	//{
	//	std::pair<bool, std::string> r;
	//	r.first = false;
	//	r.second = "ERRROROR";
	//	for (auto&& string : stringvector)
	//	{
	//		if (stringIsSubString(string, substring)
	//		{
	//			r.first = true;
	//			r.second = string;
	//			return r;
	//		}
	//	}
	//	return r;
	//}

	time_t timeNow(){ return time(nullptr); }

	std::string getTimeAndDateString()
	{
		//  std::cout << "baseObject::currentDateTime() " << std::endl;
		time_t     now = timeNow();
		struct tm  tstruct;
		char       buf[GlobalConstants::eighty_sizet];

		localtime_s(&tstruct, &now);

		 // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		 // for more information about date/time format
		 // strftime(buf, sizeof(buf), "%Y-%m-%d-%H%M%",&tstruct);
		strftime(buf, sizeof(buf), "%c", &tstruct);
		// std::cout << "buf = " << buf <<  std::endl;
		return buf;
	}

	void pause_x(std::chrono::milliseconds x)
	{
		std::this_thread::sleep_for(x);
	}
	//______________________________________________________________________________
	void standard_pause()
	{
		pause_2000();
	}
	//______________________________________________________________________________
	void pause_1000()
	{
		pause_x(GlobalConstants::PAUSE_1000);
	}
	//______________________________________________________________________________
	void pause_2000()
	{
		pause_x(GlobalConstants::STANDARD_PAUSE);
	}
	//______________________________________________________________________________
	void pause_300()
	{
		pause_x(GlobalConstants::PAUSE_300);
	}
	//______________________________________________________________________________
	void pause_500()
	{
		pause_x(GlobalConstants::PAUSE_500);
	}
	//______________________________________________________________________________
	void pause_50()
	{
		pause_x(GlobalConstants::PAUSE_50);
	}
	//______________________________________________________________________________
	void pause_2()
	{
		pause_x(GlobalConstants::PAUSE_2);
	}
	//______________________________________________________________________________
	void pause_1()
	{
		pause_x(GlobalConstants::PAUSE_1);
	}


}