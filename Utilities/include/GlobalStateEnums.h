#ifndef GLOBAL_STATE_ENUMS_H_
#define GLOBAL_STATE_ENUMS_H_

#include <boost/preprocessor.hpp>
#include <string>

/** \file GlobalStateEnums.h 
 *	A header file for STATE enum and other enums
 *	that are used across CATAP by classes.
*/


/*!A Macro that allows us to define enums by name, 
with automatic 'toString' function for printing */
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                                                                            \
     This macro creates an enum and also enables us to                     \
     get strings of the entries with ENUM_TO_STRING                        \

#include <boost/preprocessor.hpp>
#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
    case elem : return BOOST_PP_STRINGIZE(elem);
#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                \
    enum name {                                                               \
        BOOST_PP_SEQ_ENUM(enumerators)                                        \
    };                                                                        \
    inline  std::string ENUM_TO_STRING(name v)                                \
    {                                                                         \
        switch(v)                                                             \
        {                                                                     \
            BOOST_PP_SEQ_FOR_EACH(                                            \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
                name,                                                         \
                enumerators                                                   \
         )                                                                    \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
        }                                                                     \
    }                                                                         \

//!< Global State Enum for commonly used states across CATAP
DEFINE_ENUM_WITH_STRING_CONVERSIONS(STATE, (OFF) //!< STATE::OFF used for PSU Readings for example 
                                           (ON) //!< STATE::ON used for PSU Readings for example
                                           (UNKNOWN_NAME) //!< STATE::UNKNOWN_NAME returned when cannot find name in config for example
                                           (SUCCESS) //!< STATE::SUCCESS returned when connected to EPICS for example
                                           (OFFLINE) //!< STATE::OFFLINE is a mode of CATAP (no connection to EPICS whatsoever)
                                           (PHYSICAL)//!< STATE::PHYSICAL is a mode of CATAP (connection to CLARA Controls System)
                                           (VIRTUAL)//!< STATE::VIRTUAL is a mode of CATAP (connection Virtual EPICS only)
                                           (ERR) //!< STATE::ERR is an enum for ERROR (returned when cannot find key in map for example)


)

#endif //GLOBAL_STATE_ENUMS_H_