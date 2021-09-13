#ifndef LIGHTING_H_
#define LIGHTING_H_

#include <Hardware.h>
#include <LoggingSystem.h>
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



	/*! Get LED states.
	@param[out] map<string, STATE>, value for each LED */
	std::map<std::string, STATE> getLEDState()const;
	/*! Get LED states, Python version.
	@param[out] dict, value for each LED */
	boost::python::dict getLEDState_Py()const;
	/*! Get Lighting state.
	@param[out] map<string, STATE>, value for each Light */
	std::map<std::string, STATE> getLightingState()const;
	/*! Get Lighting state, python version 
	@param[out] dict, value for each Light */
	boost::python::dict getLightingState_Py()const;
	/*! Turn on all LEDS.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool allLEDOn();
	/*! Turn off all LEDS.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool allLEDOff();

	/*! Get the state of the CLARA Phase 1 camera / screen LED.
	@param[out] STATE, ON, OFF etc	*/
	STATE getClaraLEDState()const;
	/*! Turn the LED on for CLARA Phase 1 camera / screen.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setClaraLEDOn();
	/*! Turn the LED off for CLARA Phase 1 camera / screen.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setClaraLEDOff();
	/*! Is the CLARA Phase 1 camera / screen LED on,
	@param[out] bool, is state == ON.*/
	bool isClaraLEDOn()const;
	/*! Is the CLARA Phase 1 camera / screen LED off,
	@param[out] bool, is state == OFF.*/
	bool isClaraLEDOff()const;
	/*! Toggle between ON / OFF states for the CLARA Phase 1  camera / screen LED,
	@param[out] bool,  if command got sent to EPICS (not if it was accepted)	*/
	bool toggleClaraLED();

	/*! Get the state of the VELA camera / screen LED.
	@param[out] STATE, ON, OFF etc	*/
	STATE getVelaLEDState()const;
	/*! Turn the LED on for VELA camera / screen.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setVelaLEDOn();
	/*! Turn the LED off for VELA camera / screen.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setVelaLEDOff();
	/*! Is the VELA camera / screen LED on, 
	@param[out] bool, is state == ON.*/
	bool isVelaLEDOn()const;
	/*! Is the VELA camera / screen LED off,
	@param[out] bool, is state == OFF.*/
	bool isVelaLEDOff()const;
	/*! Toggle between ON / OFF states for the VELA camera / screen LED,
	@param[out] bool,  if command got sent to EPICS (not if it was accepted)	*/
	bool toggleVelaLED();


	/*! Get the state of the BA1 Light.
	@param[out] STATE, ON, OFF etc	*/
	STATE getBA1LightState()const;
	/*! Turn the BA1 Light on.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setBA1LightOn();
	/*! Turn the BA1 Light off.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setBA1LightOff();
	/*! Is the BA1 Light on,
	@param[out] bool, is state == ON.*/
	bool isBA1LightOn()const;
	/*! Is the BA1 Light off,
	@param[out] bool, is state == OFF.*/
	bool isBA1LightOff()const;
	/*! Toggle between ON / OFF states for the BA11 light,
	@param[out] bool,  if command got sent to EPICS (not if it was accepted)	*/
	bool toggleBA1Light();

	/*! Get the state of the Hall 1 Light.
	@param[out] STATE, ON, OFF etc	*/
	STATE getAcceleratorHallLightState()const;
	/*! Turn the Accelerator Hall  Light on.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setAcceleratorHallLightOn();
	/*! Turn the Accelerator Hall  Light off.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setAcceleratorHallLightOff();
	/*! Is the Accelerator Hall Light on,
	@param[out] bool, is state == ON.*/
	bool isAcceleratorHallLightOn()const;
	/*! Is the Accelerator Hall  Light off,
	@param[out] bool, is state == OFF.*/
	bool isAcceleratorHallLightOff()const;
	/*! Toggle between ON / OFF states for the Accelerator Hall light,
	@param[out] bool,  if command got sent to EPICS (not if it was accepted)	*/
	bool toggleAcceleratorHallLight();


	friend class LightingFactory;
	friend class EPICSLightingInterface;
	friend class PILaserSystem;
protected:
	/*! pdbm_level of the LLRF trace interlock (interlocks are only applicable to POWER traces) */
	std::pair<epicsTimeStamp, STATE > vela_led_state;
	std::pair<epicsTimeStamp, STATE > clara_led_state;
	std::pair<epicsTimeStamp, STATE > accelerator_hall_lighting_state; // todo is this a good name ?? idk
	std::pair<epicsTimeStamp, STATE > ba1_lighting_state; // todo is this a good name ?? idk


};


#endif //LIGHTING_H_