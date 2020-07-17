#ifndef GETTER_H
#define GETTER_H

#include <EPICSInterface.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <LoggingSystem.h>
#include <PV.h>
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/python.hpp>

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

class Getter
{
public:
	Getter();
	Getter(const std::string& pvStr);
	Getter(const std::string& pvStr, const STATE& mode);
	Getter(const Getter& getter);
	void setupChannels();
	EPICSInterface_sptr epicsInterface;
	double getValue();
	boost::python::object getValue_Py();
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToGet;
	//boost::variant<int, double, float, STATE, std::string> value;
	double value;
	boost::python::object pyValue;

};



#endif // GETTER_H