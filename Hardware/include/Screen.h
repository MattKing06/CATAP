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
	std::vector< std::string > getScreenNames() const;
	std::vector< screenStructs::STATE > getAvailableDevices() const;
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
	void moveScreenTo(const std::string& name, const screenStructs::SCREEN_STATE& state);
	void moveScreensTo(const std::vector< const std::string > names, const std::vector< screenStructs::SCREEN_STATE > states);
	void insertYAG(const std::string& name);
	void makeReadEqualSet(const std::string& name);
	void makeReadEqualSetAll();
	void makeSetEqualRead(const std::string& name);
	void makeSetEqualReadAll();
	void moveScreenOut(const std::string& name);
	void resetPosition(const std::string& name);
	void jogScreen(const std::string& name, const double jog);
	void setPosition(const std::string& name, const double setPos);
	bool setScreenSDEV(const std::string& name, const screenStructs::SCREEN_STATE& state);
	bool setScreenTrigger(const std::string& name);
	bool setScreenTrigger(const std::string& name, const screenStructs::SCREEN_STATE& state);
	bool setEX(const std::string& name);
	bool setEN(const std::string& name, const screenStructs::DRIVER_DIRECTION direction);
	STATE bpmState;
	std::string name;
	friend class EPICSBPMInterface;
protected:
	//what else does a bpm need?
	std::vector<std::string> aliases;
	std::string bpmType;
};

#endif //BPM_H_