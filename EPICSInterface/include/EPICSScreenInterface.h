#ifndef EPICS_SCREEN_INTERFACE_H_
#define EPICS_SCREEN_INTERFACE_H_
#ifndef SCREEN_H_
#include <Screen.h>
#endif //SCREEN_H_
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif 
#include <iostream>
#include <string>
#include <boost/circular_buffer.hpp>
#include <GlobalStateEnums.h>
// forward declaratiOn of Screen class
// tells compiler that we will use this class.
class Screen;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSScreenInterface : public EPICSInterface
{
public:
	EPICSScreenInterface();
	~EPICSScreenInterface();
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
	static void updateHSDEV(const struct event_handler_args args);
	static void updateHTRIGGER(const struct event_handler_args args);
	static void updateHEX(const struct event_handler_args args);
	static void updateHTGTPOS(const struct event_handler_args args);
	static void updateHJOG(const struct event_handler_args args);
	static void updateHJDIFF(const struct event_handler_args args);
	static void updateHMOVING(const struct event_handler_args args);
	static void updateHREADY(const struct event_handler_args args);
	static void updateHGETDEV(const struct event_handler_args args);
	static void updateHDEVSTATE(const struct event_handler_args args);
	static void updateHMAXPOS(const struct event_handler_args args);
	static void updateHDEVCENT(const struct event_handler_args args);
	static void updateHACTPOS(const struct event_handler_args args);
	static void updateHEN(const struct event_handler_args args);
	static void updateVSDEV(const struct event_handler_args args);
	static void updateVTRIGGER(const struct event_handler_args args);
	static void updateVEX(const struct event_handler_args args);
	static void updateVTGTPOS(const struct event_handler_args args);
	static void updateVJOG(const struct event_handler_args args);
	static void updateVJDIFF(const struct event_handler_args args);
	static void updateVMOVING(const struct event_handler_args args);
	static void updateVREADY(const struct event_handler_args args);
	static void updateVGETDEV(const struct event_handler_args args);
	static void updateVDEVSTATE(const struct event_handler_args args);
	static void updateVMAXPOS(const struct event_handler_args args);
	static void updateVDEVCENT(const struct event_handler_args args);
	static void updateVACTPOS(const struct event_handler_args args);
	static void updateVEN(const struct event_handler_args args);
	static void updateSDEV(const struct event_handler_args args);
	static void updateTRIGGER(const struct event_handler_args args);
	static void updateEX(const struct event_handler_args args);
	static void updateTGTPOS(const struct event_handler_args args);
	static void updateJOG(const struct event_handler_args args);
	static void updateJDIFF(const struct event_handler_args args);
	static void updateMOVING(const struct event_handler_args args);
	static void updateREADY(const struct event_handler_args args);
	static void updateGETDEV(const struct event_handler_args args);
	static void updateDEVSTATE(const struct event_handler_args args);
	static void updateMAXPOS(const struct event_handler_args args);
	static void updateDEVCENT(const struct event_handler_args args);
	static void updateACTPOS(const struct event_handler_args args);
	static void updateEN(const struct event_handler_args args);
	void setSDEV(const int value, const pvStruct& pv);
	void setEX(const int& value, const pvStruct& pv);
	void setEN(const int& value, const pvStruct& pv);
	void setTRIGGER(const int& value, const pvStruct& pv);
	void setTGTPOS(const double& value, const pvStruct& pv);
	void setJDIFF(const double& value, const pvStruct& pv);
	void setJOG(const double& value, const pvStruct& pv);
    static LoggingSystem static_messenger;
};

#endif //EPICS_Screen_INTERFACE_H_