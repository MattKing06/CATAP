#include "GlobalFunctions.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

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

	std::string toString(const std::vector<TYPE>& vec)
	{
		std::vector<std::string> vec_string;
		for (auto it : vec)
		{
			vec_string.push_back(ENUM_TO_STRING(it));
		}
		return toString(vec_string);
	}



	bool stringIsSubString(const std::string& stringToCheck, const std::string& stringToLookFor) 
	{
		return stringToCheck.find(stringToLookFor) != std::string::npos;
	}




	std::string getTimeAndDateString()
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		auto str = oss.str();
		return str;
		//  std::cout << "baseObject::currentDateTime() " << std::endl;
		//time_t     now = timeNow();
		//struct tm  tstruct;
		//char       buf[GlobalConstants::eighty_sizet];
		//localtime(&tstruct, &now);
		// // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// // for more information about date/time format
		// // strftime(buf, sizeof(buf), "%Y-%m-%d-%H%M%",&tstruct);99iiiiiii
		//strftime(buf, sizeof(buf), "%c", &tstruct);
		//// std::cout << "buf = " << buf <<  std::endl;
		//return buf;
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

	TYPE stringToType(const std::string& string_to_check)
	{
		auto it = GlobalConstants::stringToTypeMap.find(string_to_check);
		if (it != GlobalConstants::stringToTypeMap.end())
		{
			return GlobalConstants::stringToTypeMap.at(string_to_check);
		}
		return TYPE::UNKNOWN_TYPE;
	}

	bool isInMachineArea(TYPE testArea, TYPE area)
	{
		// we will have multiple, overlapping machine areas, 
		// and so we need a function to check if an area is in testArea
		// atm we'll do this 
		switch (testArea)
		{
		case TYPE::UNKNOWN_AREA:
			return false;
		case TYPE::ALL_VELA_CLARA:
			return true;
		case TYPE::VELA_GUN:
			return entryExists(std::vector<TYPE>{TYPE::VELA_GUN, TYPE::VELA_LASER}, area);
		case TYPE::CLARA_GUN:
			return entryExists(std::vector<TYPE>{TYPE::CLARA_GUN, TYPE::CLARA_LASER}, area);
		case TYPE::HRRG_GUN:
			return testArea == area;
		case TYPE::LRRG_GUN:
			return testArea == area;
		case TYPE::L01:
			return testArea == area;
		case TYPE::BA1:
			return testArea == area;
		case TYPE::BA2:
			return testArea == area;
		case TYPE::VELA:
			return entryExists(std::vector<TYPE>{TYPE::VELA_GUN, TYPE::VELA, TYPE::INJ}, area);
		case TYPE::INJ:
			return testArea == area;
		case TYPE::GUN:
			return entryExists(std::vector<TYPE>{TYPE::VELA_GUN, TYPE::CLARA_GUN, TYPE::HRRG_GUN, TYPE::LRRG_GUN}, area);
		case TYPE::S01:
			return testArea == area;
		case TYPE::S02:
			return testArea == area;
		case TYPE::C2V:
			return testArea == area;
		case TYPE::VELA_INJ:
			return entryExists(std::vector<TYPE>{TYPE::VELA_GUN, TYPE::VELA_LASER, TYPE::INJ}, area);
		case TYPE::VELA_2_BA1:
			return entryExists(std::vector<TYPE>{TYPE::VELA_LASER, TYPE::VELA_GUN, TYPE::INJ, TYPE::BA1}, area);
		case TYPE::VELA_2_BA1_BA2:
			return entryExists(std::vector<TYPE>{TYPE::VELA_LASER, TYPE::VELA_GUN, TYPE::INJ, TYPE::BA1, TYPE::BA2}, area);
		case TYPE::CLARA_PH1:
			return entryExists(std::vector<TYPE>{TYPE::CLARA_LASER, TYPE::CLARA_GUN, TYPE::L01, TYPE::S01, TYPE::S02, TYPE::C2V}, area);
		case TYPE::CLARA_2_BA1:
			return entryExists(std::vector<TYPE>{TYPE::CLARA_LASER, TYPE::CLARA_GUN, TYPE::L01, TYPE::S01, TYPE::S02, TYPE::C2V, TYPE::INJ, TYPE::BA1}, area);
		case TYPE::CLARA_2_BA1_BA2:
			return entryExists(std::vector<TYPE>{TYPE::CLARA_LASER, TYPE::CLARA_GUN, TYPE::L01, TYPE::S01, TYPE::S02, TYPE::C2V, TYPE::INJ, TYPE::BA1, TYPE::BA2}, area);
		}
		return false;
	}

}