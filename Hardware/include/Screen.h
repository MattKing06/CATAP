#ifndef SCREEN_H_
#define SCREEN_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#include "EPICSScreenInterface.h"
#include "ScreenPVRecords.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>

class EPICSScreenInterface;
typedef boost::shared_ptr<EPICSScreenInterface> EPICSScreenInterface_sptr;
class Screen : public Hardware
{
public:
	Screen();
	//should need a Screen name (full PV root, or alias can be given)
	//Screen(Hardware hardware); // this should be possible, but isn't useful right now.
	Screen(std::string knownNameOfScreen);
	Screen(const std::map<std::string, std::string>& screenParametersAndValuesMap, STATE mode);
	Screen(const Screen& copyScreen);
	EPICSScreenInterface_sptr epicsInterface;
	std::string getScreenName() const;
	std::vector<std::string> getAliases() const;
	std::string getScreenType() const;
	std::map<std::string, std::string> screenParametersAndValuesMap;
	LoggingSystem messenger;
	void setPVStructs();
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	void offlineSet(const long& value);
	bool isHOut() const;
	bool isVOut() const;
	bool isHIn() const;
	bool isVIn() const;
	bool is_HandV_OUT() const;
	bool isScreenIn() const;
	bool isHMoving() const;
	bool isVMoving() const;
	bool isPMoving() const;
	bool isMoving() const;
	bool isClearForBeam() const;
	bool isHOut() const;
	bool isVOut() const;
	bool isHIn() const;
	bool isVIn() const;
	bool isMover() const;
	bool isVMover() const;
	bool isHVMover() const;
	bool isPneumatic() const;
	STATE getScreenState() const;
	STATE getScreenSetState() const;
	TYPE getScreenType() const;
	std::vector< STATE > getAvailableDevices() const;
	bool isScreenInState(STATE sta) const;
	bool isYAGIn() const;
	bool isHElement(STATE e) const;
	bool isVElement(STATE e) const;
	bool isPElement(STATE e) const;
	bool isHEnabled() const;
	bool isVEnabled() const;
	double getACTPOS() const;
	double getJDiff() const;
	double getDevicePosition(STATE state) const;
	double getPosition() const;
	bool isVELAPneumatic() const;
	bool isVELAHVMover() const;
	bool isCLARAHVMover() const;
	bool isCLARAVMover() const;
	double get_H_ACTPOS() const;
	double get_V_ACTPOS() const;
	/// SETTERS
	void moveScreenTo(STATE& state);
	void insertYAG();
	void makeReadEqualSet();
	void makeSetEqualRead();
	void moveScreenOut();
	void resetPosition();
	void jogScreen(const double jog);
	void setPosition(const double setPos);
	bool setScreenSDEV(STATE& state);
	bool setScreenTrigger();
	bool setScreenTrigger(STATE& state);
	bool setEX();
	bool setEN(STATE direction);
	std::pair< epicsTimeStamp, STATE > state;
	std::pair< epicsTimeStamp, STATE > setstate;
	std::string name;
	friend class EPICSScreenInterface;
protected:
	//what else does a screen need?
	std::vector<std::string> aliases;
	std::string screenType;
	bool has_camera;
};

#endif //SCREEN_H_