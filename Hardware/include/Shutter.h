#ifndef SHUTTER_H_
#define SHUTTER_H_

#include <Hardware.h>

#include "EPICSShutterInterface.h"



#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

class EPICSShutterInterface;

typedef boost::shared_ptr<EPICSShutterInterface> EPICSShutterInterface_sptr;

class Shutter : public Hardware
{
public:
	Shutter();
	/*! Custom constructor for Shutter object
		@param[in] paramMap strings defining parameters extracted from YAML config files
		@param[in] mode Defines the STATE of Shutter we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)			*/
	Shutter(const std::map<std::string, std::string>& paramMap, STATE mode);


	Shutter(const Shutter& copyShutter);
	~Shutter();

		
	std::pair<epicsTimeStamp, STATE > shutterState;
	std::pair<epicsTimeStamp, int > cmi;

	std::vector<int>  cmiBits;
	std::map<std::string, bool>  cmiBitMap;


	//bool closeAndWait(const time_t waitTime);
	//bool openAndWait(const time_t waitTime);
	bool isClosed() const;
	bool isOpen() const;
	STATE getState()const;
	bool close();
	bool open();
	int getCMI()const;
	std::map<std::string, bool>  getCMIBitMap()const;
	boost::python::dict  getCMIBitMap_Py()const;
	/*! get the name alises for this magnet
		@param[out] names, vector contianing all the alias names */
	std::vector<std::string> getAliases() const;
	boost::python::list getAliases_Py() const;

	/*! alternative names for the shutter (usually shorter than the full PV root), defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;

	void setPVStructs();
	EPICSShutterInterface_sptr epicsInterface;
	std::map<std::string, std::string> ShutterParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //SHUTTER_H_