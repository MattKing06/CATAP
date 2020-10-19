#include <EPICSShutterInterface.h>
#include <Shutter.h>
#include <ShutterPVRecords.h>
#include <bitset>

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
			pvStruct.updateFunction = updateShutterState;
		}
		if (pvStruct.pvRecord == ShutterRecords::Cmi)
		{
			pvStruct.updateFunction = updateShutterCmi;
		}
		else
		{
			//messenger.printDebugMessage("!! WARNING !! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
		}
	}
}
void EPICSShutterInterface::updateShutterState(const struct event_handler_args args)
{
	Shutter* recastShutter = getHardwareFromArgs<Shutter>(args);
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampShortPair(args);
	recastShutter->shutterState.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
		case GlobalConstants::zero_int: 
			recastShutter->shutterState.second = STATE::CLOSED;
			break;
		case GlobalConstants::one_int: 
			recastShutter->shutterState.second = STATE::OPEN;
			break;
		default:
			recastShutter->shutterState.second = STATE::ERR;
	}
	//messenger.printDebugMessage("Shutter STATE FOR: " + recastShutter->getHardwareName() + ": "
	//	+ ENUM_TO_STRING(recastShutter->shutterState.second));
}
void EPICSShutterInterface::updateShutterCmi(const struct event_handler_args args)
{
	Shutter* recastShutter = getHardwareFromArgs<Shutter>(args);
	updateTimeStampIntPair(args, recastShutter->cmi);

	std::string binary = std::bitset<8>(recastShutter->cmi.second).to_string();
	std::cout << "Printing CMI bit values " << binary << std::endl;

	//size_t counter = 0;
	//std::cout << "Printign CMI bit values " << std::endl;
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



//void EPICSShutterInterface::debugMessagesOn()
//{
//	this->static_messenger.debugMessagesOn();
//	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG On");
//}
//void EPICSShutterInterface::debugMessagesOff()
//{
//	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG OFF");
//	this->static_messenger.debugMessagesOff();
//}
//void EPICSShutterInterface::messagesOn()
//{
//	this->static_messenger.messagesOn();
//	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES On");
//}
//void EPICSShutterInterface::messagesOff()
//{
//	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES OFF");
//	this->static_messenger.messagesOff();
//}
//bool EPICSShutterInterface::isMessagingOn()
//{
//	return this->static_messenger.isMessagingOn();
//}
//bool EPICSShutterInterface::isDebugOn()
//{
//	return this->static_messenger.isDebugOn();
//}