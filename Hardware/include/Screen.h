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
	std::map<std::string, std::string> screenParametersAndValuesMap;
	LoggingSystem messenger;
	void setPVStructs();
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	void offlineSet(const long& value);
	// SCREEN STATES 
	std::pair< epicsTimeStamp, STATE > screenStateH;
	std::pair< epicsTimeStamp, STATE > screenSetStateH;
	std::pair< epicsTimeStamp, STATE > screenStateV;
	std::pair< epicsTimeStamp, STATE > screenSetStateV;
	std::pair< epicsTimeStamp, STATE > screenState;
	std::pair< epicsTimeStamp, STATE > screenSetState;
	std::pair< epicsTimeStamp, double > actposH;
	std::pair< epicsTimeStamp, double > actposV;
	std::pair< epicsTimeStamp, double > actpos;
	std::pair< epicsTimeStamp, double > tgtposH;
	std::pair< epicsTimeStamp, double > tgtposV;
	std::pair< epicsTimeStamp, double > tgtpos;
	std::pair< epicsTimeStamp, double > devcentH;
	std::pair< epicsTimeStamp, double > devcentV;
	std::pair< epicsTimeStamp, double > devcent;
	std::pair< epicsTimeStamp, double > jdiffH;
	std::pair< epicsTimeStamp, double > jdiffV;
	std::pair< epicsTimeStamp, double > jdiff;
	std::pair< epicsTimeStamp, double > jogH;
	std::pair< epicsTimeStamp, double > jogV;
	std::pair< epicsTimeStamp, double > jog;
	std::pair< epicsTimeStamp, int > enH;
	std::pair< epicsTimeStamp, int > enV;
	std::pair< epicsTimeStamp, int > exH;
	std::pair< epicsTimeStamp, int > exV;
	std::pair< epicsTimeStamp, int > triggerH;
	std::pair< epicsTimeStamp, int > triggerV;
	std::pair< epicsTimeStamp, int > trigger;
	std::pair< epicsTimeStamp, int > readyH;
	std::pair< epicsTimeStamp, int > readyV;
	std::pair< epicsTimeStamp, int > ready;
	std::pair< epicsTimeStamp, int > movingH;
	std::pair< epicsTimeStamp, int > movingV;
	std::pair< epicsTimeStamp, int > moving;
	std::pair< epicsTimeStamp, int > maxposH;
	std::pair< epicsTimeStamp, int > maxposV;
	std::pair< epicsTimeStamp, int > maxpos;
	std::string name;
	std::string screenType;
	// GETTERS
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
	bool isMover() const;
	bool isVMover() const;
	bool isHVMover() const;
	bool isPneumatic() const;
	STATE getScreenState() const;
	STATE getScreenSetState() const;
	std::string getScreenType() const;
	std::vector< STATE > getAvailableDevices() const;
	bool isScreenInState(STATE sta) const;
	bool isYAGIn() const;
	bool isHElement(STATE e) const;
	bool isVElement(STATE e) const;
	bool isPElement(STATE e) const;
	bool isHEnabled() const;
	bool isVEnabled() const;
	bool isRFCageIn() const;
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
	void insertYAG();
	void makeReadEqualSet();
	void makeSetEqualRead();
	void moveScreenOut();
	void resetPosition();
	bool setScreenSDEV(STATE& state);
	bool setScreenTrigger(const int& value, TYPE type);
	bool moveScreenTo(STATE& state);
	bool setScreenSetState(STATE& state);
	bool setTGTPOS(const double& value, TYPE type);
	bool setJOG(const double& value, TYPE type);
	bool setTRIGGER(const int& value, TYPE type);
	bool setEX(const int& value, TYPE type);
	bool setEN(const int& value, TYPE type);
	bool setDEVSTATE(int& state, TYPE type);
	bool setGETDEV(int& state, TYPE type);
	bool setSDEV(int& state, TYPE type);
	int findByValue(std::map<int, STATE> mapOfElemen, STATE value);
	bool isElement(std::map<int, STATE> mapOfElemen, STATE value);
	friend class EPICSScreenInterface;
protected:
	//what else does a screen need?
	std::vector<std::string> aliases;
	bool has_camera;
};

#endif //SCREEN_H_