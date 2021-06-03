#ifndef EPICS_TOOLS_H
#define EPICS_TOOLS_H

#include <GlobalStateEnums.h>
#include <GlobalConstants.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>
#include <Listener.h>
#include <Getter.h>
#include <Putter.h>

/** @defgroup epicsTools EPICSTools
	@brief A collection of classes for monitoring, getting, and putting, to EPICS PVs.

	* EPICS Tools is designed to provide a low-level way of interacting with EPICS PVs instead of using their Hardware equivalent.
	* These tools include Monitors (Listeners), Getters, and Putters.
	*
	* Monitors, Getters, and Putters are essentially wrapping around ca_monitor, ca_get/ca_get_array, and ca_put/ca_put_array respectively.
@{*/

/*! EPICSTools
@brief A class for setting up monitoring, getting, and putting to EPICS PVs.
	   This class is also an interface to the Listener, Getter, and Putter classes
	   which are stored as objects in the listenerMap, getterMap, and putterMap respectively.
*/
class EPICSTools
{
public:
	/*! Default Constructor: initializes listener, getter, and putter maps*/
	EPICSTools();
	/*! Custom Constructor: initializes listener, getter, and putter maps as well as the mode STATE (PHYSICAL,VIRTUAL,OFFLINE)*/
	EPICSTools(STATE mode);
	/*! Copy Constructor: copies map entries over to new EPICSTools instance*/
	EPICSTools(const EPICSTools& copyEPICSTools);
	/*! Constructs a Listener object using the pv and then passes this to createSubscription in EPICSInterface
		to begin monitoring. The Listener object is then stored in the listenerMap with pv as the key.
		@param[in] pv : The PV to monitor*/
	bool monitor(const std::string& pv);
	/*! Constructs multiple Listener objects using the pvList and then passes these to createSubscription in EPICSInterface
		to begin monitoring. The Listener objects are then stored in the listenerMap with their pv as the key.
		@param[in] pvList : vector of PVs to monitor*/
	void monitor(std::vector<std::string> pvList);
	/*! Used for EPICSTools python module, python list of PVs is converted to vector for use by monitor(std::vector)*/
	void monitor_Py(boost::python::list pvList);
	/*! Removes the subscription for the supplied pv but keeps the Listener object in listenerMap. 
		If the pv cannot be found in the listenerMap, then nothing is done.
		@param[in] pv : The PV to stop monitoring.*/
	void stopMonitoring(const std::string& pv);
	/*! Sets up the subscription for the supplied pv again. If the pv cannot be found
		in the listenerMap, then nothing is done.
		@param[in] pv : The PV to start monitoring again*/
	void restartMonitoring(const std::string& pv);
	/*! Retrieves the Listener object in listenerMap via supplied pv. If there is no entry in the map,
		a Listener object is constructed using the supplied pv and added to the map; this object is then
		returned
		@param[in] pv : Name of the Listener object to return
		@param[out] Listener : The listener object associated with supplied pv*/
	Listener& getMonitor(std::string pv);
	/*! Retrieves the Listener objects in listenerMap for each pv in the vector.
		If there is no entry in the map, a Listener object is constructed using the supplied pv and added to the map;
		these objects are then returned.
		@param[in] names : Names of the Listener objects to return
		@param[out] map<string,Listener> : name and listener object (key,value) map*/
	std::map<std::string, Listener> getMonitors(std::vector<std::string> names);
	/*! Get a vector of all the PV names currently being monitored from listenerMap*/
	std::vector<std::string> getAllMonitorNames();
	/*! Get a python list of all the PV names currently being monitored from listenerMap*/
	boost::python::list getAllMonitorNames_Py();
	/*! Get the current buffer of single values from Listener as a python list 
		@param[in] pv : The name of the Listener object
		@param[out] buffer : The current value buffer from Listener*/
	boost::python::list getBuffer_Py(const std::string& pv);
	/*! Get the current buffer of array values from Listener as a python list
		@param[in] pv : The name of the Listener object
		@param[out] buffer : The current array buffer from Listener*/
	boost::python::list getArrayBuffer_Py(const std::string& pv);
	/*! Get multiple value buffers from Listener objects as a python dict
		@param[in] pvList : The names of the Listener objects
		@param[out] buffer dict : key = PV name, value = buffer*/
	boost::python::dict getBuffer_Py(boost::python::list pvList);
	/*! Get the mean average of the values held in the buffer of a Listener from listenerMap.
		@param[in] pv : name of Listener object
		@param[out] average : the mean average of the values in buffer*/
	double getBufferAverage_Py(const std::string& pv);
	/*! Get the standard deviation of the values held in the buffer of a Listener from listenerMap.
		@param[in] pv : name of Listener object
		@param[out] average : the standard deviation of the values in buffer*/
	double getBufferStdDeviation(const std::string& pv);
	/*! Get the standard deviations of values held in buffers of Listener objects as a python dictionary
		@param[in] pvList : list of Listener names
		@param[out] standard deviations dict : key = PV, value = standard deviation for buffer */
	boost::python::dict getBufferStdDeviation_Py(boost::python::list pvList);
	/*! Get the mean averages of values held in buffers of Listener objects as a python dictionary
		@param[in] pvList : list of Listener names
		@param[out] averages dict : key = PV, value = mean averages for buffer */
	boost::python::dict getBufferAverage_Py(boost::python::list pvList);
	/*! Calls ca_get from Getter object in getterMap, if there is no entry for the pv in getterMap;
		a Getter object is created and get is called from that object.
		@param[in] pv : The name of the PV to ca_get
		@param[out] value : The value returned from EPICS using ca_get*/
	template <typename T>
	T get(const std::string& pv);
	/*! Calls ca_put from Putter object in putterMap, if there is no entry for the pv in putterMap;
		a Putter object is created and put is called from that object.
		@param[in] pv : The name of the PV to ca_put
		@param[in] value : The value to put to that PV */
	template <typename T>
	void put(const std::string& pv, T value);
	/*! Get the value of an EPICS PV as a python object
		@param[in] pv : The name of the PV to get
		@param[out] value : The current value of the PV in EPICS as a python object.*/
	boost::python::object get_Py(const std::string& pv);
	/*! Get the value of an EPICS array PV as a python object
		@param[in] pv : The name of the PV to get
		@param[out] value : The current array of the PV in EPICS as a python object.*/
	boost::python::list getArray_Py(const std::string& pv);

	/*! Get the value of an EPICS array PV as a python object, with user specified array size. 
		(Useful when you know you don't need the full array data from EPICS)
		@param[in] pv : The name of the PV to get
		@param[out] value : The current array of the PV in EPICS as a python object.*/
	boost::python::list getArray_Py2(const std::string& pv, unsigned long COUNT);

	/*! Get the values of a list of EPICS PVs as a python dictionary
		@param[in] pvList : The names of the PVs to get
		@param[out] value dict : Key = PV, Value = EPICS PV value.*/
	boost::python::dict get_Py(boost::python::list pvList);
	/*! Set the value of an EPICS PV using a value stored in a python object
		@param[in] pv : The name of the PV to set
		@param[in] value : The python object value to set the PV to*/
	void put_Py(const std::string& pv, boost::python::object value);
	/*! Set the values of multiple EPICS PVs using names and values stored in a python dictionary
		@param[in] pvAndValueDict : A python dictionary containing the PVs and corresponding values to set */
	void put_Py(boost::python::dict pvAndValueDict);
	/*! Set the value of an EPICS array PV using an array stored in a python list
		@param[in] pv : The name of the PV to set
		@param[in] value : The python list to set the array PV to*/
	void putArray_Py(const std::string& pv, boost::python::list py_Array);
	/*! Retrieves the number of elements stored in a PV record
		@param[in] pv : The name of the PV
		@param[out] count : The number of elements stored in that PV record*/
	int getCount(const std::string& pv);
	/*! Retrieves the type of a PV record for getters and putters
		@param[in] pv : The name of the PV
		@param[out] type : The type of the PV record as a string*/
	std::string getType(const std::string& pv);
	/*! Retrieves the type of a PV record for listeners
		@param[in] pv : The name of the PV
		@param[out] type : The monitor type of the PV record as a string*/
	std::string getMonitorType(const std::string& pv);
	/*! Retrieves the PV, COUNT, and TYPE associated with the PV record
		@param[in] pv : The name of the PV 
		@param[out] info dictionary : PV, COUNT, TYPE and their corresponding values.*/
	boost::python::dict getEPICSInfo(const std::string& pv);
	/*! Determines whether to use the PHYSICAL (VELA/CLARA) EPICS or the VIRTUAL EPICS implementation*/
	STATE mode;
	/*! Stores the Listener objects, keyed by PV*/
	std::map<std::string, Listener> listenerMap;
	/*! Stores the Getter objects, keyed by PV*/
	std::map<std::string, Getter> getterMap;
	/*! Stores the Putter objects, keyed by PV*/
	std::map<std::string, Putter> putterMap;
};



template<typename T>
inline T EPICSTools::get(const std::string& pv)
{
	if (GlobalFunctions::entryExists(getterMap, pv))
	{
		return getterMap[pv].getValue<T>();
	}
	else
	{
		getterMap[pv] = Getter(pv);
		return getterMap[pv].getValue<T>();
	}
}

template<typename T>
inline void EPICSTools::put(const std::string& pv, T value)
{
	if (GlobalFunctions::entryExists(putterMap, pv))
	{
		putterMap[pv].put<T>(value);
	}
	else
	{
		putterMap[pv] = Putter(pv);
		putterMap[pv].put<T>(value);
	}
}

#endif //EPICS_TOOLS_H

/**@}*/
