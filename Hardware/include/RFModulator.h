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


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();


private:

	void setPVStructs();
	/*! Low level read-only values , such as ion-pump and magnet voltages/currents are 
	stored here. These are necessarily different for different modulators. value sare 
	set via EPICs in the usual manner and stored in this dictionary. They can only be 
	got through a single getter function that returns the entire map for the end user 
	to parse. */
	std::map < std::string, double > low_level_values;
	
	/*! Stores the key names for low_level_values, depends on modualtor TYPE and set 
	during construction  */
	std::vector<std::string> low_level_paramter_names;

	EPICSRFModulatorInterface_sptr epicsInterface;
	std::map<std::string, std::string> RFModulatorParamMap;

};


#endif //RF_MODULATOR_H_