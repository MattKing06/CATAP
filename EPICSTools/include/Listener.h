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
#include <boost/variant.hpp>
#include <boost/python.hpp>
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
	template<typename T>
	void setValue(T value);
	template<typename T>
	T getValue();
	boost::python::object getValue_Py();
	EPICSInterface_sptr epicsInterface;
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToMonitor;
	boost::variant<double,float,int,STATE,std::string> currentValue;
	boost::python::object pyValue;
	int callCount;
};



template<typename T>
inline void Listener::setValue(T value)
{
	currentValue = value;
	pyValue = static_cast<boost::python::object>(value);
}

template<typename T>
inline T Listener::getValue()
{
	pyValue = static_cast<boost::python::object>(boost::get<T>(currentValue));
	return boost::get<T>(currentValue);

}


#endif //LISTENER_H

