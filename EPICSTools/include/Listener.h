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
	void stopListening();
	void startListening();
	template<typename T>
	void setValue(T value);
	template<typename T>
	void setArray(const std::vector<T>& valueArray);
	template<typename T>
	T getValue();
	template<typename T>
	std::vector<T> getArray();
	boost::python::list getArray_Py();
	boost::python::object getValue_Py();
	EPICSInterface_sptr epicsInterface;
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToMonitor;
	boost::variant<double,float,long,int,unsigned short,std::string> currentValue;
	std::vector<boost::variant<double, float, long, int, unsigned short, std::string> > currentArray;
	boost::circular_buffer<boost::variant<double, float, long, int, unsigned short, std::string> > currentBuffer;
	void setBufferSize(int size);
	template<typename T>
	boost::circular_buffer<T> getBuffer();
	boost::python::list getBuffer_Py();
	double getBufferAverage();
	bool isLong();
	bool isDouble();
	bool isInt();
	bool isEnum();
	bool isString();
	bool isFloat();
	bool isLongArray();
	bool isDoubleArray();
	bool isIntArray();
	bool isEnumArray();
	bool isStringArray();
	bool isFloatArray();
	bool isDoubleBuffer();
	bool isIntBuffer();
	bool isLongBuffer();
	bool isEnumBuffer();
	bool isStringBuffer();
	bool isFloatBuffer();
	int callCount;
};



template<typename T>
inline void Listener::setValue(T value)
{
	currentValue = value;
}

template<typename T>
inline void Listener::setArray(const std::vector<T>& valueArray)
{
	if (!currentArray.empty())
	{
		currentArray.clear();
	}

	for (auto& item : valueArray)
	{
		currentArray.push_back(item);
	}
}

template<typename T>
inline T Listener::getValue()
{
	std::cout << "VALUE TYPE: " << currentValue.type().name() << std::endl;
	return boost::get<T>(currentValue);
}

template<typename T>
inline std::vector<T> Listener::getArray()
{
	std::vector<T> returnArray;
	for (auto& item : currentArray)
	{
		returnArray.push_back(boost::get<T>(item));
	}
	return returnArray;
}

template<typename T>
inline boost::circular_buffer<T> Listener::getBuffer()
{
	auto returnBuffer = boost::get<boost::circular_buffer<T>>(currentBuffer);
	return returnBuffer;
}


#endif //LISTENER_H

