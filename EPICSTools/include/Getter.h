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

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

//union ValueHolder
//{
//	double double_value;
//	int int_value;
//	unsigned short ushort_value;
//	std::string str_value;
//};

class Getter
{
public:
	//may need to add a function that explicitly says what type to return
	// based on the EPICS CHTYPE when we do setupChannels()....
	Getter();
	Getter(const std::string& pvStr);
	Getter(const std::string& pvStr, const STATE& mode);
	Getter(const Getter& getter);
	void getPythonTypeFromEPICS();
	void setupChannels();
	//ValueHolder currentValue;
	EPICSInterface_sptr epicsInterface;
	boost::python::object getValue_Py();
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToGet;
	boost::python::object pyValue;

};




#endif // GETTER_H