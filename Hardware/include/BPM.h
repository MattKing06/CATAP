#ifndef BPM_H_
#define BPM_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#include "EPICSBPMInterface.h"
#include "BPMPVRecords.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>

class EPICSBPMInterface;
typedef boost::shared_ptr<EPICSBPMInterface> EPICSBPMInterface_sptr;
class BPM : public Hardware
{
public:
	BPM();
	//should need a BPM name (full PV root, or alias can be given)
	//BPM(Hardware hardware); // this should be possible, but isn't useful right now.
	BPM(std::string knownNameOfBPM);
	BPM(const std::map<std::string, std::string>& bpmParametersAndValuesMap, STATE mode);
	BPM(const BPM& copyBPM);
	EPICSBPMInterface_sptr epicsInterface;
	std::string getBPMName() const;
	std::vector<std::string> getAliases() const;
	std::string getBPMType() const;
	std::map<std::string, std::string> bpmParametersAndValuesMap;
	LoggingSystem messenger;
	void setPVStructs();
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	void offlineSet(const long& value);
	bool monitoringData = false;
	bool ismonitoring() const;
	bool ismonitoringData() const;
	bool ismonitoringXPV() const;
	bool ismonitoringYPV() const;
	bool isXBufferFull() const;
	bool isYBufferFull() const;
	bool isXPVBufferFull() const;
	bool isYPVBufferFull() const;
	bool isDataBufferFull() const;
	bool isXBufferNotFull() const;
	bool isYBufferNotFull() const;
	bool isXPVBufferNotFull() const;
	bool isYPVBufferNotFull() const;
	bool isDataBufferNotFull() const;
	double calcX(double u11, double u12, double u13, double u14, double mn, double xn) const;
	double calcY(double u21, double u22, double u23, double u24, double mn, double yn) const;
	double calcQ(std::vector< double > rawData, double att1cal, double att2cal, double v1cal, double v2cal, double qcal) const;
	double getX() const;
	double getY() const;
	double getQ() const;
	double getPosition() const;
	double getResolution() const;
	double getXFromPV() const;
	double getYFromPV() const;
	//beamPositiOnmonitorStructs::BPM_STATUS getBPMStatus() const;
	//beamPositiOnmonitorStructs::bpmDataObject& getBPMDataObject() const;
	size_t getBufferSize() const;
	std::vector< double > getData() const;
	std::vector< double > getXPVVector() const;
	std::vector< double > getYPVVector() const;
	std::vector< std::vector< double > > getDataVector() const;
	std::vector< double > getQVector() const;
	std::vector< double > getTimeStamps() const;
	std::vector< std::string > getStrTimeStamps() const;
	boost::circular_buffer< std::vector< double > > getDataBuffer() const;
	boost::circular_buffer< double > getXPVBuffer() const;
	boost::circular_buffer< double > getYPVBuffer() const;
	boost::circular_buffer< double > getXBuffer() const;
	boost::circular_buffer< double > getYBuffer() const;
	boost::circular_buffer< double > getQBuffer() const;
	boost::circular_buffer< double > getTimeStampsBuffer() const;
	boost::circular_buffer< std::string > getStrTimeStampsBuffer() const;
	//boost::circular_buffer< beamPositiOnmonitorStructs::BPM_STATUS > getStatusBuffer() const;
	long getRA1() const;
	long getRA2() const;
	long getRD1() const;
	long getRD2() const;
	long getSA1() const;
	long getSA2() const;
	long getSD1() const;
	long getSD2() const;
	bool setSA1(const long& value);
	bool setSA2(const long& value);
	bool setSD1(const long& value);
	bool setSD2(const long& value);
	bool setRA1(const long& value);
	bool setRA2(const long& value);
	bool setRD1(const long& value);
	bool setRD2(const long& value);
	bool setAWAK(const double& value);
	bool setAWAKTStamp(const double& value);
	bool setRDY(const double& value);
	bool setRDYTStamp(const double& value);
	bool setXPV(const double& value);
	bool setYPV(const double& value);
	bool setData(const std::vector< double >& value);
	bool setResolution();
	bool checkBuffer(boost::circular_buffer< double >& buf);
	bool setQ(const std::vector< double >& rawData);
	bool reCalAttenuation(const double& charge);
	void monitorForNShots(const size_t& value);
	void setBufferSize(const size_t& value);
	void setVectorSize(const size_t& value);
	void clearBuffers();
	std::string getBPMStatusStr() const;
	STATE getBPMState() const;
	std::pair<epicsTimeStamp, double> x;
	std::pair<epicsTimeStamp, double> xPV;
	std::pair<epicsTimeStamp, double> y;
	std::pair<epicsTimeStamp, double> yPV;
	std::pair<epicsTimeStamp, double> q;
	std::pair<epicsTimeStamp, double> resolution;
	std::pair<epicsTimeStamp, std::vector< double > > data;
	double pu1;
	double pu2;
	double pu3;
	double pu4;
	double p1;
	double p2;
	double c1;
	double c2;
	unsigned int xpvshots;
	unsigned int ypvshots;
	unsigned int datashots;
	unsigned int qshots;
	size_t bufferSize;
	size_t vectorSize;
	boost::circular_buffer< double > xBuffer;
	boost::circular_buffer< double > xPVBuffer;
	boost::circular_buffer< double > yBuffer;
	boost::circular_buffer< double > yPVBuffer;
	boost::circular_buffer< double > qBuffer;
	boost::circular_buffer< std::vector< double > > dataBuffer;
	boost::circular_buffer< double > pu1Buffer;
	boost::circular_buffer< double > pu2Buffer;
	boost::circular_buffer< double > pu3Buffer;
	boost::circular_buffer< double > pu4Buffer;
	boost::circular_buffer< double > c1Buffer;
	boost::circular_buffer< double > c2Buffer;
	boost::circular_buffer< double > p1Buffer;
	boost::circular_buffer< double > p2Buffer;
	std::vector< double > xVector;
	std::vector< double > xPVVector;
	std::vector< double > yVector;
	std::vector< double > yPVVector;
	std::vector< double > qVector;
	std::vector< std::vector< double > > dataVector;
	std::pair< epicsTimeStamp, long > sa1;
	std::pair< epicsTimeStamp, long > sa2;
	std::pair< epicsTimeStamp, long > sd1;
	std::pair< epicsTimeStamp, long > sd2;
	std::pair< epicsTimeStamp, long > ra1;
	std::pair< epicsTimeStamp, long > ra2;
	std::pair< epicsTimeStamp, long > rd1;
	std::pair< epicsTimeStamp, long > rd2;
	long att1cal = 0;
	long att2cal = 0;
	double v1cal = 0.0;
	double v2cal = 0.0;
	double qcal = 0.0;
	double mn = 0;
	double position;
	std::pair< epicsTimeStamp, double > awak;
	std::pair< epicsTimeStamp, double > rdy;
	double awaktstamp;
	double rdytstamp;
	bool monitoringxpv;
	bool monitoringypv;
	bool monitoringdata;
	STATE bpmState;
	std::string name;
	friend class EPICSBPMInterface;
protected:
	//what else does a bpm need?
	std::vector<std::string> aliases;
	std::string bpmType;
};

#endif //BPM_H_