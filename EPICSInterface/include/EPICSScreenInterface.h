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
// forward declaratiOn of Screeen class
// tells compiler that we will use this class.
class Screeen;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSScreeenInterface : public EPICSInterface
{
public:
	EPICSScreeenInterface();
	~EPICSScreeenInterface();
	void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;
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
	void setSDEV(const short& value, const pvStruct& pv);
	void setDEVSTATE(const short& value, const pvStruct& pv);
	void setDEVCENT(const double& value, const pvStruct& pv);
	void setACTPOS(const double& value, const pvStruct& pv);
	void setEN(const short& value, const pvStruct& pv);
	void setJDIFF(const double& value, const pvStruct& pv);
	void setJOG(const double& value, const pvStruct& pv);
    static LoggingSystem messenger;
};

#endif //EPICS_Screeen_INTERFACE_H_