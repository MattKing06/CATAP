#ifndef BPM_H_
#define BPM_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#include "EPICSBPMInterface.h"
#include "PythonTypeConversions.h"
#include "BPMPVRecords.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

class EPICSBPMInterface;
typedef boost::shared_ptr<EPICSBPMInterface> EPICSBPMInterface_sptr;
/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control BPMs on VELA/CLARA.
	 A collection of these classes will be stored in map in the BPMFactory class.
 */
class BPM : public Hardware
{
public:
	/*! Default constructor call for BPM object*/
	BPM();
	/*! Custom constructor for BPM object
		@param[in] bpmParametersAndValuesMap: strings defining parameters extracted from YAML config files
		@param[in] mode: Defines the STATE of BPM we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)
	*/
	BPM(const std::map<std::string, std::string>& bpmParametersAndValuesMap, STATE mode);
	/*! Copy constructor call for BPM object*/
	BPM(const BPM& copyBPM);
	/*! EPICSBPMInterface for BPM-specifc calls to EPICS*/
	EPICSBPMInterface_sptr epicsInterface;
	/*! Attach to the initial CA Context for use in Multi-threaded applications*/
	void attachToInitialContext();
	/*! Returns the name of the BPM.
	@param[out] name: the name of the BPM.
	*/
	std::string getBPMName() const;
	/*! A function to retrieve the aliases for a given BPM object
		@param[out] vector of aliases that the BPM is also known by*/
	std::vector<std::string> getAliases() const;
	/*! Gets the type of BPM.
		@param[out] type: the BPM type. (Should probably be converted to TYPE rather than string)*/
	std::string getBPMType() const;
	/*! A map for storing the parameters extracted from YAML config files and their values */
	std::map<std::string, std::string> bpmParametersAndValuesMap;
	LoggingSystem messenger;
	/*! Goes through all PV records stored in BPMRecords namespace, creates pvStructs and sets up their record/fullPVName before
	inserting them into a vector of pvStructs which is later used by the BPMFactory to setup connections to EPICS.
	*/
	void setPVStructs(STATE mode);
	/*! turns debug messaging on for this BPM instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this BPM instance*/
	void debugMessagesOff();
	/*! turns messaging on for this BPM instance*/
	void messagesOn();
	/*! turns messaging off for this BPM instance*/
	void messagesOff();
	void offlineSet(const long& value);
	/*! is the BPM monitoring? */
	bool monitoringData = false;
	/*! is the BPM monitoring any PV? (see monitorForNShots)
	@param[out] bool: true if yes.*/
	bool ismonitoring() const;
	/*! is the BPM monitoring the raw data PV? (see monitorForNShots)
	@param[out] bool: true if yes.*/
	bool ismonitoringData() const;
	/*! is the BPM monitoring the X PV? (see monitorForNShots)
	@param[out] bool: true if yes.*/
	bool ismonitoringXPV() const;
	/*! is the BPM monitoring the Y PV? (see monitorForNShots)
	@param[out] bool: true if yes.*/
	bool ismonitoringYPV() const;
	/*! is the X buffer full?
	@param[out] bool: true if yes.*/
	bool isXPVBufferFull() const;
	/*! is the Y PV buffer full?
	@param[out] bool: true if yes.*/
	bool isYPVBufferFull() const;
	/*! is the raw data buffer full?
	@param[out] bool: true if yes.*/
	bool isDataBufferFull() const;
	/*! is the X PV buffer not full?
	@param[out] bool: true if yes.*/
	bool isXPVBufferNotFull() const;
	/*! is the Y PV buffer full?
	@param[out] bool: true if yes.*/
	bool isYPVBufferNotFull() const;
	/*! is the raw data buffer not full?
	@param[out] bool: true if yes.*/
	bool isDataBufferNotFull() const;
	/*! calculates the X value from raw data
	@param[in] u11: first value from horizontal raw data.
	@param[in] u12: second value from horizontal raw data.
	@param[in] u13: third value from horizontal raw data.
	@param[in] u14: fourth value from horizontal raw data.
	@param[in] mn: BPM aperture, defined in config file.
	@param[in] xn: BPM horizontal offset, defined in config file.
	@param[out] x: the calculated X value.*/
	double calcX(double u11, double u12, double u13, double u14, double mn, double xn) const;
	/*! calculates the Y value from raw data
	@param[in] u21: first value from vertical raw data.
	@param[in] u22: second value from vertical raw data.
	@param[in] u23: third value from vertical raw data.
	@param[in] u24: fourth value from vertical raw data.
	@param[in] mn: BPM aperture, defined in config file.
	@param[in] yn: BPM vertical offset, defined in config file.
	@param[out] y: the calculated Y value.*/
	double calcY(double u21, double u22, double u23, double u24, double mn, double yn) const;
	/*! gets the X value from raw data
	@param[out] x: the calculated X value.*/
	double getX() const;
	/*! gets the Y value from raw data
	@param[out] y: the calculated X value.*/
	double getY() const;
	/*! gets the charge value from raw data
	@param[out] q: the calculated charge value.*/
	double getQ() const;
	/*! gets the beamline position of the BPM
	@param[out] pos: the position.*/
	double getPosition() const;
	/*! gets the resolution of the BPM
	@param[out] res: the calculated resolution.*/
	double getResolution() const;
	/*! gets the X PV value
	@param[out] x: the X PV value.*/
	double getXFromPV() const;
	/*! gets the Y PV value
	@param[out] y: the Y PV value.*/
	double getYFromPV() const;
	/*! the status of the BPM based on raw data pickup values.*/
	STATE status;
	/*! a buffer of BPM status values.*/
	boost::circular_buffer< STATE > statusBuffer;
	/*! a vector of BPM status values (after using monitorForNShots).*/
	std::vector< STATE > statusVector;
	/*! returns the status of the BPM based on raw data pickup values.
	@param[out] state: the status.*/
	STATE getStatus() const;
	/*! returns the status of the BPM based on raw data pickup values.
	@param[out] state: the status.*/
	std::string getStatusStr() const;
	/*! returns the status buffer of the BPM based on raw data pickup values.
	@param[out] states: the status buffer.*/
	boost::circular_buffer< STATE > getStatusBuffer() const;
	/*! returns the status vector of the BPM based on raw data pickup values (after using monitorForNShots).
	@param[out] states: the status vector.*/
	std::vector< STATE > getStatusVector() const;
	/*! returns the size of the buffer that is continuously filled.
	@param[out] size: the buffer size.*/
	size_t getBufferSize() const;
	/*! returns the raw data vector.
	@param[out] data: the raw data vector.*/
	std::vector< double > getData() const;
	/*! returns X PV vector (after using monitorForNShots).
	@param[out] xvec: the X PV vector.*/
	std::vector< double > getXPVVector() const;
	/*! returns Y PV vector (after using monitorForNShots).
	@param[out] yvec: the Y PV vector.*/
	std::vector< double > getYPVVector() const;
	/*! returns the raw data vector (after using monitorForNShots).
	@param[out] datavec: the raw data vector.*/
	std::vector< std::vector< double > > getDataVector() const;
	/*! returns the charge vector (after using monitorForNShots).
	@param[out] datavec: the charge vector.*/
	std::vector< double > getQVector() const;
	/*! returns the raw data buffer.
	@param[out] buf: the raw data buffer.*/
	boost::circular_buffer< std::vector< double > > getDataBuffer() const;
	/*! returns the X PV buffer.
	@param[out] xbuf: the X PV buffer.*/
	boost::circular_buffer< double > getXPVBuffer() const;
	/*! returns the Y PV buffer.
	@param[out] ybuf: the X PV buffer.*/
	boost::circular_buffer< double > getYPVBuffer() const;
	/*! returns the X buffer (calculated from raw data).
	@param[out] xbuf: the X buffer.*/
	boost::circular_buffer< double > getXBuffer() const;
	/*! returns the Y buffer (calculated from raw data).
	@param[out] ybuf: the Y buffer.*/
	boost::circular_buffer< double > getYBuffer() const;
	/*! returns the charge buffer.
	@param[out] qbuf: the charge buffer.*/
	boost::circular_buffer< double > getQBuffer() const;
	/*! returns the raw data vector as a python list.
	@param[out] data: the raw data vector.*/
	boost::python::list getData_Py() const;
	/*! returns X PV vector (after using monitorForNShots) as a python list.
	@param[out] xvec: the X PV vector.*/
	boost::python::list getXPVVector_Py() const;
	/*! returns Y PV vector (after using monitorForNShots) as a python list.
	@param[out] yvec: the Y PV vector.*/
	boost::python::list getYPVVector_Py() const;
	/*! returns calculacted X vector (after using monitorForNShots) as a python list.
	@param[out] xvec: the X vector.*/
	boost::python::list getXVector_Py() const;
	/*! returns calculated Y vector (after using monitorForNShots) as a python list.
	@param[out] yvec: the Y vector.*/
	boost::python::list getYVector_Py() const;
	/*! returns the raw data vector (after using monitorForNShots) as a python list.
	@param[out] datavec: the raw data vector.*/
	boost::python::list getDataVector_Py() const;
	/*! returns the charge vector (after using monitorForNShots) as a python list.
	@param[out] datavec: the charge vector.*/
	boost::python::list getQVector_Py() const;
	/*! returns the raw data buffer as a python list.
	@param[out] buf: the raw data buffer.*/
	boost::python::list getDataBuffer_Py() const;
	/*! returns the X PV buffer as a python list.
	@param[out] xbuf: the X PV buffer.*/
	boost::python::list getXPVBuffer_Py() const;
	/*! returns the Y PV buffer as a python list.
	@param[out] ybuf: the X PV buffer.*/
	boost::python::list getYPVBuffer_Py() const;
	/*! returns the X buffer (calculated from raw data) as a python list.
	@param[out] xbuf: the X buffer.*/
	boost::python::list getXBuffer_Py() const;
	/*! returns the Y buffer (calculated from raw data) as a python list.
	@param[out] ybuf: the Y buffer.*/
	boost::python::list getYBuffer_Py() const;
	/*! returns the charge buffer as a python list.
	@param[out] qbuf: the charge buffer.*/
	boost::python::list getQBuffer_Py() const;
	/*! returns the status buffer of the BPM (as a python list) based on raw data pickup values.
	@param[out] states: the status buffer.*/
	boost::python::list getStatusBuffer_Py() const;
	/*! returns the status vector of the BPM (as a python list) based on raw data pickup values (after using monitorForNShots).
	@param[out] states: the status vector.*/
	boost::python::list getStatusVector_Py() const;
	/*! returns the horizontal read attenuation.
	@param[out] long: RA1.*/
	long getRA1() const;
	/*! returns the vertical read attenuation.
	@param[out] long: RA2.*/
	long getRA2() const;
	/*! returns the horizontal read delay.
	@param[out] long: RD1.*/
	long getRD1() const;
	/*! returns the vertical read delay.
	@param[out] long: RD2.*/
	long getRD2() const;
	/*! returns the horizontal set attenuation.
	@param[out] long: SA1.*/
	long getSA1() const;
	/*! returns the vertical set attenuation.
	@param[out] long: SA2.*/
	long getSA2() const;
	/*! returns the horizontal set delay.
	@param[out] long: SD1.*/
	long getSD1() const;
	/*! returns the vertical set delay.
	@param[out] long: SD2.*/
	long getSD2() const;
	/*! sets the horizontal attenuation.
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setSA1(const long& value);
	/*! sets the vertical attenuation.
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setSA2(const long& value);
	/*! sets the horizontal delay.
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setSD1(const long& value);
	/*! sets the vertical delay.
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setSD2(const long& value);
	/*! sets the horizontal attenuation (Hardware object only, no EPICS).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setRA1(const long& value);
	/*! sets the vertical attenuation (Hardware object only, no EPICS).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setRA2(const long& value);
	/*! sets the horizontal delay (Hardware object only, no EPICS).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setRD1(const long& value);
	/*! sets the vertical delay (Hardware object only, no EPICS).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setRD2(const long& value);
	/*! sets the awake parameter (Hardware object only, no EPICS).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setAWAK(const double& value);
	/*! sets the ready parameter (Hardware object only, no EPICS).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setRDY(const double& value);
	/*! sets the Y PV (virtual machine only).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setYPVVirtual(const double& value);
	/*! sets the X PV (virtual machine only).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setXPVVirtual(const double& value);
	/*! sets the X PV (Hardware object only).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setXPV(const double& value);
	/*! sets the Y PV (Hardware object only).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setYPV(const double& value);
	/*! sets the raw data vector (Hardware object only).
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setData(const std::vector< double >& value);
	/*! sets the resolution.
	@param[in] value: desired value.
	@param[out] bool: true if it worked.*/
	bool setResolution();
	/*! checks if the two most recent values in the given buffer are the same. Used to set BPM status.
	@param[in] buf: buffer to check.
	@param[out] bool: true if the values are the same.*/
	bool checkBuffer(boost::circular_buffer< double >& buf);
	/*! sets the charge.
	@param[in] rawdata: raw data vector
	@param[out] bool: true if it worked.*/
	bool setQ(const std::vector< double >& rawData);
	/*! recalibrate attenuation based on bunch charge.
	@param[in] charge: bunch charge (from WCM or elsewhere0.
	@param[out] bool: true if it worked.*/
	bool reCalAttenuation(const double& charge);
	/*! fill up data vectors.
	@param[in] value: number of shots to monitor.*/
	void monitorForNShots(const size_t& value);
	/*! sets the size of all buffers.
	@param[in] value: buffer size.*/
	void setBufferSize(const size_t& value);
	/*! sets the size of all vectors.
	@param[in] value: vector size.*/
	void setVectorSize(const size_t& value);
	/*! remove all data from buffers.*/
	void clearBuffers();
	/*! checks the BPM status based on raw data and X/Y PVs.*/
	void checkStatus();
	/*! calculated X value w/ associated epicsTimeStamp.*/
	std::pair<epicsTimeStamp, double> x;
	/*! X PV value w/ associated epicsTimeStamp.*/
	std::pair<epicsTimeStamp, double> xPV;
	/*! calculated Y value w/ associated epicsTimeStamp.*/
	std::pair<epicsTimeStamp, double> y;
	/*! Y pv value w/ associated epicsTimeStamp.*/
	std::pair<epicsTimeStamp, double> yPV;
	/*! charge value w/ associated epicsTimeStamp.*/
	std::pair<epicsTimeStamp, double> q;
	/*! resolution value w/ associated epicsTimeStamp.*/
	std::pair<epicsTimeStamp, double> resolution;
	/*! raw data vector w/ associated epicsTimeStamp.*/
	std::pair<epicsTimeStamp, std::vector< double > > data;
	/*! pickup 1 value.*/
	double pu1;
	/*! pickup 2 value.*/
	double pu2;
	/*! pickup 3 value.*/
	double pu3;
	/*! pickup 4 value.*/
	double pu4;
	/*! pedestal 1 value.*/
	double p1;
	/*! pedestal 2 value.*/
	double p2;
	/*! c1 value.*/
	double c1;
	/*! c2 value.*/
	double c2;
	/*! number of shots for X PV.*/
	size_t xpvshots = 0;
	/*! number of shots for Y PV.*/
	size_t ypvshots = 0;
	/*! number of shots for raw data.*/
	size_t datashots = 0;
	/*! number of shots for charge.*/
	size_t qshots = 0;
	/*! size of all buffers.*/
	size_t bufferSize;
	/*! size of all vectors.*/
	size_t vectorSize;
	/*! calculated X buffer.*/
	boost::circular_buffer< double > xBuffer;
	/*! X PV buffer.*/
	boost::circular_buffer< double > xPVBuffer;
	/*! calculated Y buffer.*/
	boost::circular_buffer< double > yBuffer;
	/*! Y PV buffer.*/
	boost::circular_buffer< double > yPVBuffer;
	/*! charge buffer.*/
	boost::circular_buffer< double > qBuffer;
	/*! raw data buffer.*/
	boost::circular_buffer< std::vector< double > > dataBuffer;
	/*! pickup 1 buffer.*/
	boost::circular_buffer< double > pu1Buffer;
	/*! pickup 2 buffer.*/
	boost::circular_buffer< double > pu2Buffer;
	/*! pickup 3 buffer.*/
	boost::circular_buffer< double > pu3Buffer;
	/*! pickup 4 buffer.*/
	boost::circular_buffer< double > pu4Buffer;
	/*! c1 buffer.*/
	boost::circular_buffer< double > c1Buffer;
	/*! c2 buffer.*/
	boost::circular_buffer< double > c2Buffer;
	/*! pedestal 1 buffer.*/
	boost::circular_buffer< double > p1Buffer;
	/*! pedestal 2 buffer.*/
	boost::circular_buffer< double > p2Buffer;
	/*! calculated X vector.*/
	std::vector< double > xVector;
	/*! X PV vector.*/
	std::vector< double > xPVVector;
	/*! calculated Y vector.*/
	std::vector< double > yVector;
	/*! Y PV vector.*/
	std::vector< double > yPVVector;
	/*! charge vector.*/
	std::vector< double > qVector;
	/*! raw data vector of vectors.*/
	std::vector< std::vector< double > > dataVector;
	/*! horizontal set atteunation w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > sa1;
	/*! vertical set atteunation w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > sa2;
	/*! horizontal set delay w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > sd1;
	/*! vertical set de;ay w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > sd2;
	/*! horizontal read atteunation w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > ra1;
	/*! vertical read atteunation w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > ra2;
	/*! horizontal read delay w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > rd1;
	/*! vertical read delay w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, long > rd2;
	/*! horizontal attenuation calibration factor (from config file).*/
	long att1cal = 0;
	/*! vertical attenuation calibration factor (from config file).*/
	long att2cal = 0;
	/*! horizontal voltage calibration factor (from config file).*/
	double v1cal = 0.0;
	/*! horizontal voltage calibration factor (from config file).*/
	double v2cal = 0.0;
	/*! charge calibration factor (from config file).*/
	double qcal = 0.0;
	/*! BPM aperture (from config file).*/
	double mn = 0;
	/*! beamline position (from config file).*/
	double position;
	/*! awake parameter w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, double > awak;
	/*! ready parameter w/ associated epicsTimeStamp.*/
	std::pair< epicsTimeStamp, double > rdy;
	/*! true when X PV is being monitored.*/
	bool monitoringxpv;
	/*! true when Y PV is being monitored.*/
	bool monitoringypv;
	/*! true when raw data is being monitored.*/
	bool monitoringdata;
	/*! BPM PV name.*/
	std::string name;
	friend class EPICSBPMInterface;
protected:
	//what else does a bpm need?
	std::vector<std::string> aliases;
	std::string bpmType;
};
/**\copydoc Hardware*/
/**@}*/
#endif //BPM_H_
