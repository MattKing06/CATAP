#ifndef LIGHTING_H_
#define LIGHTING_H_

#include <Hardware.h>
#include <EPICSLightingInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSLightingInterface;

typedef boost::shared_ptr<EPICSLightingInterface> EPICSLightingInterface_sptr;

class Lighting : public Hardware
{
public:
	Lighting();
	Lighting(const std::map<std::string, std::string>& paramMap, STATE mode);
	Lighting(const Lighting& copyLighting);
	~Lighting();
	void setPVStructs();
	EPICSLightingInterface_sptr epicsInterface;
	std::map<std::string, std::string> LightingParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //LIGHTING_H_