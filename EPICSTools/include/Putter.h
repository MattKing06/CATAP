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

class EPICSInterface;
typedef boost::shared_ptr<EPICSInterface> EPICSInterface_sptr;

class Putter
{
public:
	Putter();
	Putter(const std::string& pv);
	Putter(STATE mode, const std::string& pv);
	Putter(const Putter& copyPutter);
	void setupChannels();

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
