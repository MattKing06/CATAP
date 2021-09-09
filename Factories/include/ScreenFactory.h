#ifndef SCREEN_FACTORY_H_
#define SCREEN_FACTORY_H_

#include <LoggingSystem.h>
#include <ConfigReader.h>
#include <Screen.h>
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

/** @addtogroup factories
 @{*/
 /*! A class to store, setup, and perform operations on multiple Screen objects*/
class ScreenFactory
{
public:
	/*! Default constructor for ScreenFactory.
	Calls the custom constructor with mode = OFFLINE.*/
	ScreenFactory();
	/*! Custom constructor for ScreenFactory
	@param[in] mode The type of ScreenFactory we wish to create.
	PHYSICAL ScreenFactory will perform operations on CLARA/VELA Screens.
	VIRTUAL ScreenFactory will perform operations of Virtual Screens.
	OFFLINE ScreenFactory will perform operations on Offline Screens.*/
	ScreenFactory(STATE mode);
	ScreenFactory(STATE mode, const std::string& primeLatticeLocation);
	/*! Copy constructor for ScreenFactory (used for = operator)
	@param[in] copyScreenFactory the ScreenFactory we want to copy over.*/
	ScreenFactory(const ScreenFactory& copyScreenFactory);
	/*! ScreenFactory destructor.
	Goes through all Screen objects in the screenMap and removes connections/subscriptions to EPICS if they exist.*/
	~ScreenFactory();
	/*! The kind of ScreenFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	Screen hardware objects when they are created by ConfigReader*/
	STATE mode;
	/*! Flag to say whether the ScreenFactory setup function has been completed successfully.*/
	bool hasBeenSetup;
	/*! Map containing all Screens created from ConfigReader, keyed by harwdare name*/
	std::map<std::string, Screen> screenMap;
	/*! ConfigReader to parse YAML config files and create associated Screen objects*/
	ConfigReader reader;
	/*! Used to print messages both normal and debug, also sets the On/Off flags for debug and normal messages*/
	LoggingSystem messenger;
	/*! The main function that sets up EPICS connections and subscriptions to all
	screens in the Screen map. We set up all channels before sending them to EPICS
	for efficiency. After channels, the remaining data for pvStructs are set as well
	as any subscriptions that are needed for monitoring.
	@param[in] version NOT IN USE
	@param[out] hasBeenSetup false if no YAML config files to parse, true otherwise.*/
	bool setup(const std::string& VERSION);
	/*! passes an empty screenMap to the configReader to be populated by Screen objects that are
	created from the YAML Config files.*/
	void populateScreenMap();
	/*! iterates over all screens in screenMap and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();
	/*! sets the pvStruct monitor flag to true if we want to monitor the PV
		and otherwise this flag is set to false
		@param[in] pvStruct the pvStruct which we want to set the monitor flag of.
	*/
	void retrievemonitorStatus(pvStruct& pvStruct);
	/*! returns Screen by name
	@param name checks the name in the alias map if not full name.
			and returns the Screen hardware stored in screenMap at that full name
	*/
	Screen& getScreen(const std::string& fullScreenName);
	/*!gets all of the hardware names associated with all of the screens in screenMap
	@param[out] screenNames :a vector containing all of the harwdare names for the screens in screenMap*/
	std::vector<std::string> getAllScreenNames();
	/*!gets all the full name of a certain screen.
	@param[in] name: the name of the screen.
	@param[out] screenNames :a vector containing all of the harwdare names for the screens in screenMap*/
	std::string getScreenName(const std::string& name);
	STATE state;
	/*!returns true if the horizontal stage of the given screen is retracted.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the horizontal stage of the given screen is retracted*/
	bool isHOut(const std::string& name);
	/*!returns true if the vertical stage of the given screen is retracted.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the vertical stage of the given screen is retracted*/
	bool isVOut(const std::string& name);
	/*!returns true if the horizontal stage of the given screen is inserted.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the horizontal stage of the given screen is inserted*/
	bool isHIn(const std::string& name);
	/*!returns true if the vertical stage of the given screen is inserted.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the vertical stage of the given screen is inserted*/
	bool isVIn(const std::string& name);
	/*!returns true if the both stages of the given screen are retracted. (works for vertical-only movers.)
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the both stages of the given screen are retracted*/
	bool is_HandV_OUT(const std::string& name);
	/*!returns true if the YAG screen is inserted.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the YAG screen is inserted*/
	bool isScreenIn(const std::string& name);
	/*!returns true if the horizontal stage of the screen is moving.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the horizontal stage of the screen is moving*/
	bool isHMoving(const std::string& name);
	/*!returns true if the vertical stage of the screen is moving.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the vertical stage of the screen is moving*/
	bool isVMoving(const std::string& name);
	/*!returns true if the pneumatic screen is moving.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the pneumatic screen is moving*/
	bool isPMoving(const std::string& name);
	/*!returns true if the screen is moving in any direction.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is moving*/
	bool isMoving(const std::string& name);
	/*!returns true if there is no obstacle for the beam at the screen position (STATE is either RF cage or RETRACTED.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if there is no obstacle for the beam.*/
	bool isClearForBeam(const std::string& name);
	/*!returns true if the screen has a vertical and/or horizontal moving stage (i.e. not pneumatic).
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is a mover.*/
	bool isMover(const std::string& name);
	/*!returns true if the screen has a vertical moving stage (i.e. not pneumatic).
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is a vertical mover.*/
	bool isVMover(const std::string& name);
	/*!returns true if the screen has a vertical and horizontal moving stage (i.e. not pneumatic).
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is an HV mover.*/
	bool isHVMover(const std::string& name);
	/*!returns true if the screen is pneumatic.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is pneumatic.*/
	bool isPneumatic(const std::string& name);
	/*!returns the current screen state (i.e. device position).
	@param[in] name: the name of the screen.
	@param[out] STATE: returns the current screen state.*/
	STATE getScreenState(const std::string& name);
	/*!returns the current screen set state (i.e. device position).
	@param[in] name: the name of the screen.
	@param[out] STATE: returns the current screen set state.*/
	STATE getScreenSetState(const std::string& name);
	/*!returns the current screen state (i.e. device position) in a given direction.
	@param[in] name: the name of the screen.
	@param[in] TYPE: the direction.
	@param[out] std::pair<STATE,TYPE>: returns the current screen state in a given direction.*/
	std::pair< STATE, TYPE > getScreenStatePair(const std::string& name, TYPE type);
	/*!returns the current screen set state (i.e. device position) in a given direction.
	@param[in] name: the name of the screen.
	@param[in] TYPE: the direction.
	@param[out] std::pair<STATE,TYPE>: returns the current screen set state in a given direction.*/
	std::pair< STATE, TYPE > getScreenSetStatePair(const std::string& name, TYPE type);
	/*!returns the type of screen device.
	@param[in] name: the name of the screen.
	@param[out] TYPE: returns the type of screen device*/
	TYPE getScreenType(const std::string& name);
	/*!returns the available devices for a given screen.
	@param[in] name: the name of the screen.
	@param[out] std::vector<STATE>: returns a vector of all available devices for that screen.*/
	std::vector< STATE > getAvailableDevices(const std::string& name);
	/*!returns the available devices for a given screen, Python version 
	@param[in] name: the name of the screen.
	@param[out] list: returns a list of all available devices for that screen.*/
	boost::python::list getAvailableDevices_Py(const std::string& name);
	/*!returns true if the screen is in the given state.
	@param[in] name: the name of the screen.
	@param[in] STATE: the state (i.e. device position).
	@param[out] bool: returns true if the screen is in a given state.*/
	bool isScreenInState(const std::string& name, STATE sta);
	/*!returns true if the YAG screen is inserted.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the YAG screen is inserted.*/
	bool isYAGIn(const std::string& name);
	/*!returns true if the given device is in the horizontal direction.
	@param[in] name: the name of the screen.
	@param[in] STATE: the state (i.e. device position).
	@param[out] bool: returns true if the device is a horizontal element.*/
	bool isHElement(const std::string& name, STATE e);
	/*!returns true if the given device is in the vertical direction.
	@param[in] name: the name of the screen.
	@param[in] STATE: the state (i.e. device position).
	@param[out] bool: returns true if the device is a vertical element.*/
	bool isVElement(const std::string& name, STATE e);
	/*!returns true if the given device is for a pneumatic screen.
	@param[in] name: the name of the screen.
	@param[in] STATE: the state (i.e. device position).
	@param[out] bool: returns true if the device is for a pneumatic screen.*/
	bool isPElement(const std::string& name, STATE e);
	/*!returns true if the horizontal stage is enabled.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if horizontal stage is enabled.*/
	bool isHEnabled(const std::string& name);
	/*!returns true if the vertical stage is enabled.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if vertical stage is enabled.*/
	bool isVEnabled(const std::string& name);
	/*!returns true if the RF cage is inserted.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the RF cage is enabled.*/
	bool isRFCageIn(const std::string& name);
	/*!returns the current actuator position.
	@param[in] name: the name of the screen.
	@param[out] double: returns the current actuator position.*/
	double getACTPOS(const std::string& name);
	/*!returns the difference in mm from the original set state.
	@param[in] name: the name of the screen.
	@param[out] double: returns the difference in mm from the original set state.*/
	double getJDiff(const std::string& name);
	/*!returns the position of a given device on the stage.
	@param[in] name: the name of the screen.
	@param[out] double: returns the position of a given STATE in mm.*/
	double getDevicePosition(const std::string& name, STATE state);
	/*!returns the position of the screen along the beamline.
	@param[in] name: the name of the screen.
	@param[out] double: returns the position (in metres) of the screen along the beamline.*/
	double getPosition(const std::string& name);
	/*!returns true if the screen is a VELA pneumatic type.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is a VELA pneumatic type.*/
	bool isVELAPneumatic(const std::string& name);
	/*!returns true if the screen is a VELA HV mover.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is a VELA HV mover.*/
	bool isVELAHVMover(const std::string& name);
	/*!returns true if the screen is a VELA vertical mover.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is a VELA vertical mover.*/
	bool isVELAVMover(const std::string& name);
	/*!returns true if the screen is a CLARA HV mover.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is a CLARA HV mover.*/
	bool isCLARAHVMover(const std::string& name);
	/*!returns true if the screen is a CLARA vertical mover.
	@param[in] name: the name of the screen.
	@param[out] bool: returns true if the screen is a CLARA vertical mover.*/
	bool isCLARAVMover(const std::string& name);
	/*!returns the horizontal actuator position.
	@param[in] name: the name of the screen.
	@param[out] double: returns the horizontal actuator position.*/
	double get_H_ACTPOS(const std::string& name);
	/*!returns the vertical actuator position.
	@param[in] name: the name of the screen.
	@param[out] double: returns the vertical actuator position.*/
	double get_V_ACTPOS(const std::string& name);
	/*!returns the states of the speficied screens.
	@param[in] std::vector<std::string> names: the names of the screens.
	@param[out] std::map<std::string,STATE>: returns a map of the current screen states, keyed by name.*/
	std::map<std::string, STATE> getScreenStates(std::vector<std::string> names);
	/*!returns the set states of the speficied screens.
	@param[in] std::vector<std::string> names: the names of the screens.
	@param[out] std::map<std::string,STATE>: returns a map of the current screen set states, keyed by name.*/
	std::map<std::string, STATE> getScreenSetStates(std::vector<std::string> names);
	/*!returns the speficied Screen hardware objects.
	@param[in] std::vector<std::string> names: the names of the screens.
	@param[out] std::map<std::string,Screen>: returns a map of Screen objects, keyed by name.*/
	std::map<std::string, Screen> getScreens(std::vector<std::string> names);
	/*!returns the set states of all screens.
	@param[out] std::map<std::string,STATE>: returns a map of all current screen set states, keyed by name.*/
	std::map<std::string, STATE> getAllScreenSetStates();
	/*!returns the states of all screens.
	@param[out] std::map<std::string,STATE>: returns a map of all current screen states, keyed by name.*/
	std::map<std::string, STATE> getAllScreenStates();
	/*!returns all Screen hardware objects.
	@param[out] std::map<std::string,Screen>: returns a map of all Screen objects, keyed by name.*/
	std::map<std::string, Screen> getAllScreens();
	/*!returns the current screen state
	@param[in] name: the name of the screen.
	@param[out] STATE: returns the current state of the screen.*/
	STATE getState(const std::string& name) const;
	/// SETTERS
	/*!moves the screen to the specified STATE.
	@param[in] name: the name of the screen.
	@param[in] STATE: the STATE to move the screen to. Use getAvailableDevices to see what is available for that screen.*/
	void moveScreenTo(const std::string& name, STATE state);
	/*!moves the YAG screen in.
	@param[in] name: the name of the screen.*/
	void insertYAG(const std::string& name);
	/*!moves the screen to the set position if screenState != screenSetState.
	@param[in] name: the name of the screen.*/
	void makeReadEqualSet(const std::string& name);
	/*!moves the screen to the read position if screenSetState != screenState.
	@param[in] name: the name of the screen.*/
	void makeSetEqualRead(const std::string& name);
	/*!moves the screen to the RF cage position.
	@param[in] name: the name of the screen.*/
	void moveScreenOut(const std::string& name);
	/*!enables the screen trigger (if the set state is different to the read state, or if a move has not been completed).
	@param[in] name: the name of the screen.*/
	void resetPosition(const std::string& name);
	/*!shifts the screen position from its current location.
	@param[in] name: the name of the screen.
	@param[in] jog: the amount to shift the screen position (in mm).*/
	void jogScreen(const std::string& name, const double jog);
	/*!move the screen to a specific position in a given direction (advanced use).
	@param[in] name: the name of the screen.
	@param[in] setPos: the desired position in mm.
	@param[in] type: the desired direction (H or V).*/
	void setPosition(const std::string& name, const double setPos, TYPE type);
	/*!set the screen set state to the specified device. This will NOT move the screen, only change the screenSetState
	@param[in] name: the name of the screen.
	@param[in] state: the desired screenState. Use getAvailableDevices to find out what states the screen can be in.
	@param[out] bool: returns true if successful.*/
	bool setScreenSDEV(const std::string& name, STATE& state);
	/*!tell the screen to start moving if the state has been set. 
	@param[in] name: the name of the screen.
	@param[in] value: Set 1 to fire.
	@param[out] bool: returns true if successful.*/
	bool setScreenTrigger(const std::string& name, const int& value);
	/*!tell the screen to start moving in a given direction if the state has been set.
	@param[in] name: the name of the screen.
	@param[in] value: Set 1 to fire.
	@param[in] type: the direction in which to move.
	@param[out] bool: returns true if successful.*/
	bool setScreenTriggerWDir(const std::string& name, const int& value, TYPE& type);
	/*!execute move (advanced use). To be used after using setTGTPOS.
	@param[in] name: the name of the screen.
	@param[in] value: Set 1 to fire.
	@param[in] type: the direction in which to move.
	@param[out] bool: returns true if successful.*/
	bool setEX(const std::string& name, const int& value, TYPE type);
	/*!enable the stage in the specified direction.
	@param[in] name: the name of the screen.
	@param[in] value: Set 1 for true.
	@param[in] direction: the direction of the stage to enable.
	@param[out] bool: returns true if successful.*/
	bool setEN(const std::string& name, const int& value, TYPE direction);
	/*!move the screen to a specific position in a given direction (advanced use). Equivalent to setPosition.
	@param[in] name: the name of the screen.
	@param[in] value: the desired position in mm.
	@param[in] direction: the desired direction (H or V).*/
	bool setTGTPOS(const std::string& name, const double& value, TYPE direction);
	/*!set the screen set state to the specified device. Equivalent to setScreenSDEV
	@param[in] name: the name of the screen.
	@param[in] state: the desired screenState. Use getAvailableDevices to find out what states the screen can be in.
	@param[out] bool: returns true if successful.*/
	bool setScreenSetState(const std::string& name, STATE state);

	/*!gets a python list of the hardware names associated with all of the screens in screenMap
	@param[out] list :a Python list containing all of the harwdare names for the screens in screenMap*/
	boost::python::list getAllScreenNames_Py();
	/*!returns a Python dict of all Screen hardware objects.
	@param[out] dict: returns a dict of all Screen objects, keyed by name.*/
	boost::python::dict getAllScreens_Py();
	/*!returns a Python dict of the set states of all screens.
	@param[out] dict: returns a dict of all current screen set states, keyed by name.*/
	boost::python::dict getAllScreenSetStates_Py();
	/*!returns a Python dict of the states of all screens.
	@param[out] dict: returns a dict of all current screen states, keyed by name.*/
	boost::python::dict getAllScreenStates_Py();
	/*!returns a Python dict of the states of the specified screens.
	@param[in] list: the names of the screens.
	@param[out] dict: returns a dict of the specified current screen states, keyed by name.*/
	boost::python::dict getScreenStates_Py(boost::python::list names);
	/*!returns a Python dict of the set states of the specified screens.
	@param[in] list: the names of the screens.
	@param[out] dict: returns a dict of the specified current screen set states, keyed by name.*/
	boost::python::dict getScreenSetStates_Py(boost::python::list names);
	/*!returns a Python dict of the specified screen objects.
	@param[in] list: the names of the screens.
	@param[out] dict: returns a dict of the specified screen objects, keyed by name.*/
	boost::python::dict getScreens_Py(boost::python::list names);

	/*! turns debug messages on for screenFactory and calls same function in all screens and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for screenFactory and calls same function in all screens and configReader*/
	void debugMessagesOff();
	/*! turns messages on for screenFactory and calls same function in all screens and configReader*/
	void messagesOn();
	/*! turns messages off for screenFactory and calls same function in all screens and configReader*/
	void messagesOff();
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	/*! returns true if messenger messages flag is true, false otherwise*/
	bool isMessagingOn();
};
/** @}*/
#endif // Screen_FACTORY_H_