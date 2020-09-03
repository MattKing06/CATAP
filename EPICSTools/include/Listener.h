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


/** @addtogroup epicsTools
 *@{*/
 /*! Listener
 */

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

class Listener
{
public:
	Listener();
	Listener(const std::string& pvStr);
	Listener(const std::string& pvStr, const STATE& mode);
	Listener(const Listener& listener);
	UpdateFunctionHolder updateFunctions;
	void setupChannels();
	/*! Prepends VM- to the given pv
		@param[in] pv : The name of the PV to virtualize
		@param[out] virtualPV : pv with VM- prepended to it*/
	std::string getEPICSPVName(const std::string& pv);
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
	boost::circular_buffer<std::vector<boost::variant<double, float, long, int, unsigned short, std::string>>> currentArrayBuffer;
	void setBufferSize(int size);
	void setArrayBufferSize(int size);
	std::vector<double> getArrayBufferAverageArray();
	boost::python::list getArrayBufferAverageArray_Py();
	template<typename T>
	boost::circular_buffer<T> getBuffer();
	boost::python::list getBuffer_Py();
	template<typename T>
	boost::circular_buffer<std::vector<T>> getArrayBuffer();
	boost::python::list getArrayBuffer_Py();
	double getBufferAverage();
	double getBufferStdDeviation();
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
	bool isDoubleArrayBuffer();
	bool isIntArrayBuffer();
	bool isLongArrayBuffer();
	bool isEnumArrayBuffer();
	bool isStringArrayBuffer();
	bool isFloatArrayBuffer();
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

template<typename T>
inline boost::circular_buffer<std::vector<T>> Listener::getArrayBuffer()
{
	return boost::circular_buffer<std::vector<T>>(currentArrayBuffer);
}

#endif //LISTENER_H
/**@}*/
