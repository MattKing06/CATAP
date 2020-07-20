#include <Getter.h>

Getter::Getter()
{

}

Getter::Getter(const std::string& pvStr) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	pvToGet(pvStr),
	mode(STATE::UNKNOWN)
{
	//fill in constructor to setup epicsInterface
	setupChannels();
	ca_get(pv.CHTYPE, pv.CHID, &value);
	EPICSInterface::sendToEPICS();
	pyValue = static_cast<boost::python::object>(value);
	std::cout << "GETTER VALUE: " << value << std::endl;
}

void Getter::setupChannels()
{	// create pvStruct
	// get CHID, CHTYPE, MASK, EVID, UpdateFunctionPtr, COUNT
	pv = pvStruct();
	pv.fullPVName = pvToGet;
	pv.monitor = false;
	epicsInterface->retrieveCHID(pv);
	EPICSInterface::sendToEPICS();
	epicsInterface->retrieveCHTYPE(pv);
	std::cout << "CH TYPE: " << pv.CHTYPE << std::endl;
	epicsInterface->retrieveCOUNT(pv);
	pv.MASK = DBE_VALUE;
	EPICSInterface::sendToEPICS();

}

Getter::Getter(const std::string& pvStr, const STATE& mode)
{
}

Getter::Getter(const Getter& getter)
{
}

boost::python::object Getter::getValue_Py()
{
	return pyValue;
}
