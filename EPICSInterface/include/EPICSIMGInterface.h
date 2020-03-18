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


class EPICSIMGInterface : public EPICSInterface
{
public:
	EPICSIMGInterface();
	~EPICSIMGInterface();
	/*! messenger object for EPICSIMGInterface messaging. */
	static LoggingSystem static_messenger;
	std::string ownerName;
};

#endif //EPICS_IMG_INTERFACE_H_