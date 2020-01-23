#include "GlobalFunctions.h"

namespace GlobalFunctions {

	bool stringIsSubString(const std::string& stringToCheck, const std::string& stringToLookFor)
	{
		return stringToCheck.find(stringToLookFor) != std::string::npos;
	}
	bool stringIsSubString(const std::string& stringToCheck, const char * stringToLookFor)
	{
		return stringIsSubString(stringToCheck, std::string(stringToLookFor));
	}

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



}