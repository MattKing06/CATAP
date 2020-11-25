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
	updateTimeStampIntPair(args, recastShutter->cmi);

	std::string binary_raw = std::bitset<8>(recastShutter->cmi.second).to_string(); // MAGIC NUMBER but thi shas to be hardcoded 
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
		counter += 1; // meh 
		messenger.printDebugMessage(bit_name, " value = ", recastShutter->cmi_bit_map.at(bit_name));
	}

	//recastShutter->interlock_bit_names

	//size_t counter = 0;
	//std::cout << "Printing CMI bit values " << std::endl;
	//for (auto bit = 0; bit < 8; ++bit)
	//{
	//	(obj.cmi & (1 << bit)) >> bit
	//}

	//for (auto bit = 0; bit < 8; ++bit)//MAGIC_NUMBER
	//{
	//	//message(obj.name, " bit ", bit, " value = ",  (obj.cmi &( 1 << bit)) >> bit);
	//	if (std::find(obj.protKeyBits.begin(),
	//		obj.protKeyBits.end(), bit) != obj.protKeyBits.end())
	//	{
	//		obj.protKeyBitValues[counter] = (obj.cmi & (1 << bit)) >> bit;
	//		//message("obj.protKeyBitValues part ",
	//		//        counter, " = ",
	//		//        obj.protKeyBitValues[counter]);
	//		++counter;
	//	}
	//}


}
