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
typedef std::pair<epicsTimeStamp, std::vector<boost::variant<double, long, float, unsigned short, short, std::string>>> timeStampVector;
typedef std::pair<epicsTimeStamp, boost::variant<double, long, float, unsigned short, short, std::string>> timeStampValue;

namespace ListenerToPy
{
	struct convert_to_py : public boost::static_visitor<boost::python::object>
	{
		template<typename T>
		boost::python::object operator()(T value) const { return static_cast<boost::python::object>(value); }
	};
}

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
	
	~Listener();
	void attachToInitialContext();
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
	int getBufferCapacity();
	int getBufferSize();
	int getArrayBufferCapacity();
	int getArrayBufferSize();
	void initialiseCurrentValue(const pvStruct& pv);
	void initialiseCurrentArray(const pvStruct& pv);
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
	void setValue(const std::pair<epicsTimeStamp, T>& value);
	/*! Used by updateFunction to set currentArray from EPICS
		@param[in] valueArray : valueArray to set currentArray to.*/
	template<typename T>
	void setArray(const std::pair<epicsTimeStamp, std::vector<T>>& valueArray);
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
	boost::python::dict getValue_Py();
	/*! Get the most recent array of PV array record from EPICS as a python list
		@param[out] array : Most recent array in EPICS as a python list*/
	boost::python::dict getArray_Py();
	/*! Get the average over the current array 
		@param[out] average: Mean average of the current array values*/
	template<typename T>
	double getArrayAverage();

	/*! Get the mean average over the current array as python object
	@param [out] average: Return the mean average of the current array as a boost::python::object*/
	boost::python::object getArrayAverage_Py();

	/*! Resizes the single-value buffer from the default (10 entries) to supplied size
		@param[in] size : new size of single-value buffer */
	void setBufferSize(int size);
	/*! Resizes the array-value buffer from the default (10 arrays) to supplied size
		@param[in] size : new size of array-value buffer */
	void setArrayBufferSize(int size);
	/*! Get an array of the mean averages of all arrays in the buffer 
		@param[out] averageArray : Array containing the mean average for all arrays in the arrayBuffer*/
	template <typename T>
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
	boost::python::dict getBuffer_Py();

	void clearBuffer();

	void clearArrayBuffer();

	/*! Get the buffer of array-values (default size is 10 arrays)
		@param[out] buffer : circular_buffer of arrays from EPICS. */
	template<typename T>
	boost::circular_buffer<std::vector<T>> getArrayBuffer();
	/*! Get the buffer of array-values (default size is 10 arrays) as a python list
		@param[out] buffer : python list of buffered arrays from EPICS. */
	boost::python::dict getArrayBuffer_Py();
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
	/*! Returns true if currentValue is of type short*/
	bool isShort();
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
	/*! Returns true if currentArray is of type short */
	bool isShortArray();
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
	/*! Returns true if currentBuffer is of type short */
	bool isShortBuffer();
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
	/*! Returns true if currentArrayBuffer is of type short */
	bool isShortArrayBuffer();
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
	timeStampValue currentValue;
	/*! stores the current array in the EPICS record associated with Listener */
	timeStampVector currentArray;
	/*! buffer for single-values updated from EPICS*/
	boost::circular_buffer<timeStampValue> currentBuffer;
	/*! buffer for array-values updated from EPICS*/
	boost::circular_buffer<timeStampVector> currentArrayBuffer;
};



template<typename T>
inline void Listener::setValue(const std::pair<epicsTimeStamp, T>& value)
{
	currentValue.first = value.first;
	currentValue.second = static_cast<T>(value.second);
}

template<typename T>
inline void Listener::setArray(const std::pair<epicsTimeStamp, std::vector<T>>& valueArray)
{

	currentArray.first = valueArray.first;
	currentArray.second.clear();
	currentArray.second.assign(valueArray.second.begin(), valueArray.second.end());
}

template<typename T>
inline T Listener::getValue()
{
	return boost::get<T>(currentValue.second);
}

template<typename T>
inline std::vector<T> Listener::getArray()
{
	std::vector<T> returnArray;
	for (auto item : currentArray.second)
	{
		returnArray.push_back(boost::get<T>(item));
	}
	return returnArray;
}

template<typename T>
inline double Listener::getArrayAverage()
{
	if (!isStringArray() && !isEnumArray())
	{
		std::vector<T> currentArrayTyped = getArray<T>();
		double average = 0.0f;
		for (auto&& item : currentArrayTyped)
			average += static_cast<double>(item);
		return average / currentArrayTyped.size();
	}
}

template<typename T>
inline boost::circular_buffer<T> Listener::getBuffer()
{
	boost::circular_buffer<T> returnBuffer;
	for (auto& item : currentBuffer)
	{
		returnBuffer.push_back(boost::get<T>(item.second));
	}
	return returnBuffer;
}

template<typename T>
inline boost::circular_buffer<std::vector<T>> Listener::getArrayBuffer()
{
	boost::circular_buffer<std::vector<T>> floatBuff(currentArrayBuffer.capacity());
	for (auto& timeVectorPair : currentArrayBuffer)
	{
		std::vector<T> bufferVec(currentArray.second.size());
		for (auto& item : timeVectorPair.second)
		{
			bufferVec.push_back(boost::get<T>(item));
		}
		floatBuff.push_back(bufferVec);
	}
	return floatBuff;
//	return boost::circular_buffer<std::vector<T>>(currentArrayBuffer);
}

#endif //LISTENER_H
/**@}*/
