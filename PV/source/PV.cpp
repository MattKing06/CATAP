#ifndef PV_H_
#include "PV.h"

/*
*
* WE MUST DEFINE THESE VARIABLES IN THE CPP FILE
* SO THAT THEY ARE CONTAINED WITHIN THE LINKER
* DEFINITIONS FOR THE PV.lib FILE. OTHERWISE,
* EPICSInterface/Factories/etc. WILL NOT KNOW WHAT THEY ARE!
*
*/

struct pvStruct *PVStruct;
void (*pvStruct::updateFunction)(struct event_handler_args args)= NULL;

bool operator==(const pvStruct& lhs, const pvStruct& rhs)
{
	if (lhs.CHID == rhs.CHID &&
		lhs.fullPVName == rhs.fullPVName &&
		lhs.pvRecord == rhs.pvRecord &&
		lhs.COUNT == rhs.COUNT &&
		lhs.MASK == rhs.MASK &&
		lhs.CHTYPE == rhs.CHTYPE){
		return true;
	}
	else{ return false; }
}
bool operator !=(const pvStruct& lhs, const pvStruct& rhs)
{
	return !(lhs == rhs);
}
std::ostream& operator<< (std::ostream& os, const pvStruct& rhs){
	os << rhs.CHID
		<< rhs.CHTYPE
		<< rhs.COUNT
		<< rhs.fullPVName
		<< rhs.MASK
		<< rhs.pvRecord;
	return os;
}

#endif //PV_H_