#include <Listener.h>


Listener::Listener()
{
}

Listener::Listener(std::string pvStr) 
	: pvToMonitor(pvStr),
	mode(STATE::UNKNOWN),
	value(GlobalConstants::double_min),
	epicsInterface(boost::make_shared<EPICSInterface>()),
	updateFunctions(UpdateFunctionHolder())
{
	setupChannels();
}

Listener::Listener(std::string pvStr, STATE mode)
	: pvToMonitor(pvStr),
	 mode(mode),
	 value(GlobalConstants::double_min),
	epicsInterface(boost::make_shared<EPICSInterface>()),
	updateFunctions(UpdateFunctionHolder())
{
	setupChannels();
}

Listener::Listener(const Listener& copyListener)
	: pvToMonitor(copyListener.pvToMonitor),
	mode(copyListener.mode),
	pv(copyListener.pv),
	value(copyListener.value),
	messenger(copyListener.messenger),
	epicsInterface(copyListener.epicsInterface),
	updateFunctions(copyListener.updateFunctions)
{
}

void Listener::setupChannels()
{
	// create pvStruct
	// get CHID, CHTYPE, MASK, EVID, UpdateFunctionPtr, COUNT
	pv = pvStruct();
	pv.fullPVName = pvToMonitor;
	pv.monitor = true;
	epicsInterface->retrieveCHID(pv);
	EPICSInterface::sendToEPICS();
	epicsInterface->retrieveCHTYPE(pv);
	std::cout << "MON TYPE: " << pv.monitorCHTYPE  << "  CH TYPE: " << pv.CHTYPE << std::endl;
	epicsInterface->retrieveCOUNT(pv);
	pv.MASK = DBE_VALUE;
	pv.updateFunction = updateFunctions.updateDoubleValue;
	EPICSInterface::sendToEPICS();
}

void Listener::setValue(double newValue)
{
	std::cout << "NEW VALUE TO SET: " << newValue << std::endl;
	value = newValue;
}

double Listener::getValue()
{
	return value;
}
