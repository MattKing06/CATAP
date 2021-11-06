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

/** @addtogroup epicsTools
 *@{*/
 /*! Getter
	@brief A class for getting the value stored in the records of EPICS PVs.
 */

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;
typedef std::pair<epicsTimeStamp, std::vector<boost::variant<double, long, float, unsigned short, short, std::string>>> timeStampVector;
typedef std::pair<epicsTimeStamp, boost::variant<double, long, float, unsigned short, short, std::string>> timeStampValue;

class Getter
{
public:
	/*! Default constructor: not used.*/
	Getter();
	/*! Custom constructor : Sets up EPICS connections using VIRTUAL mode for the given pv.
		@param[in] pv : The PV that will be used for ca_get calls. */
	Getter(const std::string& pvStr);
	/*! Custom constructor : Sets up EPICS connections using the given mode for the given pv.
			@param[in] pv : The PV that will be used for ca_get calls.
			@param[in] mode : VIRTUAL (prepends VM- to given pv), PHYSICAL (uses given pv)*/
	Getter(const std::string& pvStr, const STATE& mode);
	/*! Copy constructor : copies content of provided getter object to current instance
		@param[in] copyGetter : the getter object to copy from*/
	Getter(const Getter& copyGetter);
	/*! Creates a CHID for the PV associated with the Getter. The CHID is then used to set the CHTYPE, COUNT, etc.*/
	void setupChannels();
	/*! Prepends VM- to the given pv
		@param[in] pv : The name of the PV to virtualize
		@param[out] virtualPV : pv with VM- prepended to it*/
	std::string getEPICSPVName(const std::string& pv);
	/*! Gets the current value stored in EPICS for the associated PV. currentValue is then set to the value retrieved 
		from EPICS using ca_get.*/
	void setValueFromEPICS();

	template<typename T>
	std::pair<std::string, T> getTimestampedValue();

	/*! Calls setValueFromEPICS and then returns the value stored in currentValue
		@param[out] value : The value stored in the EPICS record*/
	template <typename T>
	T getValue();
	/*! Calls setValueFromEPICS and then returns the array stored in currentArray
		@param[out] array : The array stored in the EPICS array record*/
	template <typename T>
	std::vector<T> getArray();
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
	/*! Returns true if currentValue is of type short */
	bool isShort();
	/*! Returns true if currentArray is of type long */
	bool isLongArray();
	/*! Returns true if currentArray is of type double */
	bool isDoubleArray();
	/*! Returns true if currentArray is of type int */
	bool isIntArray();
	/*! Returns true if currentArray is of type unsigned short */
	bool isEnumArray();
	/*! Returns true if currentArray is of type short */
	bool isShortArray();
	/*! Returns true if currentArray is of type std::string */
	bool isStringArray();
	/*! Returns true if currentArray is of type float */
	bool isFloatArray();
	/*! returns the value stored in currentValue as a python object */
	boost::python::object getValue_Py();

	boost::python::dict getTimestampedValue_Py();

	boost::python::dict getTimestampedArray_Py();

	template<typename T>
	std::pair<epicsTimeStamp, std::vector<T>> getTimestampedArray();

	/*! returns the array stored in currentArray as a python list */
	boost::python::list getArray_Py();
	/*! stores the current value in the single-valued EPICS record associated with getter */
	timeStampValue currentValue;
	/*! stores the current array in the EPICS record associated with getter */
	timeStampVector currentArray;
	/*! For Accessing common EPICS-related functionality*/
	EPICSInterface_sptr epicsInterface;
	/*! Tells us whether to use the CLARA control system (PHYSICAL),
		Virtual Machine EPICS (VIRTUAL)*/
	STATE mode;
	/*! For printing messages and debug messages to stdout*/
	LoggingSystem messenger;
	/*! The PVStruct used for all EPICS-related functions, holds all EPICS information (CHID, CHTYPE, etc.)*/
	pvStruct pv;
	/*! The PV associated to this instance of Getter */
	std::string pvToGet;
};

template <typename T>
inline T Getter::getValue()
{
	//add try catch here to deal with bad_cast exceptions
	// from boost::variant.
	setValueFromEPICS();
	if (pv.COUNT == 1)
	{
		return boost::get<T>(currentValue.second);
	}
	else
	{
		messenger.printMessage(pvToGet, " count is more than one. Try calling getArray.");
		return T();
	}

}

template <typename T>
inline std::pair<std::string, T> Getter::getTimestampedValue()
{
	setValueFromEPICS();
	if (pv.COUNT == 1)
	{
		return std::pair<std::string, T>(epicsInterface->getEPICSTime(currentValue.first), boost::get<T>(currentValue.second));
	}
	else
	{
		messenger.printMessage(pvToGet, " count is more than one. Try calling getTimestampedArray.");
		return std::pair<std::string, T>();
	}
}

template<typename T>
inline std::vector<T> Getter::getArray()
{
	setValueFromEPICS();
	if (pv.COUNT > 1)
	{
		std::vector<T> returnVector;
		for (auto& item : currentArray.second)
		{
			returnVector.push_back(boost::get<T>(item));
		}
		return returnVector;
	}
}

template<typename T>
inline std::pair<epicsTimeStamp, std::vector<T>> Getter::getTimestampedArray()
{
	setValueFromEPICS();
	if (pv.COUNT > 1)
	{
		std::pair<epicsTimeStamp, std::vector<T>> returnVector;
		returnVector.first = currentArray.first;
		for (auto& item : currentArray.second)
		{
			returnVector.second.push_back(boost::get<T>(item));
		}
		return returnVector;
	}
	else
	{
		return std::pair<epicsTimeStamp, std::vector<T>>();
	}
}



#endif // GETTER_H

/**@}*/