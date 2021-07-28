#ifndef CHARGE_H_
#define CHARGE_H_
#include "LoggingSystem.h"
#include <RunningStats.h>
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#ifndef EPICS_CHARGE_INTERFACE_H_
#include "EPICSChargeInterface.h"
#include "PythonTypeConversions.h"
#endif //EPICS_CHARGE_INTERFACE_H_
#include "ChargePVRecords.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>


class EPICSChargeInterface;
typedef boost::shared_ptr<EPICSChargeInterface> EPICSChargeInterface_sptr;
/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control charge diagnostics on VELA/CLARA.
	 A collection of these classes will be stored in map in the ChargeFactory class.
 */
class Charge : public Hardware
{
public:
	/*! Default constructor call for Charge object*/
	Charge();
	/*! Custom constructor for Charge object
		@param[in] chargeParametersAndValuesMap: strings defining parameters extracted from YAML config files
		@param[in] mode: Defines the STATE of Charge we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)
	*/
	Charge(const std::map<std::string, std::string>& chargeParametersAndValuesMap, STATE mode);
	/*! Copy constructor call for Charge object*/
	Charge(const Charge& copyCharge);
	/*! EPICSChargeInterface for Charge-specifc calls to EPICS*/
	EPICSChargeInterface_sptr epicsInterface;
	/*! Returns the name of the Charge object.
	@param[out] name: the name of the charge diagnostic.
	*/
	std::string getName() const;
	/*! A function to retrieve the aliases for a given Charge object
		@param[out] vector of aliases that the Charge object is also known by*/
	std::vector<std::string> getAliases() const;
	/*! Gets the type of Charge diagnostic.
		@param[out] type: the Charge type. (Should probably be converted to TYPE rather than string)*/
	std::string getChargeDiagnosticType() const;
	/*! A map for storing the parameters extracted from YAML config files and their values */

	std::map<std::string, std::string> chargeParametersAndValuesMap;
		
	/*! Goes through all PV records stored in ChargeRecords namespace, creates pvStructs and sets up their record/fullPVName before
		inserting them into a vector of pvStructs which is later used by the ChargeFactory to setup connections to EPICS.
		*/
	void setPVStructs();		

	LoggingSystem messenger;
	/*! turns debug messaging on for this Charge instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this Charge instance*/
	void debugMessagesOff();
	/*! turns messaging on for this Charge instance*/
	void messagesOn();
	/*! turns messaging off for this Charge instance*/
	void messagesOff();

	/*! is the Charge object monitoring? */
	bool monitoringData = false;
	/*! is the Charge object monitoring any PV? (see monitorForNShots)
	@param[out] bool: true if yes.*/
	bool ismonitoring() const;
	/*! is the Charge monitoring the charge PV? (see monitorForNShots)
	@param[out] bool: true if yes.*/
	bool ismonitoringQ() const;
	/*! is the Charge buffer full?
	@param[out] bool: true if yes.*/
	bool isQBufferFull() const;
	/*! get the current charge reading.
	@param[out] value: bunch charge.*/
	double getQ() const;
	/*! get the beamline position of the charge diagnostic
	@param[out] value: position.*/ 
	double getPosition() const;
	/*! get the size of the buffer of charge values for the charge diagnostic
	@param[out] value: buffer size.*/
	size_t getBufferSize() const;
	/*! get the vector of charge values (after using monitorForNShots)
	@param[out] values: charge vector.*/
	std::vector< double > getQVector() const;
	/*! get the buffer of charge values.
	@param[out] values: charge buffer.*/
	boost::circular_buffer< double > getQBuffer() const;
	/*! get the vector of charge values (after using monitorForNShots) as a python list
	@param[out] values: charge vector.*/
	boost::python::list getQVector_Py() const;
	/*! get the buffer of charge values as a python list.
	@param[out] values: charge buffer.*/
	boost::python::list getQBuffer_Py() const;
	/*! set charge value (virtual only).
	@param[in] double: charge value.
	@param[out] bool: true if it worked.*/
	bool setQVirtual(const double& value);
	/*! set charge value when updated from EPICS.
	@param[in] double: charge value
	@param[out] bool: true if it worked.*/
	bool setQ(const double& value);
	/*! check that the buffer is updating.
	@param[in] buf: charge buffer.
	@param[out] bool: true if yes.*/
	bool checkBuffer(boost::circular_buffer< double >& buf);
	/*! fill up a vector of charge values.
	@param[in] value: number of shots to monitor.*/
	void monitorForNShots(const size_t& value);
	/*! set charge vector size
	@param[in] value: desired vector size.*/
	void setBufferSize(const size_t& value);
	/*! set charge buffer size.
	@param[in] value: desired buffer size.*/
	void setVectorSize(const size_t& value);
	/*! empty all buffers.*/
	void clearBuffers();


	/*! charge value w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, double > q;
	/*! beamline position of charge diagnostic.*/
	double position;
	/*! number of charge readings since clearBuffers.*/
	unsigned int qshots;
	/*! size of buffer(s).*/
	size_t bufferSize;
	/*! size of vector(s).*/
	size_t vectorSize;
	/*! charge value buffer.*/
	boost::circular_buffer< double > qBuffer;
	/*! charge vector.*/
	std::vector< double > qVector;
	/*! true if charge is monitoring.*/
	bool monitoringq;
	/*! state of charge diagnostic device.*/
	STATE chargeState;
	/*! name of charge diagnostic device.*/
	std::string name;
	/*! Running statistics for charge diagnostic, access to easy statistics*/
	RunningStats qStats;
	/*! Get the running stats back as a dict*/
	boost::python::dict getRunningStats_Py();
	/*! Get the running stats object` back directly*/
	RunningStats& getQRunningStats();
	/*! Set running stats max count .*/
	void setRunningStatSize(size_t new_size);
	/*! lear running stats data.*/
	void clearRunningStats();
	size_t getRunningStatCount();
	size_t getRunningStatSize()const;
	bool isRunningStatFull();
	/*! Get the current number of data values being used by the Running Stats.
* 	@param[out] size_t: number of data values.*/
	size_t getRunningStatNumDataValues()const;

	friend class ChargeFactory;
	friend class EPICSCameraInterface;
	friend class PILaserSystem;

protected:
	std::vector<std::string> aliases;
	std::string chargeType;
};
/**\copydoc Hardware*/
/**@}*/
#endif //CHARGE_H_