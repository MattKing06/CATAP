#ifndef RF_MODULATOR_H_
#define RF_MODULATOR_H_

#include <Hardware.h>
#include <EPICSRFModulatorInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>
#include <RFModulatorPVRecords.h>
#include <GlobalFunctions.h>
#include "PythonTypeConversions.h"
#include <boost/make_shared.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

class EPICSRFModulatorInterface;

typedef boost::shared_ptr<EPICSRFModulatorInterface> EPICSRFModulatorInterface_sptr;

class RFModulator : public Hardware
{
public:
	RFModulator();
	RFModulator(const std::map<std::string, std::string>& paramMap, STATE mode);
	RFModulator(const RFModulator& copyRFModulator);
	~RFModulator();

	/*! get the name alises for this object
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases() const;
	/*! get the name alises for this LLRF (python version)
		@param[out] list, all the alias names */
	boost::python::list getAliases_Py() const;
    /*! get the name alises for this LLRF (python version)
    @param[out] list, all the alias names */
    STATE getMainState()const;
    /*! is the modulator in RF_ON STATE (For the gun Modulator RF_ON is the same as TRIG on the low level RF control)
        @param[out] bool,  */
    bool isInRFOn() const;
    /*! is the modulator in HV_ON STATE
        @param[out] bool,  */
    bool isInHVOn() const;
    /*! is the modulator in STANDBY STATE
        @param[out] bool,  */
    bool isInStandby() const;
    /*! is the modulator in OFF STATE
        @param[out] bool,  */
    bool isInOff() const;
    /*! get the rf modulator error state
        @param[out] STATE,  */
    STATE getErrorState() const;
    /*! is the modulator error state GOOD
        @param[out] bool,  */
    bool isErrorStateGood() const;
    /*! Get the (read value) of modulator state-set
        @param[out] STATE,  */
    STATE getStateSetRead() const;
    /*! Has the warmup time for the modulator completed (can also change depending on the main state)
    @param[out] bool,  */
    bool isWarm()const;
    /*! Has the warmup time for the modulator NOT completed (can also change depending on the main state)
        @param[out] bool,  */
    bool isNotWarm()const;
    /*! get the low level numerical data for this modulator (voltages, vacuumpressures etc)
    @param[out] map<strign,double>, all low level numerical data in map   */
    std::map<std::string, double> getLowLevelNumericalData()const;
    /*! get the low level numerical data for this modulator (voltages, vacuumpressures etc) Python Version
    @param[out] dict, all low level numerical data in map   */
    boost::python::dict getLowLevelNumericalData_Py()const;
    /*! get the low level string data for this modulator (error messages)
    @param[out] dict, all low level numerical data in map   */
    std::map<std::string, std::string> getLowLevelStringData()const;
    /*! get the low level string data for this modulator (error messages) Python Version
    @param[out] dict, all low level string data */
    boost::python::dict getLowLevelStringData_Py()const;
    /*! get all the low level data for this modulator (string and numerical) Python Version
    @param[out] dict, all low level string and numerical data */
    boost::python::dict getLowLevelData()const;

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();

	friend class EPICSRFModulatorInterface;
	friend class RFModulatorFactory;
protected: 
    /*! update low level string data for this modulator 
    @param[in] string, key for low_level_values for this passed data 
    @param[in] const pair<epicsTimeStamp, string>&, new value from epics */
	void updateLowLevelString(const std::string& key, const std::pair < epicsTimeStamp, std::string>& value);
    /*! update low level numerical data for this modulator
    @param[in] string, key for low_level_values for this passed data
    @param[in] const pair<epicsTimeStamp,double>&, new value from epics */
    void updateLowLevelDouble(const std::string& key, const std::pair < epicsTimeStamp, double>& value);
    /*! update the errro message string 
    @param[in] const string&, time stamp as string for this error message 
    @param[in] const string&, new error message */
    void updateGunErrorString(const std::string& time_stamp_str, const std::string& value);
    /*! get all the low level data for this modulator (string and numerical) Python Version
    @param[in] const std::pair<epicsTimeStamp, long>&, new value from epics  */
    void updateRemainingWarmUpTime(const std::pair < epicsTimeStamp, long>& value);
    /*! warmup time remaining */
	std::pair<epicsTimeStamp, long> wam_up_time_remaining;
    /*! the main state (read) for this modualtor */
	std::pair<epicsTimeStamp, std::string> main_state_string;
    /*! read of the main-state, as a STATE */
    std::pair<epicsTimeStamp, STATE> main_state;
    /*! read of the main-state set, as a STATE */
    std::pair<epicsTimeStamp, STATE> main_state_set_read;
    /*! read of the main-state set, as a string  */
    std::pair<epicsTimeStamp, std::string> main_state_set_read_string;
    /*! buffer of pairs containg <timestamp, error strings> since the object connected to EPICS*/
	std::vector< std::pair<std::string, std::string>> gun_error_string_buffer;
private:
    /*! set up the pvStructs fot this object */
	void setPVStructs();
	/*! Low level read-only numerical values, such as ion-pump and magnet voltages/currents are 
	! stored here. These are necessarily different for different modulators. Values are 
	! set via EPICs in the usual manner and stored in this dictionary. They can only be 
	! got through a single getter function that returns the entire map for the end user 
	! to parse. */
	std::map<std::string, std::pair<epicsTimeStamp, double>> low_level_values;
    /*! Low level read-only string values, such as error messages, are
    ! stored here. These are necessarily different for different modulators. Values are
    ! set via EPICs in the usual manner and stored in this dictionary. They can only be
    ! got through a single getter function that returns the entire map for the end user
    ! to parse. */
	std::map < std::string, std::pair<epicsTimeStamp, std::string >> low_level_strings;
	
	/*! Stores the key names for low_level_values, depends on modualtor TYPE and set 
	during construction  */
	std::vector<std::string> low_level_paramter_names;
    /*! epicsInterface pointer */
	EPICSRFModulatorInterface_sptr epicsInterface;
    /*! RFModulatorParamMap from teh config fuile  */
	std::map<std::string, std::string> RFModulatorParamMap;
    /*! name aliases fot this object */ // TODO should go in base harwdare object 
	std::vector<std::string> aliases;
    /*! update the error state (called from epics interface) */
    void updateErrorState();
    /*! current erro_state */
    STATE error_state;
    /*! Modulator is warm if the Standby countdown has completed  */
    bool is_warm;
    /*! Setup the master lattice data for this modulator  */
	void setMasterLatticeData();

    void setGunModHexMap();

    /*! hex codes for the GUN  modulator state that are considered a GOOD STATE */    // TODO should really be const  
    std::vector<std::string> good_gun_hex_codes =
    {
            "2000", //= "Off**************"},
            "3000", //= "OffReq***********"},
            "5000", //= "StandByReq*******"},
            "6000", //= "StandBy**********"},
            "7000", //= "HvOffReq*********"},
            "9000", //= "HvReq************"},
            "A000", //= "Hv***************"},
            "B000", //= "TrigOffReq*******"},
            "C000", //= "TrigReq**********"},
            "D000" //= "Trig*************"},
    };
    /*! all possible hex codes for the GUN modulator state */    // TODO should really be const  
    std::map<std::string, std::string> gun_mod_hex_map;

};


#endif //RF_MODULATOR_H_