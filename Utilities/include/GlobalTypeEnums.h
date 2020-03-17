#ifndef GLOBAL_TYPE_ENUMS_H_
#define GLOBAL_TYPE_ENUMS_H_

#include <boost/preprocessor.hpp>
#include <string>


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//                                                                            \
    /// This macro creates an enum and also enables us to                     \
    /// get strings of the entries with ENUM_TO_STRING                        \
//
//#include <boost/preprocessor.hpp>
//#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
//    case elem : return BOOST_PP_STRINGIZE(elem);
//#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                \
//    enum name {                                                               \
//        BOOST_PP_SEQ_ENUM(enumerators)                                        \
//    };                                                                        \
//    inline  std::string ENUM_TO_STRING(name v)                                \
//    {                                                                         \
//        switch(v)                                                             \
//        {                                                                     \
//            BOOST_PP_SEQ_FOR_EACH(                                            \
//                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
//                name,                                                         \
//                enumerators                                                   \
//         )                                                                    \
//            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
//        }                                                                     \
//    }                                                                         \
#include <boost/preprocessor.hpp>
#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)  case elem : return BOOST_PP_STRINGIZE(elem);
#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators) enum name {BOOST_PP_SEQ_ENUM(enumerators)};inline  std::string ENUM_TO_STRING(name v){switch(v){BOOST_PP_SEQ_FOR_EACH(X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,name,enumerators)default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";}}


DEFINE_ENUM_WITH_STRING_CONVERSIONS(TYPE,   //(ERROR) !!YOU CAN@T USE THE WORD ERROR IN THIS SCHEME!!! (With MSVC)
										(UNKNOWN)	
										(MAGNET)
                                        (QUADRUPOLE)
                                           (DIPOLE)
                                           (HVCOR)
                                           (VCOR)
                                           (HCOR)
										   (SOLENOID)
							/// machinw areas  are these "types" ?? 
										 (BA1)
										 (BA2)
										 (VELA)
										 (GUN)
										 (S01)
										 (S02)
										 (C2V)
)

#endif //GLOBAL_TYPE_ENUMS_H_