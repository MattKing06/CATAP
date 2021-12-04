#ifndef GLOBAL_FUNCTIOnS_H_
#define GLOBAL_FUNCTIOnS_H_

//#include "epicsTime.h"
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <utility>
#include <time.h>
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip> 
#include <iterator>
#include "GlobalConstants.h"
#include "GlobalTypeEnums.h"

/*! @addtogroup utils*/
/*!@{*/

/*!
	Namespace to define functions that are used throughout CATAP.
	These can be used within any class and were written for ease-of-use.
*/

namespace GlobalFunctions {



    /*
            We often check if entries exist in maps,
            use these functiOns to do it safely
            be careful to pick the correct One!
    */
    template<class T>
    bool entryExists(const std::map<std::string, T>& m, const std::string& name)
    {
        auto it = m.find(name);
        if (it != m.end())
			return true;
        return false;
    }

	template<class T>
	bool entriesExist(const std::map<std::string, T>& m, const std::vector<std::string>& names)
	{
		//std::cout << "entriesExist, looking for ";
		//for (auto&& item : m)
		//{
		//	std::cout << item.first << ",";
		//}
		//std::cout << std::endl;

		for (auto&& item : names)
		{
			//std::cout << "Checking " << item;
			if (entryExists(m, item))
			{
				//std::cout << " Exists! " << std::endl;
			}
			else
			{
				//std::cout << " Does not Exist returning false! " << std::endl;
				return false;
			}
		}
		//std::cout << "All entries exist, returning true " << std::endl;
		return true;
	}


	template<class T>
	bool entryExists(const std::vector<T>& v, const T& item)
	{
		if (std::find(v.begin(), v.end(), item) != v.end()) {
			/* v contains x */
			return true;
		}
		else {
			/* v does not contain x */
			return false;
		}
	}


	template<typename T>
	std::vector<T> slice(std::vector<T> const& v, int m, int n)
	{
		auto first = v.cbegin() + m;
		auto last = v.cbegin() + n + 1;
		std::vector<T> vec(first, last);
		return vec;
	}

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
	template<typename T = int>
	bool areNotSame(const  T a, const T b, const T epsilon = 0)
	{
		return !areSame(a, b, epsilon);
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
//        bool ret = false;
        //auto it = m.find(name);
		return m.find(name) != m.end();
		//if (it != m.end())
  //          ret = true;
  //      return ret;
    }


	extern TYPE stringToTYPE(const std::string& type_str);


	//template<typename T = double>
	//double roundToN(const T a, const size_t n)
	//{
	//	double p = pow(UTL::TEN_DOUBLE, (double)n);
	//	return std::round(a * p) / p;
	//}

	/* what time is is? */
	extern time_t timeNow();


	template<typename T = int>
	std::string toString(const std::vector<T>& vec)
	{
		std::ostringstream oss;
		if (!vec.empty())
		{
			// Convert all but the last element to avoid a trailing ","
			std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(oss, ","));
			// Now add the last element with no delimiter
			oss << vec.back();
		}
		return oss.str();
	}

	// trim from right
	inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
	{
		s.erase(s.find_last_not_of(t) + 1);
		return s;
	}

	/* THIS CAUSES Virtual Circuit Disconnect due to not being static...*/
	inline std::string& cStringToStdString(const char* c_string)
	{
		std::string newStr(c_string);
		//remove null-termination char from c-string
		newStr.erase(newStr.find('\0'));
		return newStr;
	}

	extern bool stringIsSubString(const std::string& stringToCheck,const std::string& stringToLookFor);

	extern std::string getTimeAndDateString();



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

	//extern bool stringIsSubString(const std::string& stringToCheck, const std::string& stringToLookFor);
	//extern bool stringIsSubString_char(const std::string& stringToCheck, const char* stringToLookFor);


	//extern std::pair<bool, std::string> findSubstringInStringvector(const std::vector<std::string>& stringvector, const std::string& substring);


	inline std::string time_iso_8601() {

		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();

		// convert to time_t which will represent the number of
		// seconds since the epoch, 
		auto epoch_seconds = std::chrono::system_clock::to_time_t(p);

		// Format this as date time to seconds resolution
		// e.g. 2016-08-30-08:18:51
		std::stringstream stream;
		stream << std::put_time(gmtime(&epoch_seconds), "%F_%T");

		// If we now convert back to a time_point we will get the time truncated
		// to whole seconds 
		//auto truncated = std::chrono::system_clock::from_time_t(epoch_seconds);

		// Now we subtract this seconds count from the original time to
		// get the number of extra microseconds..
		//auto delta_us = std::chrono::duration_cast<std::chrono::microseconds>(t - truncated).count();

		// And append this to the output stream as fractional seconds
		// e.g. 2016-08-30T08:18:51.867479
		//stream << "." << std::fixed << std::setw(2) << std::setfill('0') << delta_us;

		return stream.str();
	}

	extern std::string replaceStrChar(std::string str, const std::string& replace, char ch);


	extern unsigned short scanSTATEToNumber(const STATE state);
	extern unsigned short scanSTATEToNumber(const std::string state);
	extern STATE numberToLLRFSCAN(const unsigned short scan_num);

	extern void pause_x(std::chrono::milliseconds x);
	void standard_pause();
	void pause_2000();
	void pause_1000();
	void pause_300() ;
	void pause_500() ;
	void pause_50()  ;
	void pause_2()   ;
	void pause_1()   ;

	extern TYPE stringToType(const std::string& string_to_compare);
	extern STATE stringToState(const std::string& string_to_compare);

	// isInMahineArea

	extern bool isInMachineArea(TYPE testArea, TYPE area);
	
	extern std::string trimAllWhiteSpace(std::string strIN);
	extern std::string trimAllWhiteSpaceExceptBetweenDoubleQuotes(std::string strIN);


	extern STATE stringToSTATE(const std::string& state_str);


	extern  bool stringIsTrue(const std::string& str_to_compare);
	extern  bool stringIsFalse(const std::string& str_to_compare);
}
/*! @}*/
#endif