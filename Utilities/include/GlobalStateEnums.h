#ifndef GLOBAL_STATE_ENUMS_H_
#define GLOBAL_STATE_ENUMS_H_

#include <boost/preprocessor.hpp>
#include <string>


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

/*! @addtogroup utils*/
/*!@{*/
DEFINE_ENUM_WITH_STRING_COnVERSIONS(STATE, (OFF)
(ON)
//(ERROR) !!YOU CAN@T USE THE WORD ERROR IN THIS SCHEME!!! (With MSVC)
(UNKNOWN_NAME)
(SUCCESS)
(OFFLINE)
(PHYSICAL)
(VIRTUAL)
(ERR)
(OK)
(GOOD)
(BAD)
(OPEN)
(CLOSED)
(NONLINEAR)
(UNKNOWN)
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
)

DEFINE_ENUM_WITH_STRING_COnVERSIONS(TYPE, 
(VELA_PNEUMATIC)
(VELA_HV_MOVER)
(VELA_V_MOVER)
(CLARA_HV_MOVER)
(CLARA_V_MOVER)
(CLARA_PNEUMATIC)
(UNKNOWN_SCREEN_TYPE)
(HORIZONTAL)
(VERTICAL)
(PNEUMATIC)
)
/*!@}*/
#endif //GLOBAL_STATE_ENUMS_H_