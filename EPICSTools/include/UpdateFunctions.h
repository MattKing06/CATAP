#ifndef UPDATE_FUNCTIONS_H
#define UPDATE_FUNCTIONS_H

#include <PV.h>

typedef void(*UpdateFunctionPtr)(const struct event_handler_args args);

class UpdateFunctionHolder
{
public:
	UpdateFunctionHolder();
	UpdateFunctionHolder(const UpdateFunctionHolder& copyUpdateFunctionHolder);
	UpdateFunctionPtr findUpdateFunction(pvStruct& pv);
	static void updateDoubleValue(const struct event_handler_args args);
	static void updateIntegerValue(const struct event_handler_args args);
};


#endif