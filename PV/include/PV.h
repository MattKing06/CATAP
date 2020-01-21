#ifndef PV_H_
#define PV_H_
#include <string>
//epics
#ifndef __CINT__
#include <cadef.h>
#include <epicsTime.h>
#endif
//#define PV_NAMESPACE_ACTIVE

typedef struct pvStruct
{
	pvStruct() : EVID(nullptr){};
	chid CHID;
	std::string fullPVName;
	std::string pvRecord;
	unsigned long COUNT;
	unsigned long MASK;
	chtype CHTYPE;
	chtype monitorCHTYPE;
	static void(*updateFunctiOn)(const struct event_handler_args args);
	evid EVID;
	epicsTimeStamp time;
	bool monitor;
}pvStruct;
extern bool operator==(const pvStruct& lhs, const pvStruct& rhs);
extern bool operator !=(const pvStruct& lhs, const pvStruct& rhs);
extern std::ostream& operator<< (std::ostream& os, const pvStruct& rhs);

#endif