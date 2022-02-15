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
/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control screens on VELA/CLARA.
	 A collection of these classes will be stored in map in the ScreenFactory class.
 */
class Screen : public Hardware
{
public:
	/*! Default constructor call for Screen object*/
	Screen();
	/*! Custom constructor for Screen object
		@param[in] screenParametersAndValuesMap: strings defining parameters extracted from YAML config files
		@param[in] mode: Defines the STATE of Screen we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)
	*/
	Screen(const std::map<std::string, std::string>& screenParametersAndValuesMap, STATE mode);
	/*! Copy constructor call for Screen object*/
	Screen(const Screen& copyScreen);
	/*! EPICSScreenInterface for screen-specifc calls to EPICS*/
	EPICSScreenInterface_sptr epicsInterface;
	/*! Goes through all PV records stored in ScreenRecords namespace, creates pvStructs and sets up their record/fullPVName before
	inserting them into a vector of pvStructs which is later used by the ScreenFactory to setup connections to EPICS.
	@param[in] recordlist: strings defining which states are avilable for a given screen based on screen_type.
	*/
	void setPVStructs(std::vector<std::string> recordList);
	/*! Returns the name of the screen.
	@param[out] name: the name of the screen.
	*/
	std::string getScreenName() const;
	/*! A function to retrieve the aliases for a given Screen object
		@param[out] vector of aliases that the Screen is also known by*/
	std::vector<std::string> getAliases() const;

#ifdef BUILD_PYTHON
	/*! Retrieve the aliases for a given Screen object, Python version. 
		@param[out] list: aliases that the Screen is also known by, set in config yaml */
	boost::python::list getAliases_Py() const;
#endif //BUILD_PYTHON
	/*! A map for storing the parameters extracted from YAML config files and their values */
	std::map<std::string, std::string> screenParametersAndValuesMap;
	LoggingSystem messenger;
	/*! turns debug messaging on for this Screen instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this Screen instance*/
	void debugMessagesOff();
	/*! turns messaging on for this Screen instance*/
	void messagesOn();
	/*! turns messaging off for this Screen instance*/
	void messagesOff();
	// SCREEN STATES 
	/*! storage variable for current state of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, STATE > screenStateH;
	/*! storage variable for current set state of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, STATE > screenSetStateH;
	/*! storage variable for current state of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, STATE > screenStateV;
	/*! storage variable for currentset  state of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, STATE > screenSetStateV;
	/*! storage variable for current state of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, STATE > screenState;
	/*! storage variable for current set state of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, STATE > screenSetState;
	/*! storage variable for current actuator position of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > actposH;
	/*! storage variable for current actuator position of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > actposV;
	/*! storage variable for current actuator position of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > actpos;
	/*! storage variable for the target actuator position of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > tgtposH;
	/*! storage variable for the target actuator position of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > tgtposV;
	/*! storage variable for the target actuator position of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > tgtpos;
	/*! storage variable for the device position of Screen (in mm) in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > devcentH;
	/*! storage variable for the device position of Screen (in mm) in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > devcentV;
	/*! storage variable for the device position of Screen (in mm) in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > devcent;
	/*! storage variable for the position deviation from the last set state of Screen (in mm) in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > jdiffH;
	/*! storage variable for the position deviation from the last set state of Screen (in mm) in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > jdiffV;
	/*! storage variable for the position deviation from the last set state of Screen (in mm) in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > jdiff;
	/*! storage variable for the set position deviation from the last set state of Screen (in mm) in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > jogH;
	/*! storage variable for the set position deviation from the last set state of Screen (in mm) in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > jogV;
	/*! storage variable for the set position deviation from the last set state of Screen (in mm) in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > jog;
	/*! storage variable for the enable state of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > enH;
	/*! storage variable for the enable state of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > enV;
	/*! storage variable for the enable state of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > en;
	/*! storage variable for the fire state of Screen in the horizontal direction and the last timestamp retrieved from EPICS. (advanced use)*/
	std::pair< epicsTimeStamp, int > exH;
	/*! storage variable for the fire state of Screen in the vertical direction and the last timestamp retrieved from EPICS. (advanced use)*/
	std::pair< epicsTimeStamp, int > exV;
	/*! storage variable for the fire state of Screen in the last active direction and the last timestamp retrieved from EPICS. (advanced use)*/
	std::pair< epicsTimeStamp, int > ex;
	/*! storage variable for the trigger state of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > triggerH;
	/*! storage variable for the trigger state of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > triggerV;
	/*! storage variable for the trigger state of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > trigger;
	/*! storage variable for the ready state of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > readyH;
	/*! storage variable for the ready state of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > readyV;
	/*! storage variable for the ready state of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > ready;
	/*! storage variable for the moving state of Screen in the horizontal direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > movingH;
	/*! storage variable for the moving state of Screen in the vertical direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > movingV;
	/*! storage variable for the moving state of Screen in the last active direction and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, int > moving;
	/*! storage variable giving 0 if the screen is not on the horizontal direction or 1 if it is, and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > maxposH;
	/*! storage variable giving 0 if the screen is not on the vertical direction or 1 if it is, and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > maxposV;
	/*! storage variable giving 0 if the screen is not on the last active direction or 1 if it is, and the last timestamp retrieved from EPICS.*/
	std::pair< epicsTimeStamp, double > maxpos;
	/*! the name of the screen.*/
	std::string name;
	/*! the name of the camera associated with the screen.*/
	std::string cameraName;
	/*! the type of the screen.*/
	TYPE screenType;
	/*! the available devices for the screen.*/
	std::vector< STATE > screenDeviceVector;
	/*! true if the screen has a camera associated with it in the YAML file.*/
	bool has_camera;
	/*! the position along the beamline of the screen.*/
	double position;
	// GETTERS
	/*! the horizontal set state of the screen.
	@param[out] STATE: the horizontal screen set state.*/
	STATE getSetStateH() const;
	/*! the vertical set state of the screen.
	@param[out] STATE: the vertical screen set state.*/
	STATE getSetStateV() const;
	/*! the set state of the screen in the last active direction.
	@param[out] STATE: the screen set state in the last active direction.*/
	STATE getSetState() const;
	/*! the current horizontal state of the screen.
	@param[out] STATE: the horizontal screen state.*/
	STATE getStateH() const;
	/*! the vertical state of the screen.
	@param[out] STATE: the current vertical screen state.*/
	STATE getStateV() const;
	/*! the state of the screen in the last active direction.
	@param[out] STATE: the current screen state in the last active direction.*/
	STATE getState() const;
	/*! the horizontal actuator position of the screen.
	@param[out] double: the horizontal actuator position.*/
	double getACTPOSH() const;
	/*! the vertical actuator position of the screen.
	@param[out] double: the vertical actuator position.*/
	double getACTPOSV() const;
	/*! the horizontal target actuator position of the screen.
	@param[out] double: the horizontal target actuator position.*/
	double getTGTPOSH() const;
	/*! the vertical target actuator position of the screen.
	@param[out] double: the vertical target actuator position.*/
	double getTGTPOSV() const;
	/*! the target actuator position of the screen in the last active direction.
	@param[out] double: the target actuator position.*/
	double getTGTPOS() const;
	/*! the active axis of the screen.
	@param[out] double: gives 1 if the screen is on the horizontal axis.*/
	double getDEVCENTH() const;
	/*! the active axis of the screen.
	@param[out] double: gives 1 if the screen is on the vertical axis.*/
	double getDEVCENTV() const;
	/*! the active axis of the screen.
	@param[out] double: gives 1 if the screen is on the last active axis.*/
	double getDEVCENT() const;
	/*! the position deviation (in mm) of the screen from the last screenStateH.
	@param[out] double: the position deviation in mm.*/
	double getJDiffH() const;
	/*! the position deviation (in mm) of the screen from the last screenStateV.
	@param[out] double: the position deviation in mm.*/
	double getJDiffV() const;
	/*! the requested position deviation (in mm) of the screen from the last screenStateH.
	@param[out] double: the requested position deviation in mm.*/
	double getJOGH() const;
	/*! the requested position deviation (in mm) of the screen from the last screenStateV.
	@param[out] double: the position deviation in mm.*/
	double getJOGV() const;
	/*! the horizontal enabled state of the screen.
	@param[out] int: 1 if the horizontal stage is enabled.*/
	int getENH() const;
	/*! the vertical enabled state of the screen.
	@param[out] int: 1 if the vertical stage is enabled.*/
	int getENV() const;
	/*! the move execution state of the screen in the horizontal direction (advanced use).
	@param[out] int: 1 if the move request has been executed.*/
	int getEXH() const;
	/*! the move execution state of the screen in the vertical direction (advanced use).
	@param[out] int: 1 if the move request has been executed.*/
	int getEXV() const;
	/*! the trigger state of the screen in the horizontal direction -- required after state has been set.
	@param[out] int: 1 if the move request has been triggered.*/
	int getTriggerH() const;
	/*! the trigger state of the screen in the vertical direction -- required after state has been set.
	@param[out] int: 1 if the move request has been triggered.*/
	int getTriggerV() const;
	/*! the trigger state of the screen in the last active direction -- required after state has been set.
	@param[out] int: 1 if the move request has been triggered.*/
	int getTrigger() const;
	/*! the ready state of the screen in the horizontal direction.
	@param[out] int: 1 if the screen is ready to move horizontally.*/
	int getReadyH() const;
	/*! the ready state of the screen in the vertical direction.
	@param[out] int: 1 if the screen is ready to move vertically.*/
	int getReadyV() const;
	/*! the ready state of the screen in the last active direction.
	@param[out] int: 1 if the screen is ready to move.*/
	int getReady() const;
	/*! the moving state of the screen in the horizontal direction.
	@param[out] int: 1 if the screen is moving horizontally.*/
	int getMovingH() const;
	/*! the moving state of the screen in the vertical direction.
	@param[out] int: 1 if the screen is moving vertically.*/
	int getMovingV() const;
	/*! the moving state of the screen in the last active.
	@param[out] int: 1 if the screen is moving.*/
	int getMoving() const;
	/*! the current screen axis.
	@param[out] double: 1 if the screen on the horizontal axis.*/
	double getMaxPosH() const;
	/*! the current screen axis.
	@param[out] double: 1 if the screen on the vertical axis.*/
	double getMaxPosV() const;
	/*! the current screen axis.
	@param[out] double: 1 if the screen on the last active axis.*/
	double getMaxPos() const;
	/*! is the horizontal stage retracted?
	@param[out] bool: true if the horizontal stage is out.*/
	bool isHOut() const;
	/*! is the vertical stage retracted?
	@param[out] bool: true if the horizontal stage is out.*/
	bool isVOut() const;
	/*! is the horizontal stage inserted?
	@param[out] bool: true if the horizontal stage is inserted.*/
	bool isHIn() const;
	/*! is the vertical stage inserted?
	@param[out] bool: true if the vertical stage is inserted.*/
	bool isVIn() const;
	/*! are both stages retracted?
	@param[out] bool: true if the both stages is out.*/
	bool is_HandV_OUT() const;
	/*! is the YAG screen inserted?
	@param[out] bool: true if the screen is in.*/
	bool isScreenIn() const;
	/*! is the horizontal stage moving?
	@param[out] bool: true if the horizontal stage is moving.*/
	bool isHMoving() const;
	/*! is the vertical stage moving?
	@param[out] bool: true if the vertical stage is moving.*/
	bool isVMoving() const;
	/*! is the pneumatic screen moving?
	@param[out] bool: true if the screen is moving.*/
	bool isPMoving() const;
	/*! is the screen moving?
	@param[out] bool: true if the screen is moving.*/
	bool isMoving() const;
	/*! is there any obstacle at the screen location?
	@param[out] bool: true if the screen is either retracted or the RF cage is in.*/
	bool isClearForBeam() const;
	/*! get the current screen state.
	@param[out] STATE: the current state of the screen.*/
	STATE getScreenState() const;
	/*! get the current screen state in the specified direction.
	@param[in] dir: the direction that you are interested in.
	@param[out] std::pair<STATE,TYPE>: the current state of the screen in the given direction.*/
	std::pair< STATE, TYPE > getScreenStatePair(TYPE dir) const;
	/*! get the current screen set state.
	@param[out] STATE: the current set state of the screen.*/
	STATE getScreenSetState() const;
	/*! get the current screen set state in the specified direction.
	@param[in] dir: the direction that you are interested in.
	@param[out] std::pair<STATE,TYPE>: the current set state of the screen in the given direction.*/
	std::pair< STATE, TYPE > getScreenSetStatePair(TYPE dir) const;
	/*! get the screen type.
	@param[out] TYPE: the type of the screen.*/
	TYPE getScreenType() const;
	/*! get the available devices for that screen.
	@param[out] std::vector<STATE>: the available devices, specified in the YAML config file.*/
	std::vector< STATE > getAvailableDevices() const;
	/*! query the current screen state.
	@param[in] state: the state that you are interested in.
	@param[out] STATE: the current state of the screen.*/
	bool isScreenInState(STATE sta) const;
	/*! is the YAG screen in?
	@param[out] bool: true if yes.*/
	bool isYAGIn() const;
	/*! is this state on the horizontal axis? (NB it may not be available for that screen -- use getAvailableDevices to find out.)
	@param[in] state: the state that you are interested in.
	@param[out] bool: true if yes.*/
	bool isHElement(STATE e) const;
	/*! is this state on the vertical axis? (NB it may not be available for that screen -- use getAvailableDevices to find out.)
	@param[in] state: the state that you are interested in.
	@param[out] bool: true if yes.*/
	bool isVElement(STATE e) const;
	/*! is this state for a pneumatic screen? (NB it may not be available for that screen -- use getAvailableDevices to find out.)
	@param[in] state: the state that you are interested in.
	@param[out] bool: true if yes.*/
	bool isPElement(STATE e) const;
	/*! is the horizontal stage enabled?
	@param[out] bool: true if yes.*/
	bool isHEnabled() const;
	/*! is the vertical stage enabled?
	@param[out] bool: true if yes.*/
	bool isVEnabled() const;
	/*! is the RF cage inserted?
	@param[out] bool: true if yes.*/
	bool isRFCageIn() const;
	/*! get the current actuator position on the last active axis.
	@param[out] double: actuator position.*/
	double getACTPOS() const;
	/*! get the position deviation (in mm) from the last screenState on the last active axis.
	@param[out] double: position deviation.*/
	double getJDiff() const;
	/*! get the position on the axis of the specified STATE. Not yet enabled as I don't know all the positions.
	@param[out] double: device position.*/
	double getDevicePosition(STATE state) const;
	/*! get the position on the beamline of that screen.
	@param[out] double: screen position.*/
	double getPosition() const;
	/*! is the screen a vertical or H/V mover?
	@param[out] bool: true if the screen is any type of mover.*/
	bool isMover() const;
	/*! is the screen a vertical-only mover?
	@param[out] bool: true if the screen is a vertical mover.*/
	bool isVMover() const;
	/*! is the screen an H/V mover?
	@param[out] bool: true if the screen is an H/V mover.*/
	bool isHVMover() const;
	/*! is the screen a pneumatic type?
	@param[out] bool: true if the screen is a pneumatic type.*/
	bool isPneumatic() const;
	/*! is the screen a VELA pneumatic type?
	@param[out] bool: yes if true.*/
	bool isVELAPneumatic() const;
	/*! is the screen a VELA H/V mover type?
	@param[out] bool: yes if true.*/
	bool isVELAHVMover() const;
	/*! is the screen a VELA vertical mover type?
	@param[out] bool: yes if true.*/
	bool isVELAVMover() const;
	/*! is the screen a CLARA H/V mover type?
	@param[out] bool: yes if true.*/
	bool isCLARAHVMover() const;
	/*! is the screen a CLARA vertical mover type?
	@param[out] bool: yes if true.*/
	bool isCLARAVMover() const;
	/*! get the horizontal actuator position.
	@param[out] double: actuator position.*/
	double get_H_ACTPOS() const;
	/*! get the vertical actuator position.
	@param[out] double: actuator position.*/
	double get_V_ACTPOS() const;
	/// SETTERS
	/*! insert the YAG screen.*/
	void insertYAG();
	/*! move to screenSetState if screenState != screenSetState.*/
	void makeReadEqualSet();
	/*! move to screenState if screenSetState != screenState.*/
	void makeSetEqualRead();
	/*! move to RF cage.*/
	void moveScreenOut();
	/*! set the screen trigger (will move to screenSetState if it can).*/
	void resetPosition();
	/*! set the screenSetState.
	@param[in] STATE: desired screen state.
	@param[out] bool: true if successful. */
	bool setScreenSDEV(STATE& state);
	/*! set the trigger on the active axis, activating the move to screenSetState.
	@param[in] value: 1 to trigger.
	@param[out] bool: true if successful. */
	bool setScreenTrigger(const int& value);
	/*! set the trigger on the specified axis, activating the move to screenSetState.
	@param[in] value: 1 to trigger.
	@param[in] TYPE: direction to fire.
	@param[out] bool: true if successful. */
	bool setScreenTriggerWDir(const int& value, TYPE type);
	/*! move the screen to the specified STATE.
	@param[in] state: desired screenSetState.
	@param[out] bool: true if successful. */
	bool moveScreenTo(STATE state);
	/*! set screenSetState.
	@param[in] STATE: desired screenSetState.
	@param[out] bool: true if successful. */
	bool setScreenSetState(STATE state);
	/*! move screen from current position in mm.
	@param[in] value: desired position deviation.
	@param[out] bool: true if successful. */
	bool jogScreen(const double& value);
	/*! set the actuator postiion in the given direction.
	@param[in] value: desired position.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setPosition(const double& value, TYPE type);
	/*! EPICS call to set the target actuator postiion in the given direction (will not move the screen -- use setEX to enable, or setPosition). Advanced use.
	@param[in] value: desired position.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setTGTPOS(const double& value, TYPE type);
	/*! EPICS call to set the postiion deviation in the given direction (will not move the screen -- use setTrigger to enable, or jogScreen).
	@param[in] value: desired position.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setJOG(const double& value, TYPE type);
	/*! EPICS call to trigger the move in the given direction (moveScreenTo should be used instead).
	@param[in] value: desired value.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setTRIGGER(const int& value, TYPE type);
	/*! EPICS call to set the moving variable.
	@param[in] value: desired value.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setMOVING(const int& value, TYPE type);
	/*! EPICS call to set the current axis in the given direction.
	@param[in] value: desired value.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setMAXPOS(const double& value, TYPE type);
	/*! EPICS call to set the ready state in the given direction.
	@param[in] value: desired value.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setREADY(const int& value, TYPE type);
	/*! EPICS call to set the moving execution state (advanced use).
	@param[in] value: desired value.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setEX(const int& value, TYPE type);
	/*! EPICS call to set the enabled state in the given direction.
	@param[in] value: desired value.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setEN(const int& value, TYPE type);
	/*! EPICS call to set the current position (in mm) on the given axis.
	@param[in] state: desired value.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setDEVSTATE(int& state, TYPE type);
	/*! EPICS call to set the device selected.
	@param[in] state: desired state.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setGETDEV(int& state, TYPE type);
	/*! EPICS call to set the screenSetState in the given direction.
	@param[in] value: desired state.
	@param[in] TYPE: desired direction.
	@param[out] bool: true if successful. */
	bool setSDEV(int state, TYPE type);
	/*! find the value to send to EPICS corresponding to the STATE enum (from ScreenPVRecords).
	@param[in] map: map of elements from ScreenPVRecords.
	@param[in] STATE: state that you wish to find.
	@param[out] int: the integer corresponding to that enum in EPICS. */
	int findByValue(std::map<int, STATE> mapOfElemen, STATE value);
	/*! check if a STATE belongs to a certain map from ScreenPVRecords.
	@param[in] map: map of elements from ScreenPVRecords.
	@param[in] STATE: state that you wish to find.
	@param[out] bool: true if yes. */
	bool isElement(std::map<int, STATE> mapOfElemen, STATE value) const;




	friend class EPICSScreenInterface;
protected:
	//what else does a screen need?
	std::vector<std::string> aliases;
};
/**\copydoc Hardware*/
/**@}*/
#endif //SCREEN_H_