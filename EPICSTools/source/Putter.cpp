#include <Putter.h>
#include <PythonTypeConversions.h>

Putter::Putter()
{

}

Putter::Putter(const std::string& pv) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	pvToPut(pv),
	mode(STATE::UNKNOWN),
	messenger(LoggingSystem(false, true))
{
	setupChannels();
}

Putter::Putter(STATE mode, const std::string& pv) :
	epicsInterface(boost::make_shared<EPICSInterface>()),
	pvToPut(pv),
	mode(mode),
	messenger(LoggingSystem(false, true))
{
	setupChannels();
}

Putter::Putter(const Putter& copyPutter) : 
	epicsInterface(copyPutter.epicsInterface),
	pvToPut(copyPutter.pvToPut),
	mode(copyPutter.mode),
	messenger(copyPutter.messenger),
	pv(copyPutter.pv)
{
	setupChannels();
}

void Putter::setupChannels()
{
	pv = pvStruct();
	pv.fullPVName = pvToPut;
	pv.monitor = false;
	epicsInterface->retrieveCHID(pv);
	EPICSInterface::sendToEPICS();
	epicsInterface->retrieveCHTYPE(pv);
	std::cout << pvToPut << " CHANNEL TYPE: " << pv.CHTYPE << std::endl;
	epicsInterface->retrieveCOUNT(pv);
	pv.MASK = DBE_VALUE;
	EPICSInterface::sendToEPICS();
}

void Putter::putArray_Py(boost::python::list pyValue)
{
	switch (pv.CHTYPE)
	{
		case(DBR_DOUBLE):
		{
			std::vector<double> d_vector = to_std_vector<double>(pyValue);
			epicsInterface->putArray<double>(pv, d_vector);
			break;
		}
		case(DBR_ENUM):
		{
			std::vector<unsigned short> us_vector = to_std_vector<unsigned short>(pyValue);
			epicsInterface->putArray<unsigned short>(pv, us_vector);
			break;
		}
		case(DBR_INT):
		{
			std::vector<int> i_vector = to_std_vector<int>(pyValue);
			epicsInterface->putArray<int>(pv, i_vector);
			break;
		}
		case(DBR_LONG):
		{
			std::vector<long> l_vector = to_std_vector<long>(pyValue);
			epicsInterface->putArray<long>(pv, l_vector);
			break;
		}
		case(DBR_FLOAT):
		{
			std::vector<float> f_vector = to_std_vector<float>(pyValue);
			epicsInterface->putArray<float>(pv, f_vector);
			break;
		}
		case(DBR_CHAR):
		{		
			std::vector<char> c_vector = to_std_vector<char>(pyValue);
			epicsInterface->putArray<char>(pv, c_vector);
			break;
		}
	}
}

void Putter::put_Py(boost::python::object pyValue)
{
	switch (pv.CHTYPE) 
	{
		case(DBR_DOUBLE):
		{
			double d_value = boost::python::extract<double>(pyValue);
			epicsInterface->putValue<double>(pv, d_value);
			break;
		}
		case(DBR_ENUM):
		{
			unsigned short us_value = boost::python::extract<unsigned short>(pyValue);
			epicsInterface->putValue<unsigned short>(pv, us_value);
			break;
		}
		case(DBR_INT):
		{
			int i_value = boost::python::extract<int>(pyValue);
			epicsInterface->putValue<int>(pv, i_value);
			break;
		}
		case(DBR_LONG):
		{
			long l_value = boost::python::extract<long>(pyValue);
			epicsInterface->putValue<long>(pv, l_value);
			break;
		}
		case(DBR_FLOAT):
		{
			float f_value = boost::python::extract<float>(pyValue);
			epicsInterface->putValue<float>(pv, f_value);
			break;
		}
		case(DBR_CHAR):
		{
			char ch_value = boost::python::extract<char>(pyValue);
			epicsInterface->putValue<char>(pv, ch_value);
			break;
		}
	}
}
