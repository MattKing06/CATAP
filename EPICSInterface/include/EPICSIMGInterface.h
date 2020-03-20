#ifndef EPICS_IMG_INTERFACE_H_
#define EPICS_IMG_INTERFACE_H_
#include <EPICSInterface.h>
#ifndef EPICS_INTERFACE_H_
#include <EPICSInterface.h>
#endif // EPICS INTERFACE
#ifndef IMG_H_
#include <IMG.h>
#endif // IMG

#include <iostream>
#include <GlobalStateEnums.h>
// forward declaratiOn of IMG class
// tells compiler that we will use this class.
class IMG;
typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSIMGInterface : public EPICSInterface
{
public:
	EPICSIMGInterface();
	~EPICSIMGInterface();
	std::string ownerName;
	// EPICS calls these functiOns to update a variable
	static void updateIMGP(const struct event_handler_args args);
	/*! messenger object for EPICSIMGInterface messaging. */
//	static LoggingSystem static_messenger;
	static LoggingSystem messenger;

};

#endif //EPICS_IMG_INTERFACE_H_