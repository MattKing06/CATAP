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
	STATE getIMGState() const;
	void setIMGState(const STATE& states);
	double getIMGPressure() const;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	friend class EPICSIMGInterface;


};
/**\copydoc Hardware*/
/**@}*/


#endif //IMG_H_