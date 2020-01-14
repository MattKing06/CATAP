#ifndef CHARGE_H_
#define CHARGE_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#ifndef EPICS_CHARGE_INTERFACE_H_
#include "EPICSChargeInterface.h"
#endif //EPICS_CHARGE_INTERFACE_H_
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
// forward declaration of EPICSBPMInterface class
// tells compiler that we will use this class.
class EPICSChargeInterface;
typedef boost::shared_ptr<EPICSChargeInterface> EPICSChargeInterface_sptr;
class Charge : public Hardware
{
	public:
		Charge();
		Charge(std::string knownNameOfCharge);
		Charge(const std::map<std::string, std::string>& chargeParametersAndValuesMap, STATE mode);
		Charge(const Charge& copyCharge);
		EPICSChargeInterface_sptr epicsInterface;
		std::string getName() const;
		std::vector<std::string> getAliases() const;
		std::string getChargeDiagnosticType() const;
		std::map<std::string, std::string> chargeParametersAndValuesMap;
		
		// thsi exists in teh hardware base class
		//STATE mode;
		
		bool monitoringData = false;
		bool isMonitoring() const;
		bool isMonitoringQ() const;
		bool isQBufferFull() const;
		double getQ() const;
		double getPosition() const;
		//beamPositionMonitorStructs::BPM_STATUS getBPMStatus() const;
		//beamPositionMonitorStructs::bpmDataObject& getBPMDataObject() const;
		size_t getBufferSize() const;
		std::vector< double > getQVector() const;
		std::vector< double > getTimeStamps() const;
		std::vector< std::string > getStrTimeStamps() const;
		boost::circular_buffer< double > getQBuffer() const;
		boost::circular_buffer< double > getTimeStampsBuffer() const;
		boost::circular_buffer< std::string > getStrTimeStampsBuffer() const;
		//boost::circular_buffer< beamPositionMonitorStructs::BPM_STATUS > getStatusBuffer() const;
		bool setQ(const double& value);
		bool checkBuffer(boost::circular_buffer< double >& buf);
		void monitorForNShots(const size_t& value);
		void setBufferSize(const size_t& value);
		void setVectorSize(const size_t& value);
		void clearBuffers();
		double q;
		double position;
		unsigned int qshots;
		size_t bufferSize;
		size_t vectorSize;
		boost::circular_buffer< double > qBuffer;
		std::vector< double > qVector;
		bool monitoringq;
		STATE chargeState;
		std::string name;
	protected:
		std::vector<std::string> aliases;
		std::string chargeType;
};

#endif //CHARGE_H_