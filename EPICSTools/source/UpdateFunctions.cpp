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
	}
}

// COULD BE A PROBLEM THAT Listener cannot adapt its own value here..
// maybe moving these up
void UpdateFunctionHolder::updateDoubleValue(const struct event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	std::pair<epicsTime, double> pairToUpdate = recastListener->epicsInterface->getTimeStampDoublePair(args);
	recastListener->setValue<double>(pairToUpdate.second);
	std::cout << "LISTENER VALUE: " << recastListener->getValue<double>() << std::endl;
}