#ifndef LASER_MIRROR_H_
#define LASER_MIRROR_H_

#include <Hardware.h>
#include <EPICSLaserMirrorInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSLaserMirrorInterface;

typedef boost::shared_ptr<EPICSLaserMirrorInterface> EPICSLaserMirrorInterface_sptr;

class LaserMirror : public Hardware
{
public:
	LaserMirror();
	LaserMirror(const std::map<std::string, std::string>& paramMap, STATE mode);
	LaserMirror(const LaserMirror& copyLaserMirror);
	~LaserMirror();
	void setPVStructs();
	EPICSLaserMirrorInterface_sptr epicsInterface;
	std::map<std::string, std::string> LaserMirrorParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //LASER_MIRROR_H_