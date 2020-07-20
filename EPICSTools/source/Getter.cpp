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
	getPythonTypeFromEPICS();
}

void Getter::getPythonTypeFromEPICS()
{
	switch (pv.CHTYPE)
	{
	case(DBR_DOUBLE):
		double d_value;
		ca_get(pv.CHTYPE, pv.CHID, &d_value);
		EPICSInterface::sendToEPICS();
		pyValue = static_cast<boost::python::object>(d_value);
		std::cout << "DOUBLE TYPE" << std::endl;
		break;
	case(DBR_ENUM):
		unsigned short us_value;
		ca_get(pv.CHTYPE, pv.CHID, &us_value);
		EPICSInterface::sendToEPICS();
		pyValue = static_cast<boost::python::object>(us_value);
		std::cout << "ENUM TYPE" << std::endl;
		break;
	default:
		double def_value;
		ca_get(pv.CHTYPE, pv.CHID, &def_value);
		EPICSInterface::sendToEPICS();
		pyValue = static_cast<boost::python::object>(def_value);
		std::cout << "UNDEFINED TYPE" << std::endl;
		break;
	};
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
	getPythonTypeFromEPICS();
	return pyValue;
}
