#include <EPICSToolsInterface.h>
#include <boost/any.hpp>

EPICSToolsInterface::EPICSToolsInterface() : EPICSInterface()
{
}

EPICSToolsInterface::~EPICSToolsInterface()
{
}

void EPICSToolsInterface::updateValue(const event_handler_args args)
{
	EPICSTools* recastEPICSTool = getHardwareFromArgs<EPICSTools>(args);
	boost::any value(args.dbr);
	recastEPICSTool->setValue(value);
}

void EPICSToolsInterface::retrieveUpdateFunction(pvStruct& pv)
{
	pv.updateFunction = this->updateValue;
}
