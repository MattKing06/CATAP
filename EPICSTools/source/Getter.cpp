#include <Getter.h>
#include <PythonTypeConversions.h>

Getter::Getter()
{

}

Getter::Getter(const std::string& pvStr) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	mode(STATE::VIRTUAL),
	currentValue(timeStampValue())
{
	pvToGet = getEPICSPVName(pvStr);
	//fill in constructor to setup epicsInterface
	currentArray = timeStampVector();
	setupChannels();
	setValueFromEPICS();
}

Getter::Getter(const std::string& pvStr, const STATE& mode) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	mode(mode),
	currentValue(timeStampValue())
{
	pvToGet = getEPICSPVName(pvStr);
	currentArray = timeStampVector();
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
		currentArray.second.clear();
		case(DBR_DOUBLE):
		{
			if (pv.COUNT == 1)
			{
				dbr_time_double d_value;
				ca_get(DBR_TIME_DOUBLE, pv.CHID, &d_value);
				EPICSInterface::sendToEPICS();
				currentValue.first = d_value.stamp;
				currentValue.second = d_value.value;
			}
			else
			{
				std::vector<double> d_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &d_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << d_array.size() << std::endl;
				for (auto& item : d_array)
				{
					currentArray.second.push_back(item);
				}
			}
			break;
		}
		case(DBR_SHORT):
		{
			if (pv.COUNT == 1)
			{
				short s_value;
				ca_get(pv.CHTYPE, pv.CHID, &s_value);
				EPICSInterface::sendToEPICS();
				currentValue.second = s_value;
			}
			else
			{
				std::vector<short> s_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &s_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << s_array.size() << std::endl;
				for (auto& item : s_array)
					currentArray.second.push_back(item);
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
				currentValue.second = f_value;
			}
			else
			{
				std::vector<float> f_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &f_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << f_array.size() << std::endl;
				for (auto& item : f_array)
				{
					currentArray.second.push_back(item);
				}
			}
			break;
		}
		//case(DBR_INT):
		//{
		//	if (pv.COUNT == 1)
		//	{
		//		int i_value;
		//		ca_get(pv.CHTYPE, pv.CHID, &i_value);
		//		EPICSInterface::sendToEPICS();
		//		currentValue = i_value;
		//	}
		//	else
		//	{
		//		std::vector<int> i_array(pv.COUNT);
		//		ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &i_array[0]);
		//		EPICSInterface::sendToEPICS();
		//		std::cout << "ARRAY SIZE: " << i_array.size() << std::endl;
		//		for (auto& item : i_array)
		//		{
		//			currentArray.push_back(item);
		//		}
		//	}
		//	break;
		//}
		case(DBR_STRING):
		{
			if (pv.COUNT == 1)
			{
				std::string s_value;
				ca_get(pv.CHTYPE, pv.CHID, &s_value);
				EPICSInterface::sendToEPICS();
				currentValue.second = s_value;
			}
			else
			{
				std::vector<std::string> s_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &s_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << s_array.size() << std::endl;
				for (auto& item : s_array)
				{
					currentArray.second.push_back(item);
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
				currentValue.second = us_value;
			}
			else
			{
				//unsigned short* us_array = new unsigned short[pv.COUNT];
				std::vector<unsigned short> us_array;
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &us_array[0]);
				EPICSInterface::sendToEPICS();
				for (auto& item : us_array)
				{
					currentArray.second.push_back(item);
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
				currentValue.second = l_value;
			}
			else
			{
				std::vector<long> l_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &l_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << l_array.size() << std::endl;
				for (auto& item : l_array)
				{
					currentArray.second.push_back(item);
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
				currentValue.second = d_value;
			}
			else
			{
				std::vector<double> d_array(pv.COUNT);
				ca_array_get(pv.CHTYPE, pv.COUNT, pv.CHID, &d_array[0]);
				EPICSInterface::sendToEPICS();
				std::cout << "ARRAY SIZE: " << d_array.size() << std::endl;
				for (auto& item : d_array)
				{
					currentArray.second.push_back(item);
				}
			}
			break;
		}
	};
}

bool Getter::isLong()
{
	return (currentValue.second.type() == typeid(long));
}

bool Getter::isDouble()
{
	return (currentValue.second.type() == typeid(double));
}

bool Getter::isInt()
{
	return (currentValue.second.type() == typeid(int));
}

bool Getter::isEnum()
{
	return (currentValue.second.type() == typeid(unsigned short));
}

bool Getter::isShort()
{
	return (currentValue.second.type() == typeid(unsigned short));
}

bool Getter::isString()
{
	return (currentValue.second.type() == typeid(std::string));
}

bool Getter::isFloat()
{
	return (currentValue.second.type() == typeid(float));
}

bool Getter::isShortArray()
{
	return (currentArray.second.at(0).type() == typeid(short));
}

bool Getter::isLongArray()
{
	return (currentArray.second.at(0).type() == typeid(long));
}

bool Getter::isDoubleArray()
{
	return (currentArray.second.at(0).type() == typeid(double));
}

bool Getter::isIntArray()
{
	return (currentArray.second.at(0).type() == typeid(int));
}

bool Getter::isFloatArray()
{
	return (currentArray.second.at(0).type() == typeid(float));
}

bool Getter::isEnumArray()
{
	return (currentArray.second.at(0).type() == typeid(unsigned short));
}

bool Getter::isStringArray()
{
	return (currentArray.second.at(0).type() == typeid(std::string));
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
		return static_cast<boost::python::object>(boost::get<double>(currentValue.second));
	}
	//else if (isInt())
	//{
	//	return static_cast<boost::python::object>(boost::get<int>(currentValue));
	//}
	else if (isShort())
	{
		return static_cast<boost::python::object>(boost::get<short>(currentValue.second));
	}
	else if (isEnum())
	{
		return static_cast<boost::python::object>(boost::get<unsigned short>(currentValue.second));
	}
	else if (isString())
	{
		return static_cast<boost::python::object>(boost::get<std::string>(currentValue.second));
	}
	else if (isFloat())
	{
		return static_cast<boost::python::object>(boost::get<float>(currentValue.second));
	}
	else if (isLong())
	{
		return static_cast<boost::python::object>(boost::get<long>(currentValue.second));
	}
	else
	{
		messenger.printMessage("Could not convert EPICS PV type into PyObject. Please contact support.");
		return boost::python::object();
	}
}

boost::python::dict Getter::getTimestampedValue_Py()
{
	setValueFromEPICS();
	boost::python::dict r;
	if (isDouble())
	{
		r["value"] = static_cast<boost::python::object>(boost::get<double>(currentValue.second));
		r["time"] = epicsInterface->getEPICSTime(currentValue.first);
		return r;
	}
	//else if (isInt())
	//{
	//	return static_cast<boost::python::object>(boost::get<int>(currentValue));
	//}
	else if (isShort())
	{
		r["value"] = static_cast<boost::python::object>(boost::get<short>(currentValue.second));
		r["time"] = epicsInterface->getEPICSTime(currentValue.first);
		return r;
	}
	else if (isEnum())
	{
		r["value"] = static_cast<boost::python::object>(boost::get<unsigned short>(currentValue.second));
		r["time"] = epicsInterface->getEPICSTime(currentValue.first);
		return r;
	}
	else if (isString())
	{
		r["value"] = static_cast<boost::python::object>(boost::get<std::string>(currentValue.second));
		r["time"] = epicsInterface->getEPICSTime(currentValue.first);
		return r;
	}
	else if (isFloat())
	{
		r["value"] = static_cast<boost::python::object>(boost::get<float>(currentValue.second));
		r["time"] = epicsInterface->getEPICSTime(currentValue.first);
		return r;
	}
	else if (isLong())
	{
		r["value"] = static_cast<boost::python::object>(boost::get<long>(currentValue.second));
		r["time"] = epicsInterface->getEPICSTime(currentValue.first);
		return r;
	}
	else
	{
		messenger.printMessage("Could not convert EPICS PV type into PyObject. Please contact support.");
		return boost::python::dict();
	}
}

boost::python::list Getter::getArray_Py()
{
	currentArray.second.clear();
	setValueFromEPICS();
	if (isDoubleArray())
	{
		std::vector<double> d_vec;
		for (auto& item : currentArray.second)
		{
			d_vec.push_back(boost::get<double>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_list(d_vec);
	}
	//else if (isIntArray())
	//{
	//	std::vector<int> i_vec;
	//	for (auto& item : currentArray)
	//	{
	//		i_vec.push_back(boost::get<int>(item));
	//	}
	//	epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
	//	return to_py_list(i_vec);
	//}
	else if (isShortArray())
	{
		std::vector<short> s_vec;
		for (auto& item : currentArray.second)
		{
			s_vec.push_back(boost::get<short>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_list(s_vec);
	}
	else if (isEnumArray())
	{
		std::vector<unsigned short> us_vec;
		for (auto& item : currentArray.second)
		{
			us_vec.push_back(boost::get<unsigned short>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_list(us_vec);
	}
	else if (isStringArray())
	{
		std::vector<std::string> str_vec;
		for (auto& item : currentArray.second)
		{
			str_vec.push_back(boost::get<std::string>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_list(str_vec);
	}
	else if (isFloatArray())
	{
		std::vector<float> f_vec;
		for (auto& item : currentArray.second)
		{
			f_vec.push_back(boost::get<float>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_list(f_vec);
	}
	else if (isLongArray())
	{
		std::vector<long> l_vec;
		for (auto& item : currentArray.second)
		{
			l_vec.push_back(boost::get<long>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_list(l_vec);
	}
	else
	{
		messenger.printMessage("Could not convert EPICS PV type into PyObject. Please contact support.");
		return boost::python::list();
	}
}
