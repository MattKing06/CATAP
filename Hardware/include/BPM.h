#ifndef BPM_H_
#define BPM_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#ifndef EPICS_BPM_INTERFACE_H_
#include "EPICSBPMInterface.h"
#endif //EPICS_BPM_INTERFACE_H_
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
// forward declaration of EPICSBPMInterface class
// tells compiler that we will use this class.
class EPICSBPMInterface;
typedef boost::shared_ptr<EPICSBPMInterface> EPICSBPMInterface_sptr;
class BPM : public Hardware
{
	public:
		BPM();
		//should need a BPM name (full PV root, or alias can be given)
		//BPM(Hardware hardware); // this should be possible, but isn't useful right now.
		BPM(std::string knownNameOfBPM);
		BPM(const std::map<std::string, std::string>& bpmParametersAndValuesMap, bool isVirtual);
		BPM(const BPM& copyBPM);
		EPICSBPMInterface_sptr epicsInterface;
		std::string getFullPVName() const;
		std::vector<std::string> getAliases() const;
		std::string getBPMType() const;
		std::map<std::string, std::string> bpmParametersAndValuesMap;
		bool isVirtual;
		bool monitoringData = false;
		bool isMonitoringBPMData() const;
		bool isNotMonitoringBPMData() const;
		bool isDataBufferFull() const;
		bool isDataBufferNotFull() const;
		bool isXBufferFull() const;
		bool isYBufferFull() const;
		bool isXBufferNotFull() const;
		bool isYBufferNotFull() const;
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
		//beamPositionMonitorStructs::BPM_STATUS getBPMStatus() const;
		//beamPositionMonitorStructs::bpmDataObject& getBPMDataObject() const;
		size_t getBufferSize() const;
		std::vector< std::vector< double > > getBPMRawData() const;
		std::vector< double > getBPMXVec() const;
		std::vector< double > getBPMYVec() const;
		std::vector< double > getBPMQVec() const;
		std::vector< double > getTimeStamps() const;
		std::vector< std::string > getStrTimeStamps() const;
		boost::circular_buffer< std::vector< double > > getBPMRawDataBuffer() const;
		boost::circular_buffer< double > getBPMXPVBuffer() const;
		boost::circular_buffer< double > getBPMYPVBuffer() const;
		boost::circular_buffer< double > getBPMXBuffer() const;
		boost::circular_buffer< double > getBPMYBuffer() const;
		boost::circular_buffer< double > getBPMQBuffer() const;
		boost::circular_buffer< double > getTimeStampsBuffer() const;
		boost::circular_buffer< std::string > getStrTimeStampsBuffer() const;
		//boost::circular_buffer< beamPositionMonitorStructs::BPM_STATUS > getStatusBuffer() const;
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
		bool setXPV(const double& value);
		bool setYPV(const double& value);
		bool setXPVBuffer(const double& value);
		bool setYPVBuffer(const double& value);
		void setBufferSize(const size_t& value);
		void clearBuffers();
		std::string getBPMStatusStr() const;
		STATE getBPMState() const;
		double x;
		double xPV;
		double y;
		double yPV;
		double q;
		double resolution;
		double position;
		size_t bufferSize;
		boost::circular_buffer< double > xBuffer;
		boost::circular_buffer< double > xPVBuffer;
		boost::circular_buffer< double > yBuffer;
		boost::circular_buffer< double > yPVBuffer;
		boost::circular_buffer< double > qBuffer;
		long sa1;
		long sa2;
		long sd1;
		long sd2;
		long ra1;
		long ra2;
		long rd1;
		long rd2;
		STATE bpmState;
	protected:
		//what else does a bpm need?
		std::vector<std::string> aliases;
		std::string bpmType;
};

#endif //BPM_H_