#ifndef EPICS_BPM_INTERFACE_H_
#define EPICS_BPM_INTERFACE_H_
#ifndef BPM_H_
#include "BPM.h"
#endif //BPM_H_
#ifndef EPICS_INTERFACE_H_
#include "EPICSInterface.h"
#endif 
#include <iostream>
#include <string>
#include <boost/circular_buffer.hpp>
// forward declaration of BPM class
// tells compiler that we will use this class.
class BPM;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSBPMInterface : public EPICSInterface
{
public:
	EPICSBPMInterface();
	~EPICSBPMInterface();
	void retrieveUpdateFunctionForRecord(pvStruct& pvStruct) const;
	static void updatePositions(const struct event_handler_args args);
	static void updateXPV(const struct event_handler_args args);
	static void updateYPV(const struct event_handler_args args);
	static void updateSA1(const struct event_handler_args args);
	static void updateSA2(const struct event_handler_args args);
	static void updateSD1(const struct event_handler_args args);
	static void updateSD2(const struct event_handler_args args);
	static void updateRA1(const struct event_handler_args args);
	static void updateRA2(const struct event_handler_args args);
	static void updateRD1(const struct event_handler_args args);
	static void updateRD2(const struct event_handler_args args);
	void setSA1(const long& value, const pvStruct& pv);
	void setSA2(const long& value, const pvStruct& pv);
	void setSD1(const long& value, const pvStruct& pv);
	void setSD2(const long& value, const pvStruct& pv);
	template<typename T>
	void putValue(const pvStruct& pvStruct, const T& value) const;
	static LoggingSystem messenger;
};

#endif //EPICS_BPM_INTERFACE_H_