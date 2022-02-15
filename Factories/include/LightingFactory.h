#ifndef LIGHTING_FACTORY_H_
#define LIGHTING_FACTORY_H_
#include <ConfigReader.h>
#include <Lighting.h>
#include <GlobalStateEnums.h>
#include <vector>

class LightingFactory
{
public:
	LightingFactory();
	LightingFactory(STATE mode);
	LightingFactory(STATE mode, const std::string& primeLatticeLocation);
	LightingFactory(const LightingFactory& copyFactory);
	~LightingFactory();
	bool setup(std::string version);
	LoggingSystem messenger;
	bool hasBeenSetup;

	Lighting& getLighting(const std::string& name);

	std::map<std::string, STATE> getLEDState(const std::string& name)const;
	std::map<std::string, STATE> getLightingState(const std::string& name)const;
#ifdef BUILD_PYTHON
	boost::python::dict getLightingState_Py(const std::string& name)const;
	boost::python::dict getLEDState_Py(const std::string& name)const;
#endif //BUILD_PYTHON
	bool allLEDOn(const std::string& name);
	bool allLEDOff(const std::string& name);
	STATE getClaraLEDState(const std::string& name)const;
	bool setClaraLEDOn(const std::string& name);
	bool setClaraLEDOff(const std::string& name);
	bool isClaraLEDOn(const std::string& name)const;
	bool isClaraLEDOff(const std::string& name)const;
	bool toggleClaraLED(const std::string& name);
	STATE getVelaLEDState(const std::string& name)const;
	bool setVelaLEDOn(const std::string& name);
	bool setVelaLEDOff(const std::string& name);
	bool isVelaLEDOn(const std::string& name)const;
	bool isVelaLEDOff(const std::string& name)const;
	bool toggleVelaLED(const std::string& name);

	std::string getFullName(const std::string& name)const;

	/*! Get the state of the BA1 Light.
	@param[out] STATE, ON, OFF etc	*/
	STATE getBA1LightState(const std::string& name)const;
	/*! Turn the BA1 Light on.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setBA1LightOn(const std::string& name);
	/*! Turn the BA1 Light off.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setBA1LightOff(const std::string& name);
	/*! Is the BA1 Light on,
	@param[out] bool, is state == ON.*/
	bool isBA1LightOn(const std::string& name)const;
	/*! Is the BA1 Light off,
	@param[out] bool, is state == OFF.*/
	bool isBA1LightOff(const std::string& name)const;
	/*! Toggle between ON / OFF states for the BA11 light,
	@param[out] bool,  if command got sent to EPICS (not if it was accepted)	*/
	bool toggleBA1Light(const std::string& name);

	/*! Get the state of the Hall 1 Light.
	@param[out] STATE, ON, OFF etc	*/
	STATE getAcceleratorHallLightState(const std::string& name)const;
	/*! Turn the Accelerator Hall  Light on.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setAcceleratorHallLightOn(const std::string& name);
	/*! Turn the Accelerator Hall  Light off.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setAcceleratorHallLightOff(const std::string& name);
	/*! Is the Accelerator Hall Light on,
	@param[out] bool, is state == ON.*/
	bool isAcceleratorHallLightOn(const std::string& name)const;
	/*! Is the Accelerator Hall  Light off,
	@param[out] bool, is state == OFF.*/
	bool isAcceleratorHallLightOff(const std::string& name)const;
	/*! Toggle between ON / OFF states for the Accelerator Hall light,
	@param[out] bool,  if command got sent to EPICS (not if it was accepted)	*/
	bool toggleAcceleratorHallLight(const std::string& name);


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();

private:
	/*! The kind of Factory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	hardware objects when they are created by ConfigReader*/
	STATE mode;
	void setMonitorStatus(pvStruct& pvStruct);
	void setupChannels();

	std::map<std::string, Lighting> lightingMap;


	// private
	ConfigReader reader;
	// used when we need to return values from a requested camera name that does not exist 
	Lighting dummy_light;
};

#endif // LIGHTING_FACTORY_H_