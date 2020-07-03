#ifndef UPDATE_FUNCTIONS_H
#define UPDATE_FUNCTIONS_H

class UpdateFunctionHolder
{
public:
	UpdateFunctionHolder();
	UpdateFunctionHolder(const UpdateFunctionHolder& copyUpdateFunctionHolder);
	static void updateDoubleValue(const struct event_handler_args args);
};


#endif