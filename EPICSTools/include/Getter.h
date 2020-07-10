#ifndef GETTER_H
#define GETTER_H

#include <EPICSInterface.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <LoggingSystem.h>
#include <PV.h>
#include <string>
#include <vector>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

class Getter
{
public:
	Getter();
	Getter(const std::string& pvStr);
	Getter(const std::string& pvStr, const STATE& mode);
	Getter(const Getter& getter);
	EPICSInterface_sptr epicsInterface;
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToGet;
	double value;

};

#endif // GETTER_H