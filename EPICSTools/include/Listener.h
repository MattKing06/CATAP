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

#define DEFAULT_BUFFER_SIZE 10

/** @addtogroup epicsTools
 *@{*/
/*! Listener
   @brief A class for monitoring the value stored in the records of EPICS PVs.
*/

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

struct convert_to_py : public boost::static_visitor<boost::python::object>
{
	template<typename T>
	boost::python::object operator()(T value) const { return static_cast<boost::python::object>(value); }
};


class Listener
{
public:
	/*! Default constructor: not used.*/
	Listener();
	/*! Custom constructor : Sets up EPICS connections using VIRTUAL mode for the given pv.
		@param[in] pv : The PV that will be used for ca_monitor subscription. */
	Listener(const std::string& pvStr);
	/*! Custom constructor : Sets up EPICS connections using the given mode for the given pv.
			@param[in] pv : The PV that will be used for ca_monitor subscription.
			@param[in] mode : VIRTUAL (prepends VM- to given pv), PHYSICAL (uses given pv)*/
	Listener(const std::string& pvStr, const STATE& mode);
	/*! Copy constructor : copies content of provided listener object to current instance
		@param[in] copyGetter : the listener object to copy from*/
	Listener(const Listener& listener);
	/*! Used to set the appropriate callback function that updates the current value held by Listener*/
	UpdateFunctionHolder updateFunctions;
	/*! Creates a CHID for the PV associated with the Listener. 
		The CHID is then used to set the CHTYPE, COUNT, etc.
		The subscription is setup by EPICSTools, not the Listener class.*/
	void setupChannels();
	/*! Prepends VM- to the given pv
		@param[in] pv : The name of the PV to virtualize
		@param[out] virtualPV : pv with VM- prepended to it*/
	std::string getEPICSPVName(const std::string& pv);

	void initialiseCurrentValue(const pvStruct& pv);

	/*! Returns the bool status of whether a PV is connected or not
		@param[in] pv : The name of the PV to check
		@param[out] status : whether or not the PV is connected */
	bool isConnected();
	/*! Removes the subscription to the EPICS PV record*/
	void stopListening();
	/*! Restarts the subscription to the EPICS PV record*/
	void startListening();
	/*! Used by updateFunction to set currentValue from EPICS
		@param[in] value : value to set currentValue to.*/
	template<typename T>
	void setValue(T value);
	/*! Used by updateFunction to set currentArray from EPICS
		@param[in] valueArray : valueArray to set currentArray to.*/
	template<typename T>
	void setArray(const std::vector<T>& valueArray);
	/*! Get the most recent value of PV record from EPICS
		@param[out] value : Most recent value in EPICS*/
	template<typename T>
	T getValue();
	/*! Get the most recent array of PV array record from EPICS
		@param[out] array : Most recent array in EPICS*/
	template<typename T>
	std::vector<T> getArray();
	/*! Get the most recent value of PV record from EPICS as a python object
		@param[out] value : Most recent value in EPICS as a python object*/
	boost::python::object getValue_Py();
	/*! Get the most recent array of PV array record from EPICS as a python list
		@param[out] array : Most recent array in EPICS as a python list*/
	boost::python::list getArray_Py();
	/*! Resizes the single-value buffer from the default (10 entries) to supplied size
		@param[in] size : new size of single-value buffer */
	void setBufferSize(int size);
	/*! Resizes the array-value buffer from the default (10 arrays) to supplied size
		@param[in] size : new size of array-value buffer */
	void setArrayBufferSize(int size);
	/*! Get an array of the mean averages of all arrays in the buffer 
		@param[out] averageArray : Array containing the mean average for all arrays in the arrayBuffer*/
	std::vector<double> getArrayBufferAverageArray();
	/*! Get an array of the mean averages of all arrays in the buffer as a python list
		@param[out] averageArray : Array containing the mean average for all arrays in the arrayBuffer as a python list*/
	boost::python::list getArrayBufferAverageArray_Py();
	/*! Get the buffer of single-values (default size is 10 elements)
		@param[out] buffer : circular_buffer of values from EPICS. */
	template<typename T>
	boost::circular_buffer<T> getBuffer();
	/*! Get the buffer of single-values (default size is 10 elements) as python list
		@param[out] buffer : python list of buffered values from EPICS. */
	boost::python::list getBuffer_Py();
	/*! Get the buffer of array-values (default size is 10 arrays)
		@param[out] buffer : circular_buffer of arrays from EPICS. */
	template<typename T>
	boost::circular_buffer<std::vector<T>> getArrayBuffer();
	/*! Get the buffer of array-values (default size is 10 arrays) as a python list
		@param[out] buffer : python list of buffered arrays from EPICS. */
	boost::python::list getArrayBuffer_Py();
	/*! Get the mean average of single-values stored in the single-value buffer 
		@param[out] bufferAverage : mean average of values in buffer. */
	double getBufferAverage();
	/*! Get the standard deviation of single-values stored in the single-value buffer
		@param[out] stdDev : standard deviation of values in buffer. */
	double getBufferStdDeviation();
	/*! Returns true if currentValue is of type long */
	bool isLong();
	/*! Returns true if currentValue is of type double */
	bool isDouble();
	/*! Returns true if currentValue is of type int */
	bool isInt();
	/*! Returns true if currentValue is of type unsigned short */
	bool isEnum();
	/*! Returns true if currentValue is of type std::string */
	bool isString();
	/*! Returns true if currentValue is of type float */
	bool isFloat();
	/*! Returns true if currentArray is of type long */
	bool isLongArray();
	/*! Returns true if currentArray is of type double */
	bool isDoubleArray();
	/*! Returns true if currentArray is of type int */
	bool isIntArray();
	/*! Returns true if currentArray is of type unsigned short */
	bool isEnumArray();
	/*! Returns true if currentArray is of type std::string */
	bool isStringArray();
	/*! Returns true if currentArray is of type float */
	bool isFloatArray();
	/*! Returns true if currentBuffer is of type double */
	bool isDoubleBuffer();
	/*! Returns true if currentBuffer is of type int */
	bool isIntBuffer();
	/*! Returns true if currentBuffer is of type long */
	bool isLongBuffer();
	/*! Returns true if currentBuffer is of type unsigned short */
	bool isEnumBuffer();
	/*! Returns true if currentBuffer is of type std::string */
	bool isStringBuffer();
	/*! Returns true if currentBuffer is of type float */
	bool isFloatBuffer();
	/*! Returns true if currentArrayBuffer is of type double */
	bool isDoubleArrayBuffer();
	/*! Returns true if currentArrayBuffer is of type int */
	bool isIntArrayBuffer();
	/*! Returns true if currentArrayBuffer is of type long */
	bool isLongArrayBuffer();
	/*! Returns true if currentArrayBuffer is of type unsigned short */
	bool isEnumArrayBuffer();
	/*! Returns true if currentArrayBuffer is of type std::string */
	bool isStringArrayBuffer();
	/*! Returns true if currentArrayBuffer is of type float */
	bool isFloatArrayBuffer();
	/*! Number of times the currentValue/currentArray has been updated by EPICS */
	int callCount;
	/*! For Accessing common EPICS-related functionality*/
	EPICSInterface_sptr epicsInterface;
	/*! Tells us whether to use the CLARA control system (PHYSICAL),
		Virtual Machine EPICS (VIRTUAL)*/
	STATE mode;
	/*! For printing messages and debug messages to stdout*/
	LoggingSystem messenger;
	/*! The PVStruct used for all EPICS-related functions, holds all EPICS information (CHID, CHTYPE, etc.)*/
	pvStruct pv;
	/*! The PV associated to this instance of Listener */
	std::string pvToMonitor;
	/*! stores the current value in the single-valued EPICS record associated with Listener */
	boost::variant<double, float, long, int, unsigned short, std::string> currentValue;
	/*! stores the current array in the EPICS record associated with Listener */
	std::vector<boost::variant<double, float, long, int, unsigned short, std::string> > currentArray;
	/*! buffer for single-values updated from EPICS*/
	boost::circular_buffer<boost::variant<double, float, long, int, unsigned short, std::string> > currentBuffer;
	/*! buffer for array-values updated from EPICS*/
	boost::circular_buffer<std::vector<boost::variant<double, float, long, int, unsigned short, std::string>>> currentArrayBuffer;
};



template<typename T>
inline void Listener::setValue(T value)
{
	currentValue = static_cast<T>(value);
	std::cout << "set value type: " << currentValue.type().name() << std::endl;
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
