#include <Getter.h>
#include <PythonTypeConversions.h>

Getter::Getter()
{

}

Getter::Getter(const std::string& pvStr) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	mode(STATE::VIRTUAL),
	currentValue(boost::variant<double, int, float, long, unsigned short, std::string>())
{
	pvToGet = getEPICSPVName(pvStr);
	//fill in constructor to setup epicsInterface
	currentArray = std::vector<boost::variant<double, int, long, float, unsigned short, std::string>>();
	setupChannels();
	setValueFromEPICS();
}

Getter::Getter(const std::string& pvStr, const STATE& mode) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	mode(mode),
	currentValue(boost::variant<double, int, float, long, unsigned short, std::string>())
{
	pvToGet = getEPICSPVName(pvStr);
	currentArray = std::vector<boost::variant<double, int, long, float, unsigned short, std::string>>();
	setupChannels();
	setValueFromEPICS();
}
Getter::Getter(const Getter& copyGetter) :
	epicsInterface(copyGetter.epicsInterface),
	pvToGet(copyGetter.pvToGet),
	mode(copyGetter.mode),
	currentValue(copyGetter.currentValue),
	currentArray(copyGetter.currentArray)
{
}

std::string Getter::getEPICSPVName(const std::string& pv)
{
	if (mode == STATE::VIRTUAL)
	{
		if (pv.find("VM-") != std::string::npos)
		{
			return pv;
		}
		else
		{
			std::string virtualName = "VM-" + pv;
			return virtualName;
		}
	}
	else
	{
		return pv;
	}
}

void Getter::setValueFromEPICS()
{
	switch (pv.CHTYPE)
	{
		currentArray.clear();
		case(DBR_DOUBLE):
		{
			if (pv.COUNT == 1)
			{
				double d_value;
				ca_get(pv.CHTYPE, pv.CHID, &d_value);
				EPICSInterface::sendToEPICS();
				currentValue = d_value;
			}
			else
			{
				
				std::vector<double> d_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &d_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << d_array.size() << std::endl;
				for (auto& item : d_array)
				{
					currentArray.push_back(item);
				}
			}
			break;
		}
		case(DBR_FLOAT):
		{
			if (pv.COUNT == 1)
			{
				float f_value;
				ca_get(pv.CHTYPE, pv.CHID, &f_value);
				EPICSInterface::sendToEPICS();
				currentValue = f_value;
			}
			else
			{
				std::vector<float> f_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &f_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << f_array.size() << std::endl;
				for (auto& item : f_array)
				{
					currentArray.push_back(item);
				}
			}
			break;
		}
		case(DBR_INT):
		{
			if (pv.COUNT == 1)
			{
				int i_value;
				ca_get(pv.CHTYPE, pv.CHID, &i_value);
				EPICSInterface::sendToEPICS();
				currentValue = i_value;
			}
			else
			{
				std::vector<int> i_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &i_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << i_array.size() << std::endl;
				for (auto& item : i_array)
				{
					currentArray.push_back(item);
				}
			}
			break;
		}
		case(DBR_STRING):
		{
			if (pv.COUNT == 1)
			{
				std::string s_value;
				ca_get(pv.CHTYPE, pv.CHID, &s_value);
				EPICSInterface::sendToEPICS();
				currentValue = s_value;
			}
			else
			{
				std::vector<std::string> s_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &s_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << s_array.size() << std::endl;
				for (auto& item : s_array)
				{
					currentArray.push_back(item);
				}
			}
			break;
		}
		case(DBR_ENUM):
		{
			if (pv.COUNT == 1)
			{
				unsigned short us_value;
				ca_get(pv.CHTYPE, pv.CHID, &us_value);
				EPICSInterface::sendToEPICS();
				currentValue = us_value;
			}
			else
			{
				//unsigned short* us_array = new unsigned short[pv.COUNT];
				std::vector<unsigned short> us_array;
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &us_array[0]);
				EPICSInterface::sendToEPICS();
				for (auto& item : us_array)
				{
					currentArray.push_back(item);
				}
			}
			break;
		}
		case(DBR_LONG):
		{
			if (pv.COUNT == 1)
			{
				long l_value;
				ca_get(pv.CHTYPE, pv.CHID, &l_value);
				EPICSInterface::sendToEPICS();
				currentValue = l_value;
			}
			else
			{
				std::vector<long> l_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &l_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << l_array.size() << std::endl;
				for (auto& item : l_array)
				{
					currentArray.push_back(item);
				}
			}
			break;
		}
		default:
		{
			if (pv.COUNT == 1)
			{
				double d_value;
				ca_get(pv.CHTYPE, pv.CHID, &d_value);
				EPICSInterface::sendToEPICS();
				currentValue = d_value;
			}
			else
			{
				std::vector<double> d_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &d_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << d_array.size() << std::endl;
				for (auto& item : d_array)
				{
					currentArray.push_back(item);
				}
			}
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

bool Getter::isLongArray()
{
	return (currentArray.at(0).type() == typeid(long));
}

bool Getter::isDoubleArray()
{
	return (currentArray.at(0).type() == typeid(double));
}

bool Getter::isIntArray()
{
	return (currentArray.at(0).type() == typeid(int));
}

bool Getter::isFloatArray()
{
	return (currentArray.at(0).type() == typeid(float));
}

bool Getter::isEnumArray()
{
	return (currentArray.at(0).type() == typeid(unsigned short));
}

bool Getter::isStringArray()
{
	return (currentArray.at(0).type() == typeid(std::string));
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
	// can use the channel count to decide whether to use ca_array_get or ca_get.
	epicsInterface->retrieveCOUNT(pv);
	pv.MASK = DBE_VALUE;
	EPICSInterface::sendToEPICS();

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
	else if (isLong())
	{
		return static_cast<boost::python::object>(boost::get<long>(currentValue));
	}
	else
	{
		messenger.printMessage("Could not convert EPICS PV type into PyObject. Please contact support.");
		return boost::python::object();
	}
}

boost::python::list Getter::getArray_Py()
{
	currentArray.clear();
	setValueFromEPICS();
	if (isDoubleArray())
	{
		std::vector<double> d_vec;
		for (auto& item : currentArray)
		{
			d_vec.push_back(boost::get<double>(item));
		}
		return to_py_list(d_vec);
	}
	else if (isIntArray())
	{
		std::vector<int> i_vec;
		for (auto& item : currentArray)
		{
			i_vec.push_back(boost::get<int>(item));
		}
		return to_py_list(i_vec);
	}
	else if (isEnumArray())
	{
		std::vector<unsigned short> us_vec;
		for (auto& item : currentArray)
		{
			us_vec.push_back(boost::get<unsigned short>(item));
		}
		return to_py_list(us_vec);
	}
	else if (isStringArray())
	{
		std::vector<std::string> str_vec;
		for (auto& item : currentArray)
		{
			str_vec.push_back(boost::get<std::string>(item));
		}
		return to_py_list(str_vec);
	}
	else if (isFloatArray())
	{
		std::vector<float> f_vec;
		for (auto& item : currentArray)
		{
			f_vec.push_back(boost::get<float>(item));
		}
		return to_py_list(f_vec);
	}
	else if (isLongArray())
	{
		std::vector<long> l_vec;
		for (auto& item : currentArray)
		{
			l_vec.push_back(boost::get<long>(item));
		}
		return to_py_list(l_vec);
	}
	else
	{
		messenger.printMessage("Could not convert EPICS PV type into PyObject. Please contact support.");
		return boost::python::list();
	}
}
