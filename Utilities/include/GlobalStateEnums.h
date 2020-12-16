#ifndef GLOBAL_STATE_ENUMS_H_
#define GLOBAL_STATE_ENUMS_H_

#include <boost/preprocessor.hpp>
#include <string>

/* be careful editing the below macro, you have been warned */
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//                                                                            \
    /// This macro creates an enum and also enables us to                     \
    /// get strings of the entries with ENUM_TO_STRING                        \
//
//#include <boost/preprocessor.hpp>
//#define X_DEFINE_ENUM_WITH_STRING_COnVERSIONS_TOSTRING_CASE(r, data, elem)    \
//    case elem : return BOOST_PP_STRINGIZE(elem);
//#define DEFINE_ENUM_WITH_STRING_COnVERSIONS(name, enumerators)                \
//    enum name {                                                               \
//        BOOST_PP_SEQ_ENUM(enumerators)                                        \
//    };                                                                        \
//    inline  std::string ENUM_TO_STRING(name v)                                \
//    {                                                                         \
//        switch(v)                                                             \
//        {                                                                     \
//            BOOST_PP_SEQ_FOR_EACH(                                            \
//                X_DEFINE_ENUM_WITH_STRING_COnVERSIONS_TOSTRING_CASE,          \
//                name,                                                         \
//                enumerators                                                   \
//         )                                                                    \
//            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
//        }                                                                     \
//    }                                                                         \

#include <boost/preprocessor.hpp>
#define X_DEFINE_ENUM_WITH_STRING_COnVERSIONS_TOSTRING_CASE(r, data, elem)  case elem : return BOOST_PP_STRINGIZE(elem);
#define DEFINE_ENUM_WITH_STRING_COnVERSIONS(name, enumerators) enum name {BOOST_PP_SEQ_ENUM(enumerators)};inline  std::string ENUM_TO_STRING(name v){switch(v){BOOST_PP_SEQ_FOR_EACH(X_DEFINE_ENUM_WITH_STRING_COnVERSIONS_TOSTRING_CASE,name,enumerators)default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";}}

/*

						**************** README *********************
						**************** README *********************
						**************** README *********************
						**************** README *********************
						**************** README *********************

	IF YOU ADD TO THE LIST BE SURE TO UPDATE  stringToStateMap in GlobalConstants.h

	AND the python exposure functions GlobalStatePythonIterface.h
  ___ ___   _   ___  __  __ ___
 | _ \ __| /_\ |   \|  \/  | __|
 |   / _| / _ \| |) | |\/| | _|
 |_|_\___/_/ \_\___/|_|  |_|___|

	**************** README *********************
	**************** README *********************
	**************** README *********************
	IF YOU ADD TO THE LIST BE SURE TO UPDATE  stringToStateMap in GlobalConstants.h			
	IF YOU ADD TO THE LIST BE SURE TO UPDATE  GlobalStatesPythonInterface.h

*/
/*! @addtogroup utils*/
/*!@{*/
DEFINE_ENUM_WITH_STRING_COnVERSIONS(STATE, 
(OFF)
(ON)
//(ERROR) !!YOU CAN'T USE THE WORD ERROR IN THIS SCHEME!!! (With @!#~ MSVC)
//(ERROR) !!YOU CAN'T USE THE WORD ERROR IN THIS SCHEME!!! (With MSVC)
(UNKNOWN_NAME)
(SUCCESS)
(FAIL)
(TIMING)
(OFFLINE)
(PHYSICAL)
(VIRTUAL)
(ERR)
(OK)
(GOOD)
(BAD)
(OPEN)
(CLOSED)
(NONLINEAR) // BPM
(UNKNOWN)
//screen stuff
(ENABLED)
(DISABLED)
(HRETRACTED)
(HMAX)
(HSLIT1)
(HSLIT2)
(HSLIT3)
(HAPT1)
(HAPT2)
(HAPT3)
(VRETRACTED)
(VMAX)
(VRF)
(VMIRROR)
(VYAG)
(VGRAT)
(VCOL)
(VSLIT1)
(RETRACTED)
(YAG)
(RF)
// camera stuff 
(ACQUIRING)
(NOT_ACQUIRING)
(ANALYSING)
(NOT_ANALYSING)
(CAPTURING)
(NOT_CAPTURING)
(WRITING)
(NOT_WRITING)
(WRITE_CHECK_OK)
(WRITE_CHECK_ERROR)
(TIMEOUT)
(USING_FLOOR)
(NOT_USING_FLOOR)
(USING_NPOINT)
(NOT_USING_NPOINT)
(USING_BACKGROUND)
(NOT_USING_BACKGROUND)
(PASSIVE)   // 0 // LLRF SCAN States
(EVENT)     // 1
(IO_INTR)   // 2
(TEN)       // 3
(FIVE)      // 4
(TWO)       // 5
(ONE)       // 6
(ZERO_POINT_FIVE) // 7
(ZERO_POINT_TWO)  // 8
(ZERO_POINT_ONE)  // 9
(ZERO_POINT_ZERO_FIVE)   // 10
(NOW)    // 1 // LLRF ACQM STATES, MORE CAN BE ADDED
// (EVENT) // 2, already exists
// LLRF TRIGGER STATES
(EXTERNAL)
(INTERNAL)
(ACTIVE)
(NOT_ACTIVE)


)
/*!@}*/
#endif //GLOBAL_STATE_ENUMS_H_