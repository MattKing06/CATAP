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
	/*! Used to search for "H_SDEV" key in pv_record_map */
	extern std::string const H_SDEV; // = "H_SDEV";
	/*! Used to search for "H_TRIGGER" key in pv_record_map */
	extern std::string const H_TRIGGER; // = "H_TRIGGER";
	/*! Used to search for "H_EX" key in pv_record_map */
	extern std::string const H_EX; // = "H_EX";
	/*! Used to search for "H_TGTPOS" key in pv_record_map */
	extern std::string const H_TGTPOS; // = "H_TGTPOS";
	/*! Used to search for "H_JOG" key in pv_record_map */
	extern std::string const H_JOG; // = "H_JOG";
	/*! Used to search for "H_JDIFF" key in pv_record_map */
	extern std::string const H_JDIFF; // = "H_JDIFF";
	/*! Used to search for "H_MOVING" key in pv_record_map */
	extern std::string const H_MOVING; // = "H_MOVING";
	/*! Used to search for "H_READY" key in pv_record_map */
	extern std::string const H_READY; // = "H_READY";
	/*! Used to search for "H_GET_DEV" key in pv_record_map */
	extern std::string const H_GETDEV; // = "H_GET_DEV";
	/*! Used to search for "H_DEV_STATE" key in pv_record_map */
	extern std::string const H_DEVSTATE; // = "H_DEV_STATE";
	/*! Used to search for "H_MAX_POS" key in pv_record_map */
	extern std::string const H_MAXPOS; // = "H_MAX_POS";
	/*! Used to search for "H_DEV_CENT" key in pv_record_map */
	extern std::string const H_DEVCENT; // = "H_DEV_CENT";
	/*! Used to search for "H_ACTPOS" key in pv_record_map */
	extern std::string const H_ACTPOS; // = "H_ACTPOS";
	/*! Used to search for "H_EN" key in pv_record_map */
	extern std::string const H_EN; // = "H_EN";
	/*! Used to search for "V_SDEV" key in pv_record_map */
	extern std::string const V_SDEV; // = "V_SDEV";
	/*! Used to search for "V_TRIGGER" key in pv_record_map */
	extern std::string const V_TRIGGER; // = "V_TRIGGER";
	/*! Used to search for "V_EX" key in pv_record_map */
	extern std::string const V_EX; // = "V_EX";
	/*! Used to search for "V_TGTPOS" key in pv_record_map */
	extern std::string const V_TGTPOS; // = "V_TGTPOS";
	/*! Used to search for "V_JOG" key in pv_record_map */
	extern std::string const V_JOG; // = "V_JOG";
	/*! Used to search for "V_JDIFF" key in pv_record_map */
	extern std::string const V_JDIFF; // = "V_JDIFF";
	/*! Used to search for "V_MOVING" key in pv_record_map */
	extern std::string const V_MOVING; // = "V_MOVING";
	/*! Used to search for "V_READY" key in pv_record_map */
	extern std::string const V_READY; // = "V_READY";
	/*! Used to search for "V_GET_DEV" key in pv_record_map */
	extern std::string const V_GETDEV; // = "V_GET_DEV";
	/*! Used to search for "V_DEV_STATE" key in pv_record_map */
	extern std::string const V_DEVSTATE; // = "V_DEV_STATE";
	/*! Used to search for "V_MAX_POS" key in pv_record_map */
	extern std::string const V_MAXPOS; // = "V_MAX_POS";
	/*! Used to search for "V_DEV_CENT" key in pv_record_map */
	extern std::string const V_DEVCENT; // = "V_DEV_CENT";
	/*! Used to search for "V_ACTPOS" key in pv_record_map */
	extern std::string const V_ACTPOS; // = "V_ACTPOS";
	/*! Used to search for "V_EN" key in pv_record_map */
	extern std::string const V_EN; // = "V_EN";
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

	/*! Used to search for "PNEUMATIC_ON" key in pv_record_map */
	extern std::string const PNEUMATIC_ON; // = "EN";
	/*! Used to search for "PNEUMATIC_OFF" key in pv_record_map */
	extern std::string const PNEUMATIC_OFF; // = "EN";
	/*! Used to search for "PNEUMATIC_STA" key in pv_record_map */
	extern std::string const PNEUMATIC_STA; // = "EN";

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

	/*! A map of the STATE to integer values for the horizontal devices*/
	extern const std::map<STATE, unsigned short> p_state_to_device_map;
	/*! A map of the integer values to send to epics for the corresponding horizontal STATE enum */
	extern const std::map<unsigned short, STATE> p_device_to_state_map;


	/*! A map of the STATE to integer values for the horizontal devices*/
	extern const std::map<STATE, unsigned short> h_state_to_device_map;
	/*! A map of the integer values to send to epics for the corresponding horizontal STATE enum */
	extern const std::map<unsigned short, STATE> h_device_to_state_map;

	/*! A map of the STATE to integer values for the vertical devices*/
	extern const std::map<STATE, unsigned short> v_state_to_device_map;
	/*! A map of the integer values to send to epics for the corresponding vertical STATE enum */
	extern const std::map<unsigned short, STATE> v_device_to_state_map;

	extern const std::vector<STATE> screenPElements;
	extern const std::vector<STATE> screenHElements;
	extern const std::vector<STATE> screenVElements;

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