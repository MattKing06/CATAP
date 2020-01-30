#ifndef GLOBAL_FUNCTIOnS_H_
#define GLOBAL_FUNCTIOnS_H_

#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <utility>
#include <time.h>
#include <vector>
#include <chrono>
#include <thread>
#include "GlobalConstants.h"


namespace GlobalFunctions{

    /*
            We often check if entries exist in maps,
            use these functiOns to do it safely
            be careful to pick the correct One!
        */
    template<class T>
    bool entryExists(const std::map<std::string, T>& m, const std::string& name)
    {
        bool ret = false;
        auto it = m.find(name);
        if (it != m.end())
            ret = true;
        return ret;
    }
    //template<class T>
    //bool entryExists(const std::map<std::string, T>& m, std::string& name)
    //{
    //    bool ret = false;
    //    auto it = m.find(name);
    //    if (it != m.end())
    //        ret = true;
    //    return ret;
    //}

	template<typename T = int>
	bool areSame(const std::vector<T> a, const std::vector<T> b, const T epsilon = 0) 
	{
		if (a.size() != b.size())
			return false;
		for (auto&& it1 = a.begin(), it2 = b.begin();
			it1 < a.end() && it2 < b.end();
			++it1, ++it2)
		{
			if (std::abs(it1 - it2) < epsilon)
			{

			}
			else
			{
				return false;
			}

		}
		return true;
	}

	template<typename T>
	bool areSame(const T a, const T b, const T epsilon = 0)
	{
		if (a == b)
			return true;
		else
			return std::abs(a - b) < epsilon;
	}

	//template<typename T = int>
	//bool areSame(const T a, const T b, const T epsilon = 0)
	//{
	//	if (a == b)
	//		return true;
	//	else
	//		return std::abs(a - b) < epsilon;
	//}


    template<class T, class U>
    bool entryExists(const std::map<U, T>& m, const U& name)
    {
        bool ret = false;
        auto it = m.find(name);
        if (it != m.end())
            ret = true;
        return ret;
    }

	template<typename T = int>
	bool areNotSame(const  T a, const T b, const T epsilon = 0)
	{
		return !areSame(a, b, epsilon);
	}

	//template<typename T = double>
	//double roundToN(const T a, const size_t n)
	//{
	//	double p = pow(UTL::TEN_DOUBLE, (double)n);
	//	return std::round(a * p) / p;
	//}

	extern time_t timeNow();// { return time(nullptr); }

	//std::string currentDateTime()
	//{
	//	time_t     now = timeNow();
	//	struct tm  tstruct;
	//	const size_t BUFFER_EIGHTY = 80;KO
	//	char       buf[BUFFER_EIGHTY];KO
	//	localtime_s(&tstruct, &now);
	//	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	//	// for more information about date/time format
	//	strftime(buf, sizeof(buf), "%Y-%m-%d-%H%M%", &tstruct);
	//	return buf;
	//}

	extern bool stringIsSubString(const std::string& stringToCheck, const std::string& stringToLookFor);
	extern bool stringIsSubString(const std::string& stringToCheck, const char* stringToLookFor);


	//extern std::pair<bool, std::string> findSubstringInStringvector(const std::vector<std::string>& stringvector, const std::string& substring);


	extern void pause_x(std::chrono::milliseconds x);
	void standard_pause();
	void pause_2000();
	void pause_1000();
	void pause_300() ;
	void pause_500() ;
	void pause_50()  ;
	void pause_2()   ;
	void pause_1()   ;



}
#endif