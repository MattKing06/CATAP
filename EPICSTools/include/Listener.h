#ifndef LISTENER_H
#define LISTENER_H

#include <EPICSInterface.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <LoggingSystem.h>
#include <PV.h>
#include <UpdateFunctions.h>
#include <string>
#include <vector>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

class Listener
{
public:
	Listener();
	Listener(std::string pvStr);
	Listener(std::string pvStr, STATE mode);
	Listener(const Listener& listener);
	UpdateFunctionHolder updateFunctions;
	void setupChannels();
	void setValue(double value);
	double getValue();
	EPICSInterface_sptr epicsInterface;
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToMonitor;
	double value;
};

#endif //LISTENER_H