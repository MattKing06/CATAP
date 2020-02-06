#ifndef VALVE_H_
#define VALVE_H_
#include <Hardware.h>
#include <EPICSValveInterface.h>
#include <boost/shared_ptr.hpp>
#include <GlobalConstants.h>
#include <ValvePVRecords.h>
#include <vector>

class EPICSValveInterface;

typedef boost::shared_ptr<EPICSValveInterface> EPICSValveInterface_sptr;


class Valve : public Hardware
{
public:
	Valve();
	Valve(const std::map<std::string, std::string>& valveParameterMap, STATE mode);
	EPICSValveInterface_sptr epicsInterface;
	std::map<std::string, std::string> valveParameterAndValuesMap;
	
	void setPVStructs();
	std::vector<std::string> getAliases() const;
	void open();
	void close();
	void setValveState(const STATE& state);
	STATE getValveState() const;
	bool isOpen() const;
	bool isClosed() const;
	void offlineSetValveState(STATE state);
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	friend class EPICSValveInterface;
protected:
	std::vector<std::string> aliases;
	std::pair<epicsTimeStamp, STATE> valveState;

};


#endif //VALVE_H_