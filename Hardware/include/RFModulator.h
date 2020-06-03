#ifndef RF_MODULATOR_H_
#define RF_MODULATOR_H_

#include <Hardware.h>
#include <EPICSRFModulatorInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSRFModulatorInterface;

typedef boost::shared_ptr<EPICSRFModulatorInterface> EPICSRFModulatorInterface_sptr;

class RFModulator : public Hardware
{
public:
	RFModulator();
	RFModulator(const std::map<std::string, std::string>& paramMap, STATE mode);
	RFModulator(const RFModulator& copyRFModulator);
	~RFModulator();
	void setPVStructs();
	EPICSRFModulatorInterface_sptr epicsInterface;
	std::map<std::string, std::string> RFModulatorParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //RF_MODULATOR_H_