#include <Listener.h>


Listener::Listener()
{
}

Listener::Listener(std::string pvStr) 
	: pvToMonitor(pvStr),
	mode(STATE::UNKNOWN),
	currentValue(GlobalConstants::double_min),
	currentBuffer(boost::circular_buffer<boost::variant<double, float, long, int, unsigned short, std::string> >(10)),
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
	currentBuffer(boost::circular_buffer<boost::variant<double, float, long, int, unsigned short, std::string> >(10)),
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

boost::python::object Listener::getValue_Py()
{
	return pyValue;
}

void Listener::setBufferSize(int size)
{
	currentBuffer.resize(size);
	messenger.printMessage("size of buffer is now: ", currentBuffer.capacity());
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
		double total;
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
