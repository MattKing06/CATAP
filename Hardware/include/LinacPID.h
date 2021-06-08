#ifndef LINAC_PID_H_
#define LINAC_PID_H_
#include <Hardware.h>
#include <EPICSLinacPIDInterface.h>
#include <boost/shared_ptr.hpp>
#include <GlobalConstants.h>
#include <LinacPIDPVRecords.h>
#include <vector>

class EPICSLinacPIDInterface;
typedef boost::shared_ptr<EPICSLinacPIDInterface> EPICSLinacPIDInterface_sptr;

/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control the Linac PID loop on VELA/CLARA.
	 A collection of these classes will be stored in map in the LinacPIDFactory class.
 */

class LinacPID : public Hardware
{
public:
	LinacPID();
	LinacPID(const std::map<std::string, std::string>& linacPIDParameterMap, STATE mode);
	//LinacPID(const LinacPID& copyLinacPID);
	//~LinacPID();
	EPICSLinacPIDInterface_sptr epicsInterface;
	std::map<std::string, std::string> linacPIDParameterAndValuesMap;
	void setPVStructs();
	std::vector<std::string> getAliases() const;
	/*! turns debug messaging on for this Valve instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this Valve instance*/
	void debugMessagesOff();
	/*! turns messaging on for this Valve instance*/
	void messagesOn();
	/*! turns messaging off for this Valve instance*/
	void messagesOff();
	friend class EPICSLinacPIDInterface;
protected:
	std::vector<std::string> aliases;
};

/**\copydoc Hardware*/
/**@}*/

#endif // !LINAC_PID_H_