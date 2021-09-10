#include <EPICSLightingInterface.h>
#include "LightingPVRecords.h"
#include <GlobalStateEnums.h>
#include <iostream>




EPICSLightingInterface::EPICSLightingInterface() : EPICSInterface()
{
}

EPICSLightingInterface::~EPICSLightingInterface()
{
}


void EPICSLightingInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == LightingRecords::CLARA_LED_Sta)
	{
		pvStruct.updateFunction = this->update_CLARA_LED_Sta;
	}
	else if (pvStruct.pvRecord == LightingRecords::VELA_LED_Sta)
	{
		pvStruct.updateFunction = this->update_VELA_LED_Sta;
	}	
	else if (pvStruct.pvRecord == LightingRecords::BA1_LIGHT_Sta)
	{
		pvStruct.updateFunction = this->update_BA1_LIGHT_Sta;
	}
	else if (pvStruct.pvRecord == LightingRecords::ACCELERATOR_HALL_LIGHT_Sta)
	{
		pvStruct.updateFunction = this->update_ACCELERATOR_HALL_LIGHT_Sta;
	}
	else
	{
		//messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSLightingInterface::update_CLARA_LED_Sta(const struct event_handler_args args)
{
	Lighting* recastLighting = getHardwareFromArgs<Lighting>(args);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastLighting->clara_led_state.first = new_value.first;
	switch (new_value.second)
	{
	case	 GlobalConstants::zero_ushort: recastLighting->clara_led_state.second = STATE::OFF; break;
	case	 GlobalConstants::one_ushort:  recastLighting->clara_led_state.second = STATE::ON;  break;
	default: recastLighting->clara_led_state.second = STATE::ERR;
	}
	//messenger.printDebugMessage(recastLighting->hardwareName, " update_CLARA_LED_Sta = ",
	//	ENUM_TO_STRING(recastLighting->clara_led_state.second));
}
void EPICSLightingInterface::update_VELA_LED_Sta(const struct event_handler_args args)
{
	Lighting* recastLighting = getHardwareFromArgs<Lighting>(args);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastLighting->vela_led_state.first = new_value.first;
	switch (new_value.second)
	{
	case	 GlobalConstants::zero_ushort: recastLighting->vela_led_state.second = STATE::OFF; break;
	case	 GlobalConstants::one_ushort:  recastLighting->vela_led_state.second = STATE::ON;  break;
	default: recastLighting->vela_led_state.second = STATE::ERR;
	}
	//messenger.printDebugMessage(recastLighting->hardwareName, " update_VELA_LED_Sta = ",
	//	ENUM_TO_STRING(recastLighting->vela_led_state.second));
}


void EPICSLightingInterface::update_ACCELERATOR_HALL_LIGHT_Sta(const struct event_handler_args args)
{
	Lighting* recastLighting = getHardwareFromArgs<Lighting>(args);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastLighting->accelerator_hall_lighting_state.first = new_value.first;
	switch (new_value.second)
	{
	case	 GlobalConstants::zero_ushort: recastLighting->accelerator_hall_lighting_state.second = STATE::OFF; break;
	case	 GlobalConstants::one_ushort:  recastLighting->accelerator_hall_lighting_state.second = STATE::ON;  break;
	default: recastLighting->accelerator_hall_lighting_state.second = STATE::ERR;
	}
	//messenger.printDebugMessage(recastLighting->hardwareName, " update_CLARA_LED_Sta = ",
	//	ENUM_TO_STRING(recastLighting->clara_led_state.second));
}

void EPICSLightingInterface::update_BA1_LIGHT_Sta(const struct event_handler_args args)
{
	Lighting* recastLighting = getHardwareFromArgs<Lighting>(args);
	std::pair<epicsTimeStamp, unsigned short> new_value = getTimeStampUnsignedShortPair(args);
	recastLighting->ba1_lighting_state.first = new_value.first;
	switch (new_value.second)
	{
	case	 GlobalConstants::zero_ushort: recastLighting->ba1_lighting_state.second = STATE::OFF; break;
	case	 GlobalConstants::one_ushort:  recastLighting->ba1_lighting_state.second = STATE::ON;  break;
	default: recastLighting->ba1_lighting_state.second = STATE::ERR;
	}
	//messenger.printDebugMessage(recastLighting->hardwareName, " update_CLARA_LED_Sta = ",
	//	ENUM_TO_STRING(recastLighting->clara_led_state.second));
}