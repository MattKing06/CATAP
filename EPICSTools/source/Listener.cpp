#include <Listener.h>


Listener::Listener()
{
}

Listener::Listener(std::string pvStr) 
	: pvToMonitor(pvStr),
	mode(STATE::UNKNOWN),
	currentValue(GlobalConstants::double_min),
	epicsInterface(boost::make_shared<EPICSInterface>()),
	updateFunctions(UpdateFunctionHolder()),
	callCount(0),
	messenger(LoggingSystem(false,true))
{
	setupChannels();
}

Listener::Listener(std::string pvStr, STATE mode)
	: pvToMonitor(pvStr),
	 mode(mode),
	 currentValue(GlobalConstants::double_min),
	epicsInterface(boost::make_shared<EPICSInterface>()),
	updateFunctions(UpdateFunctionHolder()),
	callCount(0)
{
	setupChannels();
}

Listener::Listener(const Listener& copyListener)
	: pvToMonitor(copyListener.pvToMonitor),
	mode(copyListener.mode),
	pv(copyListener.pv),
	currentValue(copyListener.currentValue),
	messenger(copyListener.messenger),
	epicsInterface(copyListener.epicsInterface),
	updateFunctions(copyListener.updateFunctions),
	callCount(copyListener.callCount)
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
	std::cout << pvToMonitor << " CHANNEL TYPE: " << pv.CHTYPE << std::endl;
	epicsInterface->retrieveCOUNT(pv);
	pv.MASK = DBE_VALUE;
	pv.updateFunction = updateFunctions.findUpdateFunction(pv);
	EPICSInterface::sendToEPICS();
}

boost::python::object Listener::getValue_Py()
{
	return pyValue;
}