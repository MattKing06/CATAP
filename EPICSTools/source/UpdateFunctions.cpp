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
	case(DBR_ENUM):
		return updateIntegerValue;
		break;
	case(DBR_TIME_ENUM):
		return updateIntegerValue;
		break;
	}
}

// COULD BE A PROBLEM THAT Listener cannot adapt its own value here..
// maybe moving these up
void UpdateFunctionHolder::updateDoubleValue(const struct event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTime, double> pairToUpdate = recastListener->epicsInterface->getTimeStampDoublePair(args);
	recastListener->setValue<double>(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<double>());
}

void UpdateFunctionHolder::updateIntegerValue(const struct event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTime, int> pairToUpdate = recastListener->epicsInterface->getTimeStampEnumPair(args);
	recastListener->setValue<int>(pairToUpdate.second);
	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<int>());
}