#include <UpdateFunctions.h>
#include <Listener.h>

UpdateFunctionHolder::UpdateFunctionHolder()
{}

UpdateFunctionHolder::UpdateFunctionHolder(const UpdateFunctionHolder& copyUpdateFunctionHolder)
{}


// COULD BE A PROBLEM THAT Listener cannot adapt its own value here..
// maybe moving these up
void UpdateFunctionHolder::updateDoubleValue(const struct event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	std::pair<epicsTime, double> pairToUpdate = recastListener->epicsInterface->getTimeStampDoublePair(args);
	recastListener->setValue(pairToUpdate.second);
	std::cout << "LISTENER VALUE: " << recastListener->getValue() << std::endl;
}