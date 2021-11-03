#include <Listener.h>
#include <boost/foreach.hpp>

Listener::Listener()
{
}

Listener::Listener(const std::string& pvStr) 
	:
	mode(STATE::VIRTUAL),
	currentValue(timeStampValue()),
	currentBuffer(boost::circular_buffer<timeStampValue>(DEFAULT_BUFFER_SIZE)),
	currentArray(timeStampVector()),
	currentArrayBuffer(boost::circular_buffer<timeStampVector>(DEFAULT_BUFFER_SIZE)),
	epicsInterface(boost::make_shared<EPICSInterface>()),
	updateFunctions(UpdateFunctionHolder()),
	callCount(0),
	messenger(LoggingSystem(false,true))
{
	pvToMonitor = getEPICSPVName(pvStr);
	currentArrayBuffer.set_capacity(DEFAULT_BUFFER_SIZE);
	currentBuffer.set_capacity(DEFAULT_BUFFER_SIZE);
	setupChannels();
}

Listener::Listener(const std::string& pvStr, const STATE& mode)
	: mode(mode),
	// currentValue(boost::variant<double, float, long, int, unsigned short, std::string>()),
	currentBuffer(boost::circular_buffer<timeStampValue>(DEFAULT_BUFFER_SIZE)),
	currentArray(timeStampVector()),
	currentArrayBuffer(boost::circular_buffer<timeStampVector>(DEFAULT_BUFFER_SIZE)),
	epicsInterface(boost::make_shared<EPICSInterface>()),
	updateFunctions(UpdateFunctionHolder()),
	callCount(0)
{
	pvToMonitor = getEPICSPVName(pvStr);
	currentArrayBuffer.set_capacity(DEFAULT_BUFFER_SIZE);
	currentBuffer.set_capacity(DEFAULT_BUFFER_SIZE);
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
	std::cout << " COPYING LISTENER FOR " << pvToMonitor << std::endl;
	currentArray.second.assign(copyListener.currentArray.second.begin(), copyListener.currentArray.second.end());
	currentBuffer.assign(copyListener.currentBuffer.begin(), copyListener.currentBuffer.end());
	currentArrayBuffer.assign(copyListener.currentArrayBuffer.begin(), copyListener.currentArrayBuffer.end());
}

Listener::~Listener()
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
	epicsInterface->sendToEPICSm2("creating channel");
	epicsInterface->retrieveCHTYPE(pv);
	epicsInterface->retrieveCOUNT(pv);
	if (pv.COUNT == 1) { initialiseCurrentValue(pv); }
	pv.MASK = DBE_VALUE;
	pv.updateFunction = updateFunctions.findUpdateFunction(pv);
	EPICSInterface::sendToEPICSm2("setting up complete.");
}

std::string Listener::getEPICSPVName(const std::string& pv)
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

int Listener::getBufferCapacity()
{
	return currentBuffer.capacity();
}

int Listener::getBufferSize()
{
	return currentBuffer.size();
}

int Listener::getArrayBufferCapacity()
{
	return currentArrayBuffer.capacity();
}

int Listener::getArrayBufferSize()
{
	return currentArrayBuffer.size();
}

void Listener::initialiseCurrentArray(const pvStruct& pv)
{
	switch (ca_field_type(pv.CHID))
	{
	case(DBR_TIME_DOUBLE):
	{
		std::vector<double> epicsDBLTimeArray(pv.COUNT);
		ca_array_get(DBR_TIME_DOUBLE, pv.COUNT, pv.CHID, &epicsDBLTimeArray[0]);
		for (auto& val : epicsDBLTimeArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_DOUBLE):
	{
		std::vector<double> epicsDBLArray(pv.COUNT);
		ca_array_get(DBR_DOUBLE, pv.COUNT, pv.CHID, &epicsDBLArray[0]);
		for (auto& val : epicsDBLArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_TIME_SHORT):
	{
		std::vector<short> epicsSHORTTimeArray(pv.COUNT);
		ca_array_get(DBR_TIME_SHORT, pv.COUNT, pv.CHID, &epicsSHORTTimeArray[0]);
		for (auto& val : epicsSHORTTimeArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_SHORT):
	{
		std::vector<short> epicsSHORTArray(pv.COUNT);
		ca_array_get(DBR_SHORT, pv.COUNT, pv.CHID, &epicsSHORTArray[0]);
		for (auto& val : epicsSHORTArray)
			currentArray.second.push_back(val);
		break;
	}
	//case(DBR_TIME_INT):
	//{
	//	std::vector<int> epicsINTTimeArray(pv.COUNT);
	//	ca_array_get(DBR_TIME_INT, pv.COUNT, pv.CHID, &epicsINTTimeArray[0]);
	//	for (auto& val : epicsINTTimeArray)
	//		currentArray.push_back(val);
	//	break;
	//}
	//case(DBR_INT):
	//{
	//	std::vector<int> epicsINTArray(pv.COUNT);
	//	ca_array_get(DBR_INT, pv.COUNT, pv.CHID, &epicsINTArray[0]);
	//	for (auto& val : epicsINTArray)
	//		currentArray.push_back(val);
	//	break;
	//}
	case(DBR_TIME_ENUM):
	{
		std::vector<unsigned short> epicsENUMTimeArray(pv.COUNT);
		ca_array_get(DBR_TIME_ENUM, pv.COUNT, pv.CHID, &epicsENUMTimeArray[0]);
		for (auto& val : epicsENUMTimeArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_ENUM):
	{
		std::vector<unsigned short> epicsENUMArray(pv.COUNT);
		ca_array_get(DBR_ENUM, pv.COUNT, pv.CHID, &epicsENUMArray[0]);
		for (auto& val : epicsENUMArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_TIME_LONG):
	{
		std::vector<long> epicsLONGTimeArray(pv.COUNT);
		ca_array_get(DBR_TIME_LONG, pv.COUNT, pv.CHID, &epicsLONGTimeArray[0]);
		for (auto& val : epicsLONGTimeArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_LONG):
	{
		std::vector<long> epicsLONGArray(pv.COUNT);
		ca_array_get(DBR_TIME_LONG, pv.COUNT, pv.CHID, &epicsLONGArray[0]);
		for (auto& val : epicsLONGArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_TIME_FLOAT):
	{
		std::vector<float> epicsFLOATTimeArray(pv.COUNT);
		ca_array_get(DBR_TIME_FLOAT, pv.COUNT, pv.CHID, &epicsFLOATTimeArray[0]);
		for (auto& val : epicsFLOATTimeArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_FLOAT):
	{
		std::vector<float> epicsFLOATArray(pv.COUNT);
		ca_array_get(DBR_FLOAT, pv.COUNT, pv.CHID, &epicsFLOATArray[0]);
		for (auto& val : epicsFLOATArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_TIME_STRING):
	{
		std::vector<std::string> epicsSTRINGTimeArray(pv.COUNT);
		ca_array_get(DBR_TIME_STRING, pv.COUNT, pv.CHID, &epicsSTRINGTimeArray[0]);
		for (auto& val : epicsSTRINGTimeArray)
			currentArray.second.push_back(val);
		break;
	}
	case(DBR_STRING):
	{
		std::vector<std::string> epicsSTRINGArray(pv.COUNT);
		ca_array_get(DBR_STRING, pv.COUNT, pv.CHID, &epicsSTRINGArray[0]);
		for (auto& val : epicsSTRINGArray)
			currentArray.second.push_back(val);
		break;
	}
}
}

void Listener::initialiseCurrentValue(const pvStruct& pv)
{

	switch (ca_field_type(pv.CHID))
	{
	case(DBR_TIME_DOUBLE):
	{
		double epicsDBLTimeValue;
		ca_get(DBR_TIME_DOUBLE, pv.CHID, &epicsDBLTimeValue);
		currentValue.second = static_cast<double>(epicsDBLTimeValue);
		break;
	}
	case(DBR_DOUBLE):
	{
		double epicsDBLValue;
		ca_get(DBR_DOUBLE, pv.CHID, &epicsDBLValue);
		currentValue.second = static_cast<double>(epicsDBLValue);
		break;
	}
	case(DBR_TIME_SHORT):
	{
		short epicsSHORTValue;
		ca_get(DBR_TIME_SHORT, pv.CHID, &epicsSHORTValue);
		currentValue.second = static_cast<short>(epicsSHORTValue);
		break;
	}
	case(DBR_SHORT):
	{
		short epicsSHORTValue;
		ca_get(DBR_SHORT, pv.CHID, &epicsSHORTValue);
		currentValue.second = static_cast<short>(epicsSHORTValue);
		break;
	}
	//case(DBR_TIME_INT):
	//{
	//	int epicsINTTimeValue;
	//	ca_get(DBR_TIME_INT, pv.CHID, &epicsINTTimeValue);
	//	currentValue = static_cast<int>(epicsINTTimeValue);
	//	break;
	//}
	//case(DBR_INT):
	//{
	//	int epicsINTValue;
	//	ca_get(DBR_INT, pv.CHID, &epicsINTValue);
	//	currentValue = static_cast<int>(epicsINTValue);
	//	break;
	//}
	case(DBR_TIME_ENUM):
	{
		unsigned short epicsENUMTimeValue;
		ca_get(DBR_TIME_ENUM, pv.CHID, &epicsENUMTimeValue);
		currentValue.second = static_cast<unsigned short>(epicsENUMTimeValue);
		break;
	}
	case(DBR_ENUM):
	{
		unsigned short epicsENUMValue;
		ca_get(DBR_ENUM, pv.CHID, &epicsENUMValue);
		currentValue.second = static_cast<unsigned short>(epicsENUMValue);
		break;
	}
	case(DBR_TIME_LONG):
	{
		long epicsLONGTimeValue;
		ca_get(DBR_TIME_LONG, pv.CHID, &epicsLONGTimeValue);
		currentValue.second = static_cast<long>(epicsLONGTimeValue);
		break;
	}
	case(DBR_LONG):
	{
		long epicsLONGValue;
		ca_get(DBR_LONG, pv.CHID, &epicsLONGValue);
		currentValue.second = static_cast<long>(epicsLONGValue);
		break;
	}
	case(DBR_TIME_FLOAT):
	{
		float epicsFLOATTimeValue;
		ca_get(DBR_TIME_FLOAT, pv.CHID, &epicsFLOATTimeValue);
		currentValue.second = static_cast<float>(epicsFLOATTimeValue);
		break;
	}
	case(DBR_FLOAT):
	{
		float epicsFLOATValue;
		ca_get(DBR_FLOAT, pv.CHID, &epicsFLOATValue);
		currentValue.second = static_cast<float>(epicsFLOATValue);
		break;
	}
	case(DBR_TIME_STRING):
	{
		float epicsSTRINGTimeValue;
		ca_get(DBR_FLOAT, pv.CHID, &epicsSTRINGTimeValue);
		currentValue.second = static_cast<float>(epicsSTRINGTimeValue);
		break;
	}
	case(DBR_STRING):
	{
		float epicsSTRINGValue;
		ca_get(DBR_FLOAT, pv.CHID, &epicsSTRINGValue);
		currentValue.second = static_cast<float>(epicsSTRINGValue);
		break;
	}
	}

}

bool Listener::isConnected()
{
	if (ca_state(pv.CHID) == cs_conn)
	{
		return true;
	}
	else
	{
		return false;
	}
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
		std::cout << "already monitoring " << pv.fullPVName << std::endl;
	}
}

boost::python::list Listener::getArray_Py()
{
	if (isDoubleArray())
	{
		std::vector<double> d_vec;
		for (auto& item : currentArray.second)
		{
			d_vec.push_back(boost::get<double>(item));
		}
		return to_py_list(d_vec);
	}
	else if (isShortArray())
	{
		std::vector<short> i_vec;
		for (auto& item : currentArray.second)
		{
			i_vec.push_back(boost::get<short>(item));
		}
		return to_py_list(i_vec);
	}
	else if (isEnumArray())
	{
		std::vector<unsigned short> us_vec;
		for (auto& item : currentArray.second)
		{
			us_vec.push_back(boost::get<unsigned short>(item));
		}
		return to_py_list(us_vec);
	}
	else if (isStringArray())
	{
		std::vector<std::string> str_vec;
		for (auto& item : currentArray.second)
		{
			str_vec.push_back(boost::get<std::string>(item));
		}
		return to_py_list(str_vec);
	}
	else if (isFloatArray())
	{
		std::vector<float> f_vec;
		for (auto& item : currentArray.second)
		{
			f_vec.push_back(boost::get<float>(item));
		}
		return to_py_list(f_vec);
	}
	else if (isLongArray())
	{
		std::vector<long> l_vec;
		for (auto& item : currentArray.second)
		{
			l_vec.push_back(boost::get<long>(item));
		}
		return to_py_list(l_vec);
	}
}

boost::python::object Listener::getArrayAverage_Py()
{
	double mean = 0.0f;
	if (isDoubleArray())
	{
		mean = getArrayAverage<double>();
		return boost::python::object(mean);
	}
	else if (isShortArray())
	{
		mean = getArrayAverage<short>();
		return boost::python::object(mean);
	}
	else if (isFloatArray())
	{
		mean = getArrayAverage<float>();
		return boost::python::object(mean);
	}
	else if (isLongArray())
	{
		mean = getArrayAverage<long>();
		return boost::python::object(mean);
	}
}

void Listener::setBufferSize(int size)
{
	currentBuffer.clear();
	currentBuffer.set_capacity(size);
	messenger.printMessage("size of buffer is now: ", currentBuffer.capacity());
}

void Listener::setArrayBufferSize(int size)
{
	currentArrayBuffer.clear();
	currentArrayBuffer.set_capacity(size);
	messenger.printMessage("size of array buffer is now: ", currentArrayBuffer.capacity());
}

template <typename T>
std::vector<double> Listener::getArrayBufferAverageArray()
{
	if (!isStringArrayBuffer() && !isEnumArrayBuffer())
	{
		std::vector<double> bufferAverageVector(pv.COUNT);
		for (int i = 0; i < pv.COUNT; i++)
		{
			for (auto& vector : currentArrayBuffer)
			{
				bufferAverageVector.at(i) += boost::get<T>(vector.second.at(i));
			}
			bufferAverageVector.at(i) /= currentArrayBuffer.size();
		}
		return bufferAverageVector;
	}
}

boost::python::list Listener::getArrayBufferAverageArray_Py()
{
	std::vector<double> bufferAverageVector;
	if (isDoubleArrayBuffer())
	{
		bufferAverageVector = getArrayBufferAverageArray<double>();
	}
	else if (isLongArrayBuffer())
	{
		bufferAverageVector = getArrayBufferAverageArray<long>();
	}
	else if (isShortArrayBuffer())
	{
		bufferAverageVector = getArrayBufferAverageArray<short>();
	}
	else if (isFloatArrayBuffer())
	{
		bufferAverageVector = getArrayBufferAverageArray<float>();
	}

	boost::python::list bufferAverageList = to_py_list(bufferAverageVector);
	return bufferAverageList;
}



boost::python::object Listener::getValue_Py()
{
	if (!currentValue.second.empty())
	{
		return boost::apply_visitor(ListenerToPy::convert_to_py{}, currentValue.second);
	}
	else
	{
		std::cout << "waiting on connection, please try again.." << std::endl;
	}
}

boost::python::list Listener::getBuffer_Py()
{
	if (isDoubleBuffer())
	{
		boost::circular_buffer<double> dblBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			dblBuff.push_back(boost::get<double>(item.second));
		}
		return to_py_list(dblBuff);
	}
	else if (isShortBuffer())
	{
		boost::circular_buffer<short> shortBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			shortBuff.push_back(boost::get<short>(item.second));
		}
		return to_py_list(shortBuff);
	}
	else if (isLongBuffer())
	{
		boost::circular_buffer<long> longBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			longBuff.push_back(boost::get<long>(item.second));
		}
		return to_py_list(longBuff);
	}
	else if (isEnumBuffer())
	{
		boost::circular_buffer<unsigned short> usBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			usBuff.push_back(boost::get<unsigned short>(item.second));
		}
		return to_py_list(usBuff);
	}
	else if (isStringBuffer())
	{
		boost::circular_buffer<std::string> strBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			strBuff.push_back(boost::get<std::string>(item.second));
		}
		return to_py_list(strBuff);
	}
	else if (isFloatBuffer())
	{
		boost::circular_buffer<float> fltBuff(currentBuffer.capacity());
		for (auto& item : currentBuffer)
		{
			fltBuff.push_back(boost::get<float>(item.second));
		}
		return to_py_list(fltBuff);
	}
}

void Listener::clearBuffer()
{
	currentBuffer.clear();
}

void Listener::clearArrayBuffer()
{
	currentArrayBuffer.clear();
}

boost::python::list Listener::getArrayBuffer_Py()
{
	if (isDoubleArrayBuffer())
	{
		std::cout << "constructing py list" << std::endl;
		boost::circular_buffer<std::vector<double>> dblBuff(currentArrayBuffer.capacity());
		for (auto& vector : currentArrayBuffer)
		{
			std::vector<double> bufferVec;
			for (auto& item : vector.second)
			{
				bufferVec.push_back(boost::get<double>(item));
			}
			dblBuff.push_back(bufferVec);
		}
		return to_py_list(dblBuff);
	}
	else if (isShortArrayBuffer())
	{
		std::cout << "constructing py list" << std::endl;
		boost::circular_buffer<std::vector<short>> shortBuff(currentArrayBuffer.capacity());
		for (auto& vector : currentArrayBuffer)
		{
			std::vector<short> bufferVec;
			for (auto& item : vector.second)
			{
				bufferVec.push_back(boost::get<short>(item));
			}
			shortBuff.push_back(bufferVec);
		}
		return to_py_list(shortBuff);
	}
	else if (isLongArrayBuffer())
	{
		std::cout << "constructing py list" << std::endl;
		boost::circular_buffer<std::vector<long>> longBuff(currentArrayBuffer.capacity());
		for (auto& vector : currentArrayBuffer)
		{
			std::vector<long> bufferVec;
			for (auto& item : vector.second)
			{
				bufferVec.push_back(boost::get<long>(item));
			}
			longBuff.push_back(bufferVec);
		}
		return to_py_list(longBuff);
	}
	else if (isEnumArrayBuffer())
	{
		std::cout << "constructing py list" << std::endl;
		boost::circular_buffer<std::vector<unsigned short>> usBuff(currentArrayBuffer.capacity());
		for (auto& vector : currentArrayBuffer)
		{
			std::vector<unsigned short> bufferVec;
			for (auto& item : vector.second)
			{
				bufferVec.push_back(boost::get<unsigned short>(item));
			}
			usBuff.push_back(bufferVec);
		}
		return to_py_list(usBuff);
	}
	else if (isStringArrayBuffer())
	{
		std::cout << "constructing py list" << std::endl;
		boost::circular_buffer<std::vector<std::string>> strBuff(currentArrayBuffer.capacity());
		for (auto& vector : currentArrayBuffer)
		{
			std::vector<std::string> bufferVec;
			for (auto& item : vector.second)
			{
				bufferVec.push_back(boost::get<std::string>(item));
			}
			strBuff.push_back(bufferVec);
		}
		return to_py_list(strBuff);
	}
	else if (isFloatArrayBuffer())
	{
		std::cout << "constructing py list" << std::endl;
		boost::circular_buffer<std::vector<float>> floatBuff(currentArrayBuffer.capacity());
		for (auto& vector : currentArrayBuffer)
		{
			std::vector<float> bufferVec;
			for (auto& item : vector.second)
			{
				bufferVec.push_back(boost::get<float>(item));
			}
			floatBuff.push_back(bufferVec);
		}
		return to_py_list(floatBuff);
	}
}



double Listener::getBufferAverage()
{
	if (!isStringBuffer() && !isEnumBuffer())
	{
		double total = 0.0;
		for (auto& item : currentBuffer)
		{
			total += boost::get<double>(item.second);
		}
		return (total / currentBuffer.size());
	}
	else
	{
		messenger.printMessage("Cannot perform average on String or Enum types.");
	}

}

double Listener::getBufferStdDeviation()
{
	if (!isStringBuffer() && !isEnumBuffer())
	{
		double mean = getBufferAverage();
		double sum = 0.0;
		for (auto& item : currentBuffer)
		{
			sum += std::pow((boost::get<double>(item.second) - mean),2);
		}
		return std::sqrt((sum / currentBuffer.size()));
	}
	else
	{
		messenger.printMessage("Cannot perform average on String or Enum types.");
	}

}

bool Listener::isLong()
{
	return (currentValue.second.type() == typeid(long));
}

bool Listener::isDouble()
{
	return (currentValue.second.type() == typeid(double));
}

bool Listener::isShort()
{
	return (currentValue.second.type() == typeid(short));
}

bool Listener::isInt()
{
	return (currentValue.second.type() == typeid(int));
}

bool Listener::isEnum()
{
	return (currentValue.second.type() == typeid(unsigned short));
}

bool Listener::isString()
{
	return (currentValue.second.type() == typeid(std::string));
}

bool Listener::isFloat()
{
	return (currentValue.second.type() == typeid(float));
}

bool Listener::isLongArray()
{
	return (currentArray.second.at(0).type() == typeid(long));
}

bool Listener::isShortArray()
{
	return (currentArray.second.at(0).type() == typeid(short));
}

bool Listener::isDoubleArray()
{
	return (currentArray.second.at(0).type() == typeid(double));
}

bool Listener::isIntArray()
{
	return (currentArray.second.at(0).type() == typeid(int));
}

bool Listener::isFloatArray()
{
	return (currentArray.second.at(0).type() == typeid(float));
}

bool Listener::isEnumArray()
{
	return (currentArray.second.at(0).type() == typeid(unsigned short));
}

bool Listener::isStringArray()
{
	return (currentArray.second.at(0).type() == typeid(std::string));
}


bool Listener::isDoubleBuffer()
{
	if (currentBuffer.size() != 0)
	{
		return(currentBuffer.at(0).second.type() == typeid(double));
	}
}

bool Listener::isIntBuffer()
{
	if (currentBuffer.size() != 0)
	{
		return(currentBuffer.at(0).second.type() == typeid(int));
	}
}

bool Listener::isShortBuffer()
{
	if (currentBuffer.size() != 0)
	{
		return(currentBuffer.at(0).second.type() == typeid(short));
	}
}

bool Listener::isEnumBuffer()
{
	if (currentBuffer.size() != 0)
	{
		return(currentBuffer.at(0).second.type() == typeid(unsigned short));
	}
}

bool Listener::isStringBuffer()
{
	if (currentBuffer.size() != 0)
	{
		return(currentBuffer.at(0).second.type() == typeid(std::string));
	}
}

bool Listener::isFloatBuffer()
{
	if (currentBuffer.size() != 0)
	{
		return(currentBuffer.at(0).second.type() == typeid(float));
	}
}

bool Listener::isLongBuffer()
{
	if (currentBuffer.size() != 0)
	{
		return(currentBuffer.at(0).second.type() == typeid(long));
	}
}

bool Listener::isDoubleArrayBuffer()
{
	if (currentArrayBuffer.size() != 0)
	{
		return(currentArrayBuffer.at(0).second.at(0).type() == typeid(double));
	}
}

bool Listener::isShortArrayBuffer()
{
	if (currentArrayBuffer.size() != 0)
	{
		return (currentArrayBuffer.at(0).second.at(0).type() == typeid(short));
	}
}

bool Listener::isLongArrayBuffer()
{
	if (currentArrayBuffer.size() != 0)
	{
		return(currentArrayBuffer.at(0).second.at(0).type() == typeid(long));
	}
}

bool Listener::isFloatArrayBuffer()
{
	if (currentArrayBuffer.size() != 0)
	{
		return(currentArrayBuffer.at(0).second.at(0).type() == typeid(float));
	}
}

bool Listener::isIntArrayBuffer()
{
	if (currentArrayBuffer.size() != 0)
	{
		return(currentArrayBuffer.at(0).second.at(0).type() == typeid(int));
	}
}

bool Listener::isEnumArrayBuffer()
{
	if (currentArrayBuffer.size() != 0)
	{
		return(currentArrayBuffer.at(0).second.at(0).type() == typeid(unsigned short));
	}
}

bool Listener::isStringArrayBuffer()
{
	if (currentArrayBuffer.size() != 0)
	{
		return(currentArrayBuffer.at(0).second.at(0).type() == typeid(std::string));
	}
}
