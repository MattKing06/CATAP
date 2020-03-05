#ifndef IMG_H_
#define IMG_H_

#include <Hardware.h>
#include <EPICSIMGInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSIMGInterface;

typedef boost::shared_ptr<EPICSIMGInterface> EPICSIMGInterface_sptr;

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
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //IMG_H_