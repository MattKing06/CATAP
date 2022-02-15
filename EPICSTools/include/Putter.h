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
#ifdef BUILD_PYTHON
#include <boost/python.hpp>
#include <PythonTypeConversions.h>
#endif //BUILD_PYTHON
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
	/*! Custom constructor : Sets up EPICS connections using VIRTUAL mode for the given pv.
		@param[in] pv : The PV that will be used for ca_put calls. */
	Putter(const std::string& pv);
	/*! Custom constructor : Sets up EPICS connections using the given mode for the given pv.
		@param[in] pv : The PV that will be used for ca_put calls. 
		@param[in] mode : VIRTUAL (prepends VM- to given pv), PHYSICAL (uses given pv)*/
	Putter(const std::string& pv, const STATE& mode);
	/*! Copy constructor : copies content of provided putter object to current instance
		@param[in] copyPutter : the putter object to copy from*/
	Putter(const Putter& copyPutter);
	/*! Creates a CHID for the PV associated with the Putter. The CHID is then used to set the CHTYPE, COUNT, etc.*/
	void setupChannels();
	/*! Prepends VM- to the given pv if mode is VIRTUAL (if VM is already prepended by user, pv is not changed).
		If mode is PHYSICAL the pv is left unchanged.
		@param[in] pv : The name of the PV to virtualize
		@param[out] virtualPV : pv with VM- prepended to it*/
	std::string getEPICSPVName(const std::string& pv);
	//With PUT function, will have to check the EPICS CHTYPE 
	//and use appropriate boost::python::Extract template to put value
	//from a python object..
#ifdef BUILD_PYTHON
	/*! Converts the python object to the appropriate type for the CHTYPE associated with the pv in Putter
		and calls ca_put using that value.
		@param[in] pyValue : The value to be set in EPICS*/
	void put_Py(boost::python::object pyValue);
	/*! Converts the python list to the appropriate type of array for the CHTYPE associated with the pv in Putter
		and calls ca_array_put using that array.
		@param[in] pyValue : The array to be set in EPICS*/
	void putArray_Py(boost::python::list pyValue);

#endif //BUILD_PYTHON
	/*! Uses ca_put to set the value for a pv record
		@param[in] value : The value to set*/
	template <typename T>
	void put(T value);
	/*! Uses ca_array_put to set the value for a pv array record
		@param[in] value : The array to set*/
	template <typename T>
	void put(std::vector<T> value);
	/*! For Accessing common EPICS-related functionality*/
	EPICSInterface_sptr epicsInterface;
	/*! Tells us whether to use the CLARA control system (PHYSICAL), 
		Virtual Machine EPICS (VIRTUAL)*/
	STATE mode;
	/*! For printing messages and debug messages to stdout*/
	LoggingSystem messenger;
	/*! The PVStruct used for all EPICS-related functions, holds all EPICS information (CHID, CHTYPE, etc.)*/
	pvStruct pv;
	/*! The PV associated to this instance of Putter */
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