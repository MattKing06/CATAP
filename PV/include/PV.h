#ifndef PV_H_
#define PV_H_
#include <string>
//epics
#ifndef __CINT__
#include <cadef.h>
#include <epicsTime.h>
#endif
//#define PV_NAMESPACE_ACTIVE
/*!< pvStruct contains the necessary information needed by EPICS to communicate
	with a PV. This communcation involves monitoring, getting, and setting values
	for PVs */
typedef struct pvStruct
{
	pvStruct() : EVID(nullptr){};
	chid CHID; //!< Channel ID, assigned by EPICS for communcation
	std::string fullPVName; //!< The name of the hardware component e.g. CLA-C2V-MAG-HCOR-01
	std::string pvRecord; //!< The PV record on the hardware component e.g. SETI, GETSETI, etc.
	unsigned long COUNT; //!< number of elements contained in the PV record
	unsigned long MASK; //!< EPICS Mask relates to the kind of PV Value, Alarm, Log, Property
	chtype CHTYPE; //!< Expected type to set/return from EPICS e.g. DBR_DOUBLE, DBR_TIME_DOUBLE etc.
	chtype monitorCHTYPE; //!< Expected type to return from EPICS for monitoring (we force DBR_TIME_XXXX)
	static void(*updateFunction)(const struct event_handler_args args); //!< Function to call when value for EPICS PV has changed
	evid EVID; //!< Event ID, returned when monitor subscription to PV is setup.
	epicsTimeStamp time; //!< Last timestamp returned from EPICS
	bool monitor; //!< Flag to indicate when a PV is able to be monitored

}pvStruct;
extern bool operator==(const pvStruct& lhs, const pvStruct& rhs);
extern bool operator !=(const pvStruct& lhs, const pvStruct& rhs);
extern std::ostream& operator<< (std::ostream& os, const pvStruct& rhs);

#endif