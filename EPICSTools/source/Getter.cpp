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
		case(DBR_DOUBLE):
		{
			if (pv.COUNT == 1)
			{
				dbr_time_double d_value;
				ca_get(DBR_TIME_DOUBLE, pv.CHID, &d_value);
				EPICSInterface::sendToEPICS();
				currentValue.first = d_value.stamp;
				currentValue.second = static_cast<double>(d_value.value);
			}
			else
			{
				
				std::vector<double> d_array(pv.COUNT);
				dbr_time_double d_value;
				ca_array_get(DBR_DOUBLE, pv.COUNT, pv.CHID, &d_array[0]);
				ca_get(DBR_TIME_DOUBLE, pv.CHID, &d_value);
				EPICSInterface::sendToEPICS();
				currentArray.first = d_value.stamp;
				currentArray.second.clear();
				for (auto& item : d_array)
				{
					currentArray.second.push_back(item);
				}
				
				//std::cout << "ARRAY SIZE: " << d_array.size() << std::endl;

			}
			break;
		}
		case(DBR_SHORT):
		{
			if (pv.COUNT == 1)
			{
				dbr_time_short s_value;
				ca_get(DBR_TIME_SHORT, pv.CHID, &s_value);
				EPICSInterface::sendToEPICS();
				currentValue.second = static_cast<short>(s_value.value);
				currentValue.first = s_value.stamp;
			}
			else
			{
				std::vector<short> s_array(pv.COUNT);
				dbr_time_short s_value;
				ca_array_get(DBR_SHORT, pv.COUNT, pv.CHID, &s_array[0]);
				ca_get(DBR_TIME_SHORT, pv.CHID, &s_value);
				EPICSInterface::sendToEPICS();
				currentArray.first = s_value.stamp;
				currentArray.second.clear();
				for (auto& item : s_array)
				{
					currentArray.second.push_back(item);
				}
			}
			break;
		}
		case(DBR_FLOAT):
		{
			if (pv.COUNT == 1)
			{
				dbr_time_float f_value;
				ca_get(DBR_TIME_FLOAT, pv.CHID, &f_value);
				EPICSInterface::sendToEPICS();
				currentValue.second = static_cast<float>(f_value.value);
				currentValue.first = f_value.stamp;
			}
			else
			{
				std::vector<float> f_array(pv.COUNT);
				dbr_time_float f_value;
				ca_array_get(DBR_FLOAT, pv.COUNT, pv.CHID, &f_array[0]);
				ca_get(DBR_TIME_FLOAT, pv.CHID, &f_value);
				EPICSInterface::sendToEPICS();
				currentArray.first = f_value.stamp;
				currentArray.second.clear();
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
				dbr_time_string s_value;
				ca_get(DBR_TIME_STRING, pv.CHID, &s_value);
				EPICSInterface::sendToEPICS();
				currentValue.second = static_cast<std::string>(s_value.value);
				currentValue.first = s_value.stamp;
			}
			else
			{
				std::vector<std::string> st_array(pv.COUNT);
				dbr_time_float st_value;
				ca_array_get(DBR_FLOAT, pv.COUNT, pv.CHID, &st_array[0]);
				ca_get(DBR_TIME_FLOAT, pv.CHID, &st_value);
				EPICSInterface::sendToEPICS();
				currentArray.first = st_value.stamp;
				currentArray.second.clear();
				for (auto& item : st_array)
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
				dbr_time_enum us_value;
				ca_get(DBR_TIME_ENUM, pv.CHID, &us_value);
				EPICSInterface::sendToEPICS();
				currentValue.second = static_cast<unsigned short>(us_value.value);
				std::cout << "USHORT: " << boost::get<unsigned short>(currentValue.second) << std::endl;
				currentValue.first = us_value.stamp;
			}
			else
			{
				std::vector<unsigned short> us_array(pv.COUNT);
				dbr_time_enum us_value;
				ca_array_get(DBR_ENUM, pv.COUNT, pv.CHID, &us_array[0]);
				ca_get(DBR_TIME_ENUM, pv.CHID, &us_value);
				EPICSInterface::sendToEPICS();
				currentArray.first = us_value.stamp;
				currentArray.second.clear();
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
				dbr_time_long l_value;
				ca_get(DBR_TIME_LONG, pv.CHID, &l_value);
				EPICSInterface::sendToEPICS();
				currentValue.second = static_cast<long>(l_value.value);
				currentValue.first = l_value.stamp;
			}
			else
			{
				std::vector<long> l_array(pv.COUNT);
				dbr_time_enum l_value;
				ca_array_get(DBR_LONG, pv.COUNT, pv.CHID, &l_array[0]);
				ca_get(DBR_TIME_LONG, pv.CHID, &l_value);
				EPICSInterface::sendToEPICS();
				currentArray.first = l_value.stamp;
				currentArray.second.clear();
				for (auto& item : l_array)
				{
					currentArray.second.push_back(item);
				}
			}
			break;
		}
		default:
		{
			messenger.printMessage("Could not get EPICS value for ", pv.fullPVName, ".");
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
	return (currentValue.second.type() == typeid(short));
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

boost::python::dict Getter::getTimestampedArray_Py()
{
	if (isDoubleArray())
	{
		std::pair<epicsTimeStamp, std::vector<double>> t_vec = getTimestampedArray<double>();
		std::pair<std::string, std::vector<double>> d_vec;
		d_vec.first = epicsInterface->getEPICSTime(t_vec.first);
		d_vec.second = t_vec.second;
		return to_py_dict(d_vec);
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
	if (isShortArray())
	{
		std::pair<std::string, std::vector<short>> s_vec;
		s_vec.first = epicsInterface->getEPICSTime(currentArray.first);
		for (auto& item : currentArray.second)
		{
			s_vec.second.push_back(boost::get<short>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_dict(s_vec);
	}
	if (isEnumArray())
	{
		std::pair<std::string, std::vector<unsigned short>> us_vec;
		us_vec.first = epicsInterface->getEPICSTime(currentArray.first);
		for (auto& item : currentArray.second)
		{
			us_vec.second.push_back(boost::get<unsigned short>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_dict(us_vec);
	}
	else if (isStringArray())
	{
		std::pair<std::string, std::vector<std::string>> str_vec;
		str_vec.first = epicsInterface->getEPICSTime(currentArray.first);
		for (auto& item : currentArray.second)
		{
			str_vec.second.push_back(boost::get<std::string>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_dict(str_vec);
	}
	else if (isFloatArray())
	{
		std::pair<std::string, std::vector<float>> f_vec;
		f_vec.first = epicsInterface->getEPICSTime(currentArray.first);
		for (auto& item : currentArray.second)
		{
			f_vec.second.push_back(boost::get<float>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_dict(f_vec);
	}
	else if (isLongArray())
	{
		std::pair<std::string, std::vector<long>> l_vec;
		l_vec.first = epicsInterface->getEPICSTime(currentArray.first);
		for (auto& item : currentArray.second)
		{
			l_vec.second.push_back(boost::get<long>(item));
		}
		epicsInterface->retrieveCOUNT(pv); // reset count due to DJS hack for camera ROI dynamic COUNT requirements 
		return to_py_dict(l_vec);
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
