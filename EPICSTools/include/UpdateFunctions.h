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
	static void updateEnumValue(const struct event_handler_args args);
	static void updateFloatValue(const struct event_handler_args args);
	static void updateLongValue(const struct event_handler_args args);
	static void updateStringValue(const struct event_handler_args args);
	static void updateDoubleArray(const struct event_handler_args args);
	static void updateIntegerArray(const struct event_handler_args args);
	static void updateEnumArray(const struct event_handler_args args);
	static void updateFloatArray(const struct event_handler_args args);
	static void updateLongArray(const struct event_handler_args args);
	static void updateStringArray(const struct event_handler_args args);
};


#endif