#ifndef IMG_H_
#define IMG_H_

#include <Hardware.h>
#include <EPICSIMGInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSIMGInterface;

typedef boost::shared_ptr<EPICSIMGInterface> EPICSIMGInterface_sptr;

/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control IMG on VELA/CLARA.
	 A collection of these classes will be stored in map in the IMGFactory class.
 */

class IMG : Hardware
{
public:
	IMG();
	IMG(const std::map<std::string, std::string>& paramMap, STATE mode);
	IMG(const IMG& copyIMG);
	~IMG();
	std::pair<epicsTimeStamp, double> pressure;
	std::pair<epicsTimeStamp, STATE> state;
	void setPVStructs();
	
	EPICSIMGInterface_sptr epicsInterface;
	std::map<std::string, std::string> IMGParamMap;
	std::vector<std::string> aliases;
	std::map<std::string, std::string> aliasToNameMap;
	std::vector<std::string> getAliases() const;
	/*! sets the valve state to open.
	If we have a Physical valve then the request is sent to CLARA EPICS to open the valve.
	If we have a Virtual valve then the request is sent to Virtual EPICS to open the valve.
	If we have an Offline valve then the valve state is simply set to OPEN. */
	
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	friend class EPICSIMGInterface;


};
/**\copydoc Hardware*/
/**@}*/


#endif //IMG_H_