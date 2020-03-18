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
//typedef void(*updateFunctionPtr)(struct event_handler_args args);
//class Screen;
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
	bool isMover(const std::string& name);
	bool isVMover(const std::string& name);
	bool isHVMover(const std::string& name);
	bool isPneumatic(const std::string& name);
	STATE getScreenState(const std::string& name);
	STATE getScreenSetState(const std::string& name);
	std::pair< STATE, TYPE > getScreenStatePair(const std::string& name, TYPE type);
	std::pair< STATE, TYPE > getScreenSetStatePair(const std::string& name, TYPE type);
	TYPE getScreenType(const std::string& name);
	std::vector< STATE > getAvailableDevices(const std::string& name);
	bool isScreenInState(const std::string& name, STATE sta);
	bool isYAGIn(const std::string& name);
	bool isHElement(const std::string& name, STATE e);
	bool isVElement(const std::string& name, STATE e);
	bool isPElement(const std::string& name, STATE e);
	bool isHEnabled(const std::string& name);
	bool isVEnabled(const std::string& name);
	bool isRFCageIn(const std::string& name);
	double getACTPOS(const std::string& name);
	double getJDiff(const std::string& name);
	double getDevicePosition(const std::string& name, STATE state);
	double getPosition(const std::string& name);
	bool isVELAPneumatic(const std::string& name);
	bool isVELAHVMover(const std::string& name);
	bool isCLARAHVMover(const std::string& name);
	bool isCLARAVMover(const std::string& name);
	double get_H_ACTPOS(const std::string& name);
	double get_V_ACTPOS(const std::string& name);
	std::map<std::string, STATE> getScreenStates(std::vector<std::string> names);
	std::map<std::string, STATE> getScreenSetStates(std::vector<std::string> names);
	std::map<std::string, Screen> getScreens(std::vector<std::string> names);
	std::map<std::string, STATE> getAllScreenSetStates();
	std::map<std::string, STATE> getAllScreenStates();
	std::map<std::string, Screen> getAllScreens();
	/// SETTERS
	void moveScreenTo(const std::string& name, STATE& state);
	void insertYAG(const std::string& name);
	void makeReadEqualSet(const std::string& name);
	void makeSetEqualRead(const std::string& name);
	void moveScreenOut(const std::string& name);
	void resetPosition(const std::string& name);
	void jogScreen(const std::string& name, const double jog);
	void setPosition(const std::string& name, const double setPos, TYPE type);
	bool setScreenSDEV(const std::string& name, STATE& state);
	bool setScreenTrigger(const std::string& name, const int& value);
	bool setScreenTriggerWDir(const std::string& name, const int& value, TYPE& type);
	bool setEX(const std::string& name, const int& value, TYPE type);
	bool setEN(const std::string& name, const int& value, TYPE direction);
	bool setTGTPOS(const std::string& name, const double& value, TYPE direction);
	bool setScreenSetState(const std::string& name, STATE state);
	STATE getState(const std::string& name) const;
	boost::python::list getAllScreenNames_Py();
	boost::python::dict getAllScreens_Py();
	boost::python::dict getAllScreenSetStates_Py();
	boost::python::dict getAllScreenStates_Py();
	boost::python::dict getScreenStates_Py(boost::python::list names);
	boost::python::dict getScreenSetStates_Py(boost::python::list names);
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