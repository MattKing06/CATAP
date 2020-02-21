#ifndef SCREEN_RECORDS
#define SCREEN_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace ScreenRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const HSDEV; // = "H:SDEV";
	extern std::string const HTRIGGER; // = "H:TRIGGER";
	extern std::string const HEX; // = "H:EX";
	extern std::string const HTGTPOS; // = "H:TGTPOS";
	extern std::string const HJOG; // = "H:JOG";
	extern std::string const HJDIFF; // = "H:JDIFF";
	extern std::string const HMOVING; // = "H:MOVING";
	extern std::string const HREADY; // = "H:READY";
	extern std::string const HGETDEV; // = "H:GET_DEV";
	extern std::string const HDEVSTATE; // = "H:DEV_STATE";
	extern std::string const HMAXPOS; // = "H:MAX_POS";
	extern std::string const HDEVCENT; // = "H:DEV_CENT";
	extern std::string const HACTPOS; // = "H:ACTPOS";
	extern std::string const HEN; // = "H:EN";
	extern std::string const VSDEV; // = "V:SDEV";
	extern std::string const VTRIGGER; // = "V:TRIGGER";
	extern std::string const VEX; // = "V:EX";
	extern std::string const VTGTPOS; // = "V:TGTPOS";
	extern std::string const VJOG; // = "V:JOG";
	extern std::string const VJDIFF; // = "V:JDIFF";
	extern std::string const VMOVING; // = "V:MOVING";
	extern std::string const VREADY; // = "V:READY";
	extern std::string const VGETDEV; // = "V:GET_DEV";
	extern std::string const VDEVSTATE; // = "V:DEV_STATE";
	extern std::string const VMAXPOS; // = "V:MAX_POS";
	extern std::string const VDEVCENT; // = "V:DEV_CENT";
	extern std::string const VACTPOS; // = "V:ACTPOS";
	extern std::string const VEN; // = "V:EN";
	extern std::string const SDEV; // = "SDEV";
	extern std::string const TRIGGER; // = "TRIGGER";
	extern std::string const EX; // = "EX";
	extern std::string const TGTPOS; // = "TGTPOS";
	extern std::string const JOG; // = "JOG";
	extern std::string const JDIFF; // = "JDIFF";
	extern std::string const MOVING; // = "MOVING";
	extern std::string const READY; // = "READY";
	extern std::string const GETDEV; // = "GET_DEV";
	extern std::string const DEVSTATE; // = "DEV_STATE";
	extern std::string const MAXPOS; // = "MAX_POS";
	extern std::string const DEVCENT; // = "DEV_CENT";
	extern std::string const ACTPOS; // = "ACTPOS";
	extern std::string const EN; // = "EN";

	extern std::string const HPV;
	extern std::string const VPV;

	extern std::pair< int, STATE > const HRETRACTED; // = "HRETRACTED";
	extern std::pair< int, STATE > const HMAX; // = "HMAX";
	extern std::pair< int, STATE > const HSLIT1; // = "HSLIT1";
	extern std::pair< int, STATE > const HSLIT2; // = "HSLIT2";
	extern std::pair< int, STATE > const HSLIT3; // = "HSLIT3";
	extern std::pair< int, STATE > const HAPT1; // = "HAPT1";
	extern std::pair< int, STATE > const HAPT2; // = "HAPT2";
	extern std::pair< int, STATE > const HAPT3; // = "HAPT3";
	extern std::pair< int, STATE > const VRETRACTED; // = "HRETRACTED";
	extern std::pair< int, STATE > const VMAX; // = "VMAX";
	extern std::pair< int, STATE > const VRF; // = "VRF";
	extern std::pair< int, STATE > const VMIRROR; // = "VMIRROR";
	extern std::pair< int, STATE > const VYAG; // = "VYAG";
	extern std::pair< int, STATE > const VGRAT; // = "VGRAT";
	extern std::pair< int, STATE > const VCOL; // = "VCOL";
	extern std::pair< int, STATE > const VSLIT1; // = "VSLIT1";
	extern std::pair< int, STATE > const RETRACTED; // = "RETRACTED";
	extern std::pair< int, STATE > const YAG; // = "YAG";

	extern std::vector<std::string> screenRecordList;
	extern std::map<std::string, TYPE> screenDirectionList;
	extern std::map<int, STATE> screenHElementMap;
	extern std::map<int, STATE> screenVElementMap;
	extern std::map<int, STATE> screenPElementMap;

}

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