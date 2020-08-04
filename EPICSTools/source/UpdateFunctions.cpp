#include <UpdateFunctions.h>
#include <Listener.h>


UpdateFunctionHolder::UpdateFunctionHolder()
{}

UpdateFunctionHolder::UpdateFunctionHolder(const UpdateFunctionHolder& copyUpdateFunctionHolder)
{}

UpdateFunctionPtr UpdateFunctionHolder::findUpdateFunction(pvStruct& pv)
{
	switch (ca_field_type(pv.CHID))
	{
	case(DBR_TIME_DOUBLE):
		return updateDoubleValue;
		break;
	case(DBR_DOUBLE):
		return updateDoubleValue;
		break;
	case(DBR_TIME_INT):
		return updateIntegerValue;
		break;
	case(DBR_INT):
		return updateIntegerValue;
		break;
	case(DBR_TIME_ENUM):
		return updateEnumValue;
		break;
	case(DBR_ENUM):
		return updateEnumValue;
		break;
	case(DBR_TIME_LONG):
		return updateLongValue;
		break;
	case(DBR_LONG):
		return updateLongValue;
		break;
	case(DBR_TIME_FLOAT):
		return updateFloatValue;
		break;
	case(DBR_FLOAT):
		return updateFloatValue;
		break;
	case(DBR_TIME_STRING):
		return updateStringValue;
		break;
	case(DBR_STRING):
		return updateStringValue;
		break;
	}
}

// COULD BE A PROBLEM THAT Listener cannot adapt its own value here..
// maybe moving these up
void UpdateFunctionHolder::updateDoubleValue(const struct event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, double> pairToUpdate = recastListener->epicsInterface->getTimeStampDoublePair(args);
	recastListener->setValue<double>(pairToUpdate.second);
	recastListener->currentBuffer.push_back(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<double>());
}

void UpdateFunctionHolder::updateIntegerValue(const struct event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, int> pairToUpdate = recastListener->epicsInterface->getTimeStampEnumPair(args);
	recastListener->setValue<int>(pairToUpdate.second);
	recastListener->currentBuffer.push_back(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<int>());
}

void UpdateFunctionHolder::updateEnumValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = recastListener->epicsInterface->getTimeStampEnumPair(args);
	recastListener->setValue<unsigned short>(pairToUpdate.second);
	recastListener->currentBuffer.push_back(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<unsigned short>());
}

void UpdateFunctionHolder::updateFloatValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, float> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampFloatPair(args, pairToUpdate);
	recastListener->setValue<float>(pairToUpdate.second);
	recastListener->currentBuffer.push_back(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<float>());
}

void UpdateFunctionHolder::updateLongValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, long> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampLongPair(args, pairToUpdate);
	recastListener->setValue<long>(pairToUpdate.second);
	recastListener->currentBuffer.push_back(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<long>());
}

void UpdateFunctionHolder::updateStringValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::string> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampStringPair(args, pairToUpdate);
	recastListener->setValue<std::string>(pairToUpdate.second);
	recastListener->currentBuffer.push_back(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<std::string>());
}
