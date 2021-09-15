#include <EPICSShutterInterface.h>
#include <Shutter.h>
#include <ShutterPVRecords.h>
#include <bitset>
#include <iostream>
#include <string>
#include <algorithm>

LoggingSystem EPICSShutterInterface::messenger;

EPICSShutterInterface::EPICSShutterInterface()
{
}

EPICSShutterInterface::EPICSShutterInterface(const EPICSShutterInterface& copyInterface)
{
  // I ususally just print something here but could be used to actually copy stuff..
}

EPICSShutterInterface::~EPICSShutterInterface()
{
}
void EPICSShutterInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.monitor)
	{
		if (pvStruct.pvRecord == ShutterRecords::State)
		{
			messenger.printDebugMessage(pvStruct.pvRecord, " update function = updateShutterState" );
			pvStruct.updateFunction = updateShutterState;
		}
		else if (pvStruct.pvRecord == ShutterRecords::Cmi)
		{
			messenger.printDebugMessage(pvStruct.pvRecord, " update function = updateShutterCmi");
			pvStruct.updateFunction = updateShutterCmi;
		}
		else
		{
			messenger.printDebugMessage("!! WARNING !! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
		}
	}
	else
	{

	}
}
void EPICSShutterInterface::updateShutterState(const struct event_handler_args args)
{
	Shutter* recastShutter = getHardwareFromArgs<Shutter>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastShutter->shutter_state.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
		case GlobalConstants::zero_int: 
			recastShutter->shutter_state.second = STATE::CLOSED;
			break;
		case GlobalConstants::one_int: 
			recastShutter->shutter_state.second = STATE::OPEN;
			break;
		default:
			recastShutter->shutter_state.second = STATE::ERR;
	}
	messenger.printDebugMessage("Shutter STATE FOR: " + recastShutter->getHardwareName() + ": "
		+ ENUM_TO_STRING(recastShutter->shutter_state.second));
}
void EPICSShutterInterface::updateShutterCmi(const struct event_handler_args args)
{
	Shutter* recastShutter = getHardwareFromArgs<Shutter>(args);
	updateTimeStampLongPair(args, recastShutter->cmi);

	std::string binary_raw = std::bitset<16>(recastShutter->cmi.second).to_string(); // MAGIC NUMBER but this has to be hardcoded 
	//GOTCHA WHAT ENDIAN IS THIS NUMBER ???!!!
	std::string binary(binary_raw.rbegin(), binary_raw.rend()); //GOTCHA WHAT ENDIAN IS THIS NUMBER ???!!!
	// std::reverse(binary); //GOTCHA WHAT ENDIAN IS THIS NUMBER ???!!!  no only in c++ 17 
	std::cout << "Printing CMI 8 bit values " << binary_raw << std::endl;
	std::cout << "Printing CMI 8 bit values " << binary << std::endl;

	size_t counter = 0;
	for(auto&& bit_name: recastShutter->interlock_bit_names)
	{
		std::string bit_string = binary.substr(counter, 1);
		int bit_int = std::stoi(bit_string);

		std::cout << "bit_name == " << bit_name << " bit_string = " << bit_string << " bit_int = " << bit_int << std::endl;
		if( bit_int == 0)
		{ 
			recastShutter->cmi_bit_map.at(bit_name) = STATE::BAD;
		}
		else if(bit_int == 1)
		{ 
			recastShutter->cmi_bit_map.at(bit_name) = STATE::GOOD;
		}
		else
		{
			recastShutter->cmi_bit_map.at(bit_name) = STATE::ERR;
		}
		counter += 1; 
		messenger.printDebugMessage(bit_name, " value = ", recastShutter->cmi_bit_map.at(bit_name));
	}
}
