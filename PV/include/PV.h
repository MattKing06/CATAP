#ifndef PV_H_
#define PV_H_
#include <string>
//epics
#ifndef __CINT__
#include <cadef.h>
#include <epicsTime.h>
#endif


/** @addtogroup hardware
 *@{*/


/*!
  A struct to hold all EPICS-related information for a particular Process Variable.
  Each instance of a hardware class will hold a vector of pvStructs detailing all of the 
  process variables which that hardware uses to talk to EPICS.
*/
struct pvStruct
{
	pvStruct() : EVID(nullptr){};
	/*! Channel ID assigned by EPICS for the process variable.*/
	chid CHID;
	/*! Full name of the Process Variable (hardware name + record) */
	std::string fullPVName;
	/*! The record of the Process Variable i.e. READI, On, etc.*/
	std::string pvRecord; 
	// DJS adding thsi in, so we can hardcode the FULL OPV name into the yaml file
	// this enables us to handle weird and non-standrd record names
	// a fullPVRecord would be  'CLA-C2V-MAG-HCOR-01:GETSETI'
	/*! (deprecated in favour of fullPVName now) */
	std::string fullPVRecordName;
	/*! Number of elements expected when retreiving information from the process variable. Assigned by EPICS*/
	unsigned long COUNT;
	/*! Type of value we want to retrieve from EPICS, i.e. DBE_VALUE, DBE_ARCHIVE, DBE_ALARM etc.*/
	unsigned long MASK;
	/*! Channel Type (DBR_TIME_DOUBLE, DBR_ENUM etc.) of the Process Variable. Assigned by EPICS */
	chtype CHTYPE;
	/*! Channel Type (DBR_TIME_DOUBLE, DBR_ENUM etc.) of the Process Variable for monitoring. Assigned by CATAP */
	chtype monitorCHTYPE;
	/*! Pointer to the updateFunction that is used for callback when monitoring. Assigned by CATAP.*/
	static void(*updateFunction)(const struct event_handler_args args);
	/*! Event ID assigned when subscription to CHID is created. Assigned by EPICS.*/
	evid EVID;
	/*! most recent time-stamp associated with the Process Variable (deperecated). */
	epicsTimeStamp time;
	/*! Flag to say whether this PV should be used for command or for monitoring. Assigned by CATAP. */
	bool monitor;

	//void* user_param;
	

	// pv_enum, pv_enum_str
	// map of all pv_enum_str

}typedef pvStruct;
/**@}*/
extern bool operator==(const pvStruct& lhs, const pvStruct& rhs);
extern bool operator !=(const pvStruct& lhs, const pvStruct& rhs);
extern std::ostream& operator<< (std::ostream& os, const pvStruct& rhs);

#endif