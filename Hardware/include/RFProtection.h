#ifndef RF_PROTECTION_H_
#define RF_PROTECTION_H_

#include <Hardware.h>
#include <EPICSRFProtectionInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>
#ifdef BUILD_PYTHON
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <PythonTypeConversions.h>
#endif //BUILD_PYTHON

class EPICSRFProtectionInterface;

typedef boost::shared_ptr<EPICSRFProtectionInterface> EPICSRFProtectionInterface_sptr;

class RFProtection : public Hardware
{
public:
	RFProtection();
	RFProtection(const std::map<std::string, std::string>& paramMap, STATE mode);
	RFProtection(const RFProtection& copyRFProtection);
	~RFProtection();

	/*! get the name alises for this RFProtection
	@param[out] names, vector contianing all the alias names */
	std::vector<std::string> getAliases() const;
#ifdef BUILD_PYTHON
	/*! get the name alises for this RFProtection (python version)
		@param[out] names, python list contianing all the alias names */
	boost::python::list getAliases_Py() const;
#endif //BUILD_PYTHON
	TYPE getProtectionType() const;
	std::string getProtectionTypeAsStr() const;
	EPICSRFProtectionInterface_sptr epicsInterface;
	

	long getCmi()const;
	STATE getStatus()const;
	std::vector<int> getKeyBits()const;
	std::vector<bool>  getKeyBitValues()const;

		
	bool isGood() const;
	bool isNotGood() const;
	bool isBad() const;

	bool reset() const;
	bool enable() const;
	bool disable() const;

	

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();

	
	friend class EPICSRFProtectionInterface;
	friend class RFProtectionFactory;
protected:
	// called from EPICS to update the GETSETI variable! 
	/*! switch the magnet PSU on	*/
	//	void updateGETSETI(const double& value);
	/*! latest readi value and epicstimestamp 	*/
	std::pair<epicsTimeStamp, long > cmi;
	

	std::vector<int>  protKeyBits;
	std::vector<bool> protKeyBitValues;
	
	std::pair<epicsTimeStamp, STATE > status;




private:
	TYPE protectionType;

	void setMasterLatticeData();

	void setPVStructs();

	// TODO put in hardware Object for each type 
	/*! alternative names for this object */
	std::vector<std::string> aliases;

	std::map<std::string, std::string> RFProtectionParamMap;


	bool sendCmmand();


};


#endif //RF_PROTECTION_H_