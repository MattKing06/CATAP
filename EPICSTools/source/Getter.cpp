#include <Getter.h>

Getter::Getter()
{

}

Getter::Getter(const std::string& pvStr) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	pvToGet(pvStr),
	mode(STATE::UNKNOWN),
	currentValue(boost::variant<double, int, float, unsigned short, std::string>())
{
	//fill in constructor to setup epicsInterface
	setupChannels();
	setValueFromEPICS();
}
Getter::Getter(const Getter& copyGetter) :
	epicsInterface(copyGetter.epicsInterface),
	pvToGet(copyGetter.pvToGet),
	mode(copyGetter.mode),
	currentValue(copyGetter.currentValue)
{
}

void Getter::setValueFromEPICS()
{
	// needs to have a c++ counterpart for each DBR type.
	// also need to do array-types as well.
	switch (pv.CHTYPE)
	{
		case(DBR_DOUBLE):
		{
			double d_value;
			ca_get(pv.CHTYPE, pv.CHID, &d_value);
			EPICSInterface::sendToEPICS();
			currentValue = d_value;
			break;
		}
		case(DBR_FLOAT):
		{
			float f_value;
			ca_get(pv.CHTYPE, pv.CHID, &f_value);
			EPICSInterface::sendToEPICS();
			currentValue = f_value;
			break;
		}
		case(DBR_INT):
		{
			int i_value;
			ca_get(pv.CHTYPE, pv.CHID, &i_value);
			EPICSInterface::sendToEPICS();
			currentValue = i_value;
			break;
		}
		case(DBR_STRING):
		{
			std::string s_value;
			ca_get(pv.CHTYPE, pv.CHID, &s_value);
			EPICSInterface::sendToEPICS();
			currentValue = s_value;
			break;
		}
		case(DBR_ENUM):
		{
			unsigned short us_value;
			ca_get(pv.CHTYPE, pv.CHID, &us_value);
			EPICSInterface::sendToEPICS();
			currentValue = us_value;
			break;
		}
		case(DBR_LONG):
		{
			long l_value;
			ca_get(pv.CHTYPE, pv.CHID, &l_value);
			EPICSInterface::sendToEPICS();
			currentValue = l_value;
			break;
		}
		default:
		{
			double def_value;
			ca_get(pv.CHTYPE, pv.CHID, &def_value);
			EPICSInterface::sendToEPICS();
			currentValue = def_value;
			break;
		}
	};
}

bool Getter::isLong()
{
	return (currentValue.type() == typeid(long));
}

bool Getter::isDouble()
{
	return (currentValue.type() == typeid(double));
}

bool Getter::isInt()
{
	return (currentValue.type() == typeid(int));
}

bool Getter::isEnum()
{
	return (currentValue.type() == typeid(unsigned short));
}

bool Getter::isString()
{
	return (currentValue.type() == typeid(std::string));
}

bool Getter::isFloat()
{
	return (currentValue.type() == typeid(float));
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
	std::cout << pvToGet << " CHANNEL TYPE: " << pv.CHTYPE << std::endl;
	// can use the channel count to decide whether to use ca_array_get or ca_get.
	epicsInterface->retrieveCOUNT(pv);
	pv.MASK = DBE_VALUE;
	EPICSInterface::sendToEPICS();

}

Getter::Getter(const std::string& pvStr, const STATE& mode)
{
}

boost::python::object Getter::getValue_Py()
{
	setValueFromEPICS();
	if (isDouble())
	{
		return static_cast<boost::python::object>(boost::get<double>(currentValue));
	}
	else if (isInt())
	{
		return static_cast<boost::python::object>(boost::get<int>(currentValue));
	}
	else if (isEnum())
	{
		return static_cast<boost::python::object>(boost::get<unsigned short>(currentValue));
	}
	else if (isString())
	{
		return static_cast<boost::python::object>(boost::get<std::string>(currentValue));
	}
	else if (isFloat())
	{
		return static_cast<boost::python::object>(boost::get<float>(currentValue));
	}
}
