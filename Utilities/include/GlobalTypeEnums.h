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


#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)  case elem : return BOOST_PP_STRINGIZE(elem);
#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators) enum name {BOOST_PP_SEQ_ENUM(enumerators)};inline  std::string ENUM_TO_STRING(name v){switch(v){BOOST_PP_SEQ_FOR_EACH(X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,name,enumerators)default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";}}


/*
						**************** README *********************
	
	IF YOU ADD TO THE LIST BE SURE TO UPDATE  stringToTypeMap in GlobalConstants.h
		 
	IF ADDING A MACHINE_AREA THAT IS AN AGGREGATE OF MANY AREAS UPDATE  GlobalfUNCTIONS::isInMachineArea
*/
DEFINE_ENUM_WITH_STRING_CONVERSIONS(TYPE,   //(ERROR) !!YOU CAN'T USE THE WORD ERROR IN THIS SCHEME!!! (With MSVC)
	(UNKNOWN_TYPE)
	// magnets
	(MAGNET)
	(QUADRUPOLE)
	(DIPOLE)
	//   (HVCOR) is there such thing as an HVCorr ??? 
	(VERTICAL_CORRECTOR)
	(HORIZONTAL_CORRECTOR)
	(SOLENOID)
	(BUCKING_SOLENOID)
	// magnet PSU polarity switching type 
	(NR)
	(BIPOLAR)
	(NR_GANGED)     /// Yeah NR_GANGED, just when you thought it was already too complicated
	(POSITIVE_ONLY)
	/// machine areas  are these "types" ?? YES!!
	(UNKNOWN_AREA)
	(ALL_VELA_CLARA)
	(VELA_GUN)
	(CLARA_GUN)
	(CLARA_HRRG)
	(CLARA_LRRG)
	(VELA_HRRG)
	(VELA_LRRG)
	(HRRG_GUN)
	(LRRG_GUN)
	(LAS) // TODO change to laser or laser transport ??? 
	(VELA_LASER)
	(CLARA_LASER)
	(L01)
	(BA1)
	(BA2)
	(VELA)
	(INJ)
	(GUN)
	(S01)
	(S02)
	(C2V)
	(VELA_INJ)
	(CLARA_PH1)
	(VELA_2_BA1)
	(CLARA_2_BA1)
	(VELA_2_BA1_BA2)
	(CLARA_2_BA1_BA2)
	// M
	// screen stuff
	(SCREEN)
	(VELA_PNEUMATIC)
	(VELA_HV_MOVER)
	(VELA_V_MOVER)
	(CLARA_HV_MOVER)
	(CLARA_V_MOVER)
	(CLARA_PNEUMATIC)
	(UNKNOWN_SCREEN_TYPE)
	// horizontal and vertical may be generic
	(HORIZONTAL)
	(VERTICAL)
	(PNEUMATIC)
	(CHARGE)
	(BPM_TYPE) // TODO hmm this has the same name as the object !! and so breaks things
	(VALVE)
	(IMG_TYPE) // 
	(LLRF_TYPE)
	(CAMERA_TYPE)
	(HWP)
	(ENERGYMETER)
	// shutter objects
	(SHUTTER)

	// PROT 
	(RF_PROTECTION)
	(ENABLE)
	(GENERAL)
	(RESET)
	(TEST)

)

#endif //GLOBAL_TYPE_ENUMS_H_
