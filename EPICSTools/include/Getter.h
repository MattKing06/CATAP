#ifndef GETTER_H
#define GETTER_H

#include <EPICSInterface.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <LoggingSystem.h>
#include <PV.h>
#include <string>
#include <vector>
#include <boost/make_shared.hpp>
#include <boost/python.hpp>
#include <boost/variant.hpp>
#include <boost/core/typeinfo.hpp>

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

class Getter
{
public:
	//may need to add a function that explicitly says what type to return
	// based on the EPICS CHTYPE when we do setupChannels()....
	Getter();
	Getter(const std::string& pvStr);
	Getter(const std::string& pvStr, const STATE& mode);
	Getter(const Getter& getter);
	void setupChannels();
	void setValueFromEPICS();
	template <typename T>
	T getValue();
	// need to move these over EPICSTools
	// and use them with PV arguments.
	bool isDouble();
	bool isInt();
	bool isEnum();
	bool isString();
	bool isFloat();
	boost::variant<double,int,float,unsigned short,std::string> currentValue;
	EPICSInterface_sptr epicsInterface;
	boost::python::object getValue_Py();
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToGet;
	//boost::python::object pyValue;

};

template <typename T>
inline T Getter::getValue()
{
	//add try catch here to deal with bad_cast exceptions
	// from boost::variant.
	setValueFromEPICS();
	std::cout << "GOT VALUE FROM EPICS " << std::endl;
	return boost::get<T>(currentValue);
}


#endif // GETTER_H