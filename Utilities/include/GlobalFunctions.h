#ifndef GLOBAL_FUNCTIONS_H_
#define GLOBAL_FUNCTIONS_H_

#include <map>
#include <string>

//!< A namespace for grouping together functions that are used through CATAP
namespace GlobalFunctions{

    /*!
            We often check if entries exist in maps,
            use these functions to do it safely
            be careful to pick the correct one!
    */

	//! Function to find entry by string in a map of type <string, T>
	/*! entryExists takes a map<string, T> and a string as arguments, 
	searches for the entry in the map matching the string.
	Returns true if entry is in map, and false if it is not.*/
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

	//! Function to find entry by type U in a map of type <U,T>
	/*! entryExists takes a map<U,T> and a variable of type U,
		searches for the netry in the map matching the variable.
		Returns true if entry is in map, and false if it is not.
		*/
    template<class T, class U>
    bool entryExists(const std::map<U, T>& m, const U& name)
    {
        bool ret = false;
        auto it = m.find(name);
        if (it != m.end())
            ret = true;
        return ret;
    }


}
#endif