#ifndef LISTENER_H
#define LISTENER_H

#include <EPICSInterface.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <LoggingSystem.h>
#include <PV.h>
#include <UpdateFunctions.h>
#include <PythonTypeConversions.h>
#include <string>
#include <vector>
#include <boost/circular_buffer.hpp>
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
	boost::variant<double,float,long,int,unsigned short,std::string> currentValue;
	boost::circular_buffer<boost::variant<double, float, long, int, unsigned short, std::string> > currentBuffer;
	void setBufferSize(int size);
	template<typename T>
	boost::circular_buffer<T> getBuffer();
	boost::python::list getBuffer_Py();
	double getBufferAverage();
	bool isDoubleBuffer();
	bool isIntBuffer();
	bool isLongBuffer();
	bool isEnumBuffer();
	bool isStringBuffer();
	bool isFloatBuffer();
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

template<typename T>
inline boost::circular_buffer<T> Listener::getBuffer()
{
	auto returnBuffer = boost::get<boost::circular_buffer<T>>(currentBuffer);
	return returnBuffer;
}


#endif //LISTENER_H

