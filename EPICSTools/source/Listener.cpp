#include <Listener.h>


Listener::Listener()
{
}

Listener::Listener(std::string pvStr) 
	: pvToMonitor(pvStr),
	mode(STATE::UNKNOWN),
	currentValue(boost::variant<double, float, long, int, unsigned short, std::string>()),
	currentBuffer(boost::circular_buffer<boost::variant<double, float, long, int, unsigned short, std::string> >(10)),
	currentArray(std::vector<boost::variant<double, float, long, int, unsigned short, std::string>>()),
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
	 currentValue(boost::variant<double, float, long, int, unsigned short, std::string>()),
	currentBuffer(boost::circular_buffer<boost::variant<double, float, long, int, unsigned short, std::string> >(10)),
	currentArray(std::vector<boost::variant<double, float, long, int, unsigned short, std::string>>()),
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
	currentArray(copyListener.currentArray),
	currentBuffer(copyListener.currentBuffer),
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
	std::cout << pvToMonitor << " COUNT: " << pv.COUNT << std::endl;
	pv.MASK = DBE_VALUE;
	pv.updateFunction = updateFunctions.findUpdateFunction(pv);
	EPICSInterface::sendToEPICS();
}

void Listener::stopListening()
{
	if (pv.monitor)
	{
		if (pv.EVID)
		{
			epicsInterface->removeSubscription(pv);
			ca_flush_io();
			pv.monitor = false;
		}
	}
	if (pv.CHID)
	{
		epicsInterface->removeChannel(pv);
		EPICSInterface::sendToEPICS();
	}

}

void Listener::startListening()
{
	if (!pv.monitor)
	{
		std::cout << "setting up channels" << std::endl;
		setupChannels();
	}
	else
	{
		std::cout << "already monitoring " << pv << std::endl;
	}
}

boost::python::list Listener::getArray_Py()
{
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

void Listener::setBufferSize(int size)
{
	currentBuffer.resize(size);
	messenger.printMessage("size of buffer is now: ", currentBuffer.capacity());
}

boost::python::object Listener::getValue_Py()
{
	if (isDouble())
	{
		return static_cast<boost::python::object>(boost::get<double>(currentValue));
	}
	else if (isLong())
	{
		return static_cast<boost::python::object>(boost::get<long>(currentValue));
	}
	else if (isInt())
	{
		return static_cast<boost::python::object>(boost::get<int>(currentValue));

	}
	else if (isEnum())
	{
		return static_cast<boost::python::object>(boost::get<unsigned short>(currentValue));
	}
	else if (isFloat())
	{
		return static_cast<boost::python::object>(boost::get<float>(currentValue));
	}
	else if (isString())
	{
		return static_cast<boost::python::object>(boost::get<std::string>(currentValue));
	}
}

boost::python::list Listener::getBuffer_Py()
{
	if (isDoubleBuffer())
	{
		boost::circular_buffer<double> dblBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			dblBuff.push_back(boost::get<double>(item));
		}
		return to_py_list(dblBuff);
	}
	else if (isIntBuffer())
	{
		boost::circular_buffer<int> intBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			intBuff.push_back(boost::get<int>(item));
		}
		return to_py_list(intBuff);
	}
	else if (isEnumBuffer())
	{
		boost::circular_buffer<unsigned short> usBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			usBuff.push_back(boost::get<unsigned short>(item));
		}
		return to_py_list(usBuff);
	}
	else if (isStringBuffer())
	{
		boost::circular_buffer<std::string> strBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			strBuff.push_back(boost::get<std::string>(item));
		}
		return to_py_list(strBuff);
	}
	else if (isFloatBuffer())
	{
		boost::circular_buffer<float> fltBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			fltBuff.push_back(boost::get<float>(item));
		}
		return to_py_list(fltBuff);
	}
}

double Listener::getBufferAverage()
{
	if (!isStringBuffer() && !isEnumBuffer())
	{
		double total = 0.0;
		for (auto& item : currentBuffer)
		{
			total += boost::get<double>(item);
		}
		return (total / currentBuffer.size());
	}
	else
	{
		messenger.printMessage("Cannot perform average on String or Enum types.");
	}

}

bool Listener::isLong()
{
	return (currentValue.type() == typeid(long));
}

bool Listener::isDouble()
{
	return (currentValue.type() == typeid(double));
}

bool Listener::isInt()
{
	return (currentValue.type() == typeid(int));
}

bool Listener::isEnum()
{
	return (currentValue.type() == typeid(unsigned short));
}

bool Listener::isString()
{
	return (currentValue.type() == typeid(std::string));
}

bool Listener::isFloat()
{
	return (currentValue.type() == typeid(float));
}

bool Listener::isLongArray()
{
	return (currentArray.at(0).type() == typeid(long));
}

bool Listener::isDoubleArray()
{
	return (currentArray.at(0).type() == typeid(double));
}

bool Listener::isIntArray()
{
	return (currentArray.at(0).type() == typeid(int));
}

bool Listener::isFloatArray()
{
	return (currentArray.at(0).type() == typeid(float));
}

bool Listener::isEnumArray()
{
	return (currentArray.at(0).type() == typeid(unsigned short));
}

bool Listener::isStringArray()
{
	return (currentArray.at(0).type() == typeid(std::string));
}


bool Listener::isDoubleBuffer()
{
	return (currentBuffer.at(0).type() == typeid(double));
}

bool Listener::isIntBuffer()
{
	return (currentBuffer.at(0).type() == typeid(int));
}

bool Listener::isEnumBuffer()
{
	return (currentBuffer.at(0).type() == typeid(unsigned short));
}

bool Listener::isStringBuffer()
{
	return (currentBuffer.at(0).type() == typeid(std::string));
}

bool Listener::isFloatBuffer()
{
	return (currentBuffer.at(0).type() == typeid(float));
}

bool Listener::isLongBuffer()
{
	return(currentBuffer.at(0).type() == typeid(long));
}
