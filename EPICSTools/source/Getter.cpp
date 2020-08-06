#include <Getter.h>
#include <PythonTypeConversions.h>

Getter::Getter()
{

}

Getter::Getter(const std::string& pvStr) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	pvToGet(pvStr),
	mode(STATE::UNKNOWN),
	currentValue(boost::variant<double, int, float, long, unsigned short, std::string>())
{
	//fill in constructor to setup epicsInterface
	std::cout << "creating getter" << std::endl;
	currentArray = std::vector<boost::variant<double, int, long, float, unsigned short, std::string>>();
	setupChannels();
	std::cout << "channels setup" << std::endl;
	setValueFromEPICS();
	std::cout << "setting EPICS value" << std::endl;
}
Getter::Getter(const Getter& copyGetter) :
	epicsInterface(copyGetter.epicsInterface),
	pvToGet(copyGetter.pvToGet),
	mode(copyGetter.mode),
	currentValue(copyGetter.currentValue),
	currentArray(copyGetter.currentArray)
{
}

void Getter::setValueFromEPICS()
{
	// needs to have a c++ counterpart for each DBR type.
	// also need to do array-types as well.
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
				double* d_array = new double [pv.COUNT];
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, d_array);
				EPICSInterface::sendToEPICS();
				for (int i = 0; i < pv.COUNT; i++)
				{
					currentArray.push_back(d_array[i]);
				}
				delete[] d_array;
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
				float* f_array = new float[pv.COUNT];
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, f_array);
				EPICSInterface::sendToEPICS();
				for (int i = 0; i < pv.COUNT; i++)
				{
					currentArray.push_back(f_array[i]);
				}
				delete[] f_array;
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
				int* i_array = new int[pv.COUNT];
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, i_array);
				EPICSInterface::sendToEPICS();
				for (int i = 0; i < pv.COUNT; i++)
				{
					currentArray.push_back(i_array[i]);
				}
				delete[] i_array;
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
				std::string* str_array = new std::string[pv.COUNT];
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, str_array);
				EPICSInterface::sendToEPICS();
				for (int i = 0; i < pv.COUNT; i++)
				{
					currentArray.push_back(str_array[i]);
				}
				delete[] str_array;
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
				unsigned short* us_array = new unsigned short[pv.COUNT];
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, us_array);
				EPICSInterface::sendToEPICS();
				for (int i = 0; i < pv.COUNT; i++)
				{
					currentArray.push_back(us_array[i]);
				}
				delete[] us_array;
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
				long* l_array = new long[pv.COUNT];
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, l_array);
				EPICSInterface::sendToEPICS();
				for (int i = 0; i < pv.COUNT; i++)
				{
					currentArray.push_back(l_array[i]);
				}
				delete[] l_array;
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
				double* d_array = new double[pv.COUNT];
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, d_array);
				EPICSInterface::sendToEPICS();
				for (int i = 0; i < pv.COUNT; i++)
				{
					currentArray.push_back(d_array[i]);
				}
				delete[] d_array;
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
	else if (isLong())
	{
		return static_cast<boost::python::object>(boost::get<long>(currentValue));
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
}
