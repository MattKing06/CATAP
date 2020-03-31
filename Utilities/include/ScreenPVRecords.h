#ifndef SCREEN_RECORDS
#define SCREEN_RECORDS
#include <string>
#include <vector>
#include <map>

#include "GlobalStateEnums.h"
#include "GlobalTypeEnums.h"

#include "GlobalConstants.h"

/*! @addtogroup utils*/
/*!@{*/

/*!
	Namespace to define keys of "pv_record_map" found in MasterLattice Config Files for Screens.
*/
namespace ScreenRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	/*! Used to search for "H:SDEV" key in pv_record_map */
	extern std::string const HSDEV; // = "H:SDEV";
	/*! Used to search for "H:TRIGGER" key in pv_record_map */
	extern std::string const HTRIGGER; // = "H:TRIGGER";
	/*! Used to search for "H:EX" key in pv_record_map */
	extern std::string const HEX; // = "H:EX";
	/*! Used to search for "H:TGTPOS" key in pv_record_map */
	extern std::string const HTGTPOS; // = "H:TGTPOS";
	/*! Used to search for "H:JOG" key in pv_record_map */
	extern std::string const HJOG; // = "H:JOG";
	/*! Used to search for "H:JDIFF" key in pv_record_map */
	extern std::string const HJDIFF; // = "H:JDIFF";
	/*! Used to search for "H:MOVING" key in pv_record_map */
	extern std::string const HMOVING; // = "H:MOVING";
	/*! Used to search for "H:READY" key in pv_record_map */
	extern std::string const HREADY; // = "H:READY";
	/*! Used to search for "H:GET_DEV" key in pv_record_map */
	extern std::string const HGETDEV; // = "H:GET_DEV";
	/*! Used to search for "H:DEV_STATE" key in pv_record_map */
	extern std::string const HDEVSTATE; // = "H:DEV_STATE";
	/*! Used to search for "H:MAX_POS" key in pv_record_map */
	extern std::string const HMAXPOS; // = "H:MAX_POS";
	/*! Used to search for "H:DEV_CENT" key in pv_record_map */
	extern std::string const HDEVCENT; // = "H:DEV_CENT";
	/*! Used to search for "H:ACTPOS" key in pv_record_map */
	extern std::string const HACTPOS; // = "H:ACTPOS";
	/*! Used to search for "H:EN" key in pv_record_map */
	extern std::string const HEN; // = "H:EN";
	/*! Used to search for "V:SDEV" key in pv_record_map */
	extern std::string const VSDEV; // = "V:SDEV";
	/*! Used to search for "V:TRIGGER" key in pv_record_map */
	extern std::string const VTRIGGER; // = "V:TRIGGER";
	/*! Used to search for "V:EX" key in pv_record_map */
	extern std::string const VEX; // = "V:EX";
	/*! Used to search for "V:TGTPOS" key in pv_record_map */
	extern std::string const VTGTPOS; // = "V:TGTPOS";
	/*! Used to search for "V:JOG" key in pv_record_map */
	extern std::string const VJOG; // = "V:JOG";
	/*! Used to search for "V:JDIFF" key in pv_record_map */
	extern std::string const VJDIFF; // = "V:JDIFF";
	/*! Used to search for "V:MOVING" key in pv_record_map */
	extern std::string const VMOVING; // = "V:MOVING";
	/*! Used to search for "V:READY" key in pv_record_map */
	extern std::string const VREADY; // = "V:READY";
	/*! Used to search for "V:GET_DEV" key in pv_record_map */
	extern std::string const VGETDEV; // = "V:GET_DEV";
	/*! Used to search for "V:DEV_STATE" key in pv_record_map */
	extern std::string const VDEVSTATE; // = "V:DEV_STATE";
	/*! Used to search for "V:MAX_POS" key in pv_record_map */
	extern std::string const VMAXPOS; // = "V:MAX_POS";
	/*! Used to search for "V:DEV_CENT" key in pv_record_map */
	extern std::string const VDEVCENT; // = "V:DEV_CENT";
	/*! Used to search for "V:ACTPOS" key in pv_record_map */
	extern std::string const VACTPOS; // = "V:ACTPOS";
	/*! Used to search for "V:EN" key in pv_record_map */
	extern std::string const VEN; // = "V:EN";
	/*! Used to search for "SDEV" key in pv_record_map */
	extern std::string const SDEV; // = "SDEV";
	/*! Used to search for "TRIGGER" key in pv_record_map */
	extern std::string const TRIGGER; // = "TRIGGER";
	/*! Used to search for "EX" key in pv_record_map */
	extern std::string const EX; // = "EX";
	/*! Used to search for "TGTPOS" key in pv_record_map */
	extern std::string const TGTPOS; // = "TGTPOS";
	/*! Used to search for "JOG" key in pv_record_map */
	extern std::string const JOG; // = "JOG";
	/*! Used to search for "JDIFF" key in pv_record_map */
	extern std::string const JDIFF; // = "JDIFF";
	/*! Used to search for "MOVING" key in pv_record_map */
	extern std::string const MOVING; // = "MOVING";
	/*! Used to search for "READY" key in pv_record_map */
	extern std::string const READY; // = "READY";
	/*! Used to search for "GET_DEV" key in pv_record_map */
	extern std::string const GETDEV; // = "GET_DEV";
	/*! Used to search for "DEV_STATE" key in pv_record_map */
	extern std::string const DEVSTATE; // = "DEV_STATE";
	/*! Used to search for "MAX_POS" key in pv_record_map */
	extern std::string const MAXPOS; // = "MAX_POS";
	/*! Used to search for "DEV_CENT" key in pv_record_map */
	extern std::string const DEVCENT; // = "DEV_CENT";
	/*! Used to search for "ACTPOS" key in pv_record_map */
	extern std::string const ACTPOS; // = "ACTPOS";
	/*! Used to search for "EN" key in pv_record_map */
	extern std::string const EN; // = "EN";

	extern std::string const HPV;
	extern std::string const VPV;

	/*! Used to search for "H:RETRACTED" key in devices */
	extern std::pair< int, STATE > const HRETRACTED; // = "HRETRACTED";
	/*! Used to search for "H:MAX" key in devices */
	extern std::pair< int, STATE > const HMAX; // = "HMAX";
	/*! Used to search for "H_SLIT_1" key in devices */
	extern std::pair< int, STATE > const HSLIT1; // = "HSLIT1";
	/*! Used to search for "H_SLIT_2" key in devices */
	extern std::pair< int, STATE > const HSLIT2; // = "HSLIT2";
	/*! Used to search for "H_SLIT_3" key in devices */
	extern std::pair< int, STATE > const HSLIT3; // = "HSLIT3";
	/*! Used to search for "H_APT_1" key in devices */
	extern std::pair< int, STATE > const HAPT1; // = "HAPT1";
	/*! Used to search for "H_APT_2" key in devices */
	extern std::pair< int, STATE > const HAPT2; // = "HAPT2";
	/*! Used to search for "H_APT_3" key in devices */
	extern std::pair< int, STATE > const HAPT3; // = "HAPT3";
	/*! Used to search for "V:RETRACTED" key in devices */
	extern std::pair< int, STATE > const VRETRACTED; // = "HRETRACTED";
	/*! Used to search for "V:MAX" key in devices */
	extern std::pair< int, STATE > const VMAX; // = "VMAX";
	/*! Used to search for "V:RF" key in devices */
	extern std::pair< int, STATE > const VRF; // = "VRF";
	/*! Used to search for "V:MIRROR" key in devices */
	extern std::pair< int, STATE > const VMIRROR; // = "VMIRROR";
	/*! Used to search for "V_YAG" key in devices */
	extern std::pair< int, STATE > const VYAG; // = "VYAG";
	/*! Used to search for "V_GRAT" key in devices */
	extern std::pair< int, STATE > const VGRAT; // = "VGRAT";
	/*! Used to search for "V_COL" key in devices */
	extern std::pair< int, STATE > const VCOL; // = "VCOL";
	/*! Used to search for "V_SLIT_1" key in devices */
	extern std::pair< int, STATE > const VSLIT1; // = "VSLIT1";
	/*! Used to search for "RETRACTED" key in devices */
	extern std::pair< int, STATE > const RETRACTED; // = "RETRACTED";
	/*! Used to search for "RF" key in devices */
	extern std::pair< int, STATE > const RF; // = "RF";
	/*! Used to search for "YAG" key in devices */
	extern std::pair< int, STATE > const YAG; // = "YAG";

	/*! All possible screen PV records */
	extern std::vector<std::string> screenRecordList;
	/*! Possible screen PV records for an HV mover */
	extern std::vector<std::string> screenHVRecordList;
	/*! Possible screen PV records for a V mover */
	extern std::vector<std::string> screenVRecordList;
	/*! Possible screen PV records for a pneumatic mover */
	extern std::vector<std::string> screenPRecordList;

	/*! A map of the directions corresponding to all screen devices */
	extern std::map<std::string, TYPE> screenDirectionList;
	/*! Used for converting between device string and STATE enum */
	extern std::map<std::string, STATE> screenDevicesToEnum;
	/*! A map of the directions corresponding to all screen devices */
	extern std::map<std::string, TYPE> screenTypeToEnum;
	/*! A map of the integer values to send to epics for the corresponding horizontal STATE enum */
	extern std::map<int, STATE> screenHElementMap;
	/*! A map of the integer values to send to epics for the corresponding vertical STATE enum */
	extern std::map<int, STATE> screenVElementMap;
	/*! A map of the integer values to send to epics for the corresponding pneumatic STATE enum */
	extern std::map<int, STATE> screenPElementMap;
}
/*! @}*/


namespace ScreenState
{
	struct screenStateStruct
	{   // proviude a default constructor
		screenStateStruct() :
			numScreens(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numScreens;
		//HWC_ENUM::MACHINE_AREA machineArea;
		std::vector<std::string> screenNames;
		std::vector<STATE> screenStates;
		//#ifdef BUILD_DLL
		//		boost::python::list magNames_Py;
		//		boost::python::list psuStates_Py;
		//		boost::python::list riValues_Py;
		//		boost::python::list siValues_Py;
		//#endif
	};
}

#endif