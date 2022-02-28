#ifndef IMG_H_
#define IMG_H_

#include <LoggingSystem.h>
#ifndef HARDWARE_H_
#include <Hardware.h>
#endif //HARDWARE_H_
#ifndef EPICS_IMG_INTERFACE_H_
#include <EPICSIMGInterface.h>
#endif //EPICS_IMG_INTERFACE_H_
#include <boost/shared_ptr.hpp>
#include <vector>
#include <IMGPVRecords.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <GlobalTypeEnums.h>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>


class EPICSIMGInterface;

typedef boost::shared_ptr<EPICSIMGInterface> EPICSIMGInterface_sptr;

/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control IMG on VELA/CLARA.
	 A collection of these classes will be stored in map in the IMGFactory class.
 */

class IMG : public Hardware
{
public:
	/*! Default constructor call for IMG object*/
	IMG();
	
	/*! Custom constructor for IMG object
		@param[in] IMGParameterMap strings defining parameters extracted from YAML config files
		@param[in] mode Defines the STATE of IMG we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)
	*/
	IMG(const std::map<std::string, std::string>& paramMap, STATE mode);
	IMG(const IMG& copyIMG);
	~IMG();
	std::pair<epicsTimeStamp, double> pressure;
	std::pair<epicsTimeStamp, STATE> state;
	TYPE imgtype;
	std::string imgType;
	void setPVStructs();
	/*! EPICSIMGInterface for IMG-specifc calls to EPICS, includes setting OK/ERR state and monitoring gauge pressures*/
	EPICSIMGInterface_sptr epicsInterface;
	/*! Attach to the initial CA Context for use in Multi-threaded applications*/
	void attachToInitialContext();
	/*! A map for storing the parameters extracted from YAML config files and their values */
	std::map<std::string, std::string> IMGParamMap;
	
	/*! alternative names for the IMG (usually shorter than the full PV root), defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;
	std::map<std::string, std::string> aliasToNameMap;
	
	/*! A function to retrieve the aliases for a given IMG object
	@param [out] vector of aliases that the IMG is also known by*/
	std::vector<std::string> getAliases() const;
	
	/*! gets the IMG state.
	If we have a Physical IMG then the request is sent to CLARA EPICS to retrieve the IMG state.
	If we have a Virtual IMG then the request is sent to Virtual EPICS to retrieve the IMG state.
	If we have an Offline IMG then the valve state is simply set to OK. 
	@param [out] state of the IMG object (OK, ERR) */
	STATE getIMGState() const;
	
	/*! sets the IMGState variable for this IMG object 
	@param [in] state in which the IMG object is to be set*/
	void setIMGState(const STATE& states);
	
	/*! retrieves the IMG Pressure variable for this IMG object
	@param [out] IMGPressure the current Pressure of the IMG object */
	double getIMGPressure() const;
	
	/*! turns debug messaging on for this IMG instance*/
	void debugMessagesOn();
	
	/*! turns debug messaging off for this IMG instance*/
	void debugMessagesOff();
	
	/*! turns messaging on for this IMG instance*/
	void messagesOn();
	
	/*! turns messaging off for this Valve instance*/
	void messagesOff();
	friend class EPICSIMGInterface;


};
/**\copydoc Hardware*/
/**@}*/


#endif //IMG_H_