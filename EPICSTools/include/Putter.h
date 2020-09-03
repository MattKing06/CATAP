#ifndef PUTTER_H
#define PUTTER_H

#include <EPICSInterface.h>
#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <LoggingSystem.h>
#include <PV.h>
#include <string>
#include <vector>
#include <boost/make_shared.hpp>
#include <boost/python.hpp>

/** @addtogroup epicsTools
 *@{*/
 /*! Putter
	@brief A class for setting the value stored in the records of EPICS PVs.
 */
class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

class Putter
{
public:
	/*! Default constructor: not used.*/
	Putter();
	/*! Custom constructor : */
	Putter(const std::string& pv);
	Putter(const std::string& pv, const STATE& mode);
	Putter(const Putter& copyPutter);
	void setupChannels();
	/*! Prepends VM- to the given pv
		@param[in] pv : The name of the PV to virtualize
		@param[out] virtualPV : pv with VM- prepended to it*/
	std::string getEPICSPVName(const std::string& pv);
	//With PUT function, will have to check the EPICS CHTYPE 
	//and use appropriate boost::python::Extract template to put value
	//from a python object..

	void put_Py(boost::python::object pyValue);
	void putArray_Py(boost::python::list pyValue);
	template <typename T>
	void put(T value);
	template <typename T>
	void put(std::vector<T> value);
	EPICSInterface_sptr epicsInterface;
	STATE mode;
	LoggingSystem messenger;
	pvStruct pv;
	std::string pvToPut;
};

template<typename T>
inline void Putter::put(T value)
{
	epicsInterface->putValue<T>(pv, value);
}

template<typename T>
inline void Putter::put(std::vector<T> value)
{
	epicsInterface->putArray<T>(pv, value);
}

#endif //PUTTER_H

/**@}*/