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
	/*! Get Lighting state.
	@param[out] map<string, STATE>, value for each Light */
	std::map<std::string, STATE> getLightingState()const;

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




	friend class LightingFactory;
	friend class EPICSLightingInterface;
	friend class PILaserSystem;
protected:
	/*! pdbm_level of the LLRF trace interlock (interlocks are only applicable to POWER traces) */
	std::pair<epicsTimeStamp, STATE > vela_led_state;
	std::pair<epicsTimeStamp, STATE > clara_led_state;
	std::pair<epicsTimeStamp, STATE > injector_hall_lighting_state; // todo is thsi a good name ?? idk


};


#endif //LIGHTING_H_