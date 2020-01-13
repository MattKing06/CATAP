#ifndef GLOBAL_FUNCTIONS_H_
#define GLOBAL_FUNCTIONS_H_

#include <map>
#include <string>

namespace GlobalFunctions{

    /*
            We often check if entries exist in maps,
            use these functions to do it safely
            be careful to pick the correct one!
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