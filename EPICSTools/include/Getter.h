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
 */

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
	/*! Prepends VM- to the given pv
		@param[in] pv : The name of the PV to virtualize
		@param[out] virtualPV : pv with VM- prepended to it*/
	std::string getEPICSPVName(const std::string& pv);
	void setValueFromEPICS();
	template <typename T>
	T getValue();
	template <typename T>
	std::vector<T> getArray();
	// need to move these over EPICSTools
	// and use them with PV arguments.
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
	boost::variant<double,int,long,float,unsigned short,std::string> currentValue;
	std::vector<boost::variant<double, int, long, float, unsigned short, std::string> > currentArray;
	EPICSInterface_sptr epicsInterface;
	boost::python::object getValue_Py();
	boost::python::list getArray_Py();
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
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
		return boost::get<T>(currentValue);
	}
	else
	{
		messenger.printMessage(pvToGet, " count is more than one. Try calling getArray.");
		return T();
	}

}

template<typename T>
inline std::vector<T> Getter::getArray()
{
	setValueFromEPICS();
	if (pv.COUNT > 1)
	{
		std::vector<T> returnVector;
		for (auto& item : currentArray)
		{
			returnVector.push_back(boost::get<T>(item));
		}
		return returnVector;
	}
}

#endif // GETTER_H

/**@}*/