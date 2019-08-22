#define BOOST_TEST_MODULE EPICSShutterInterfaceTest

#include <boost/test/unit_test.hpp>
#include <string>
#include <EPICSMagnetInterface.h>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif

LoggingSystem TEST_LOGGER(true, true);

BOOST_AUTO_TEST_CASE(epics_magnet_interface_put_and_get_value_test)
{
	EPICSMagnetInterface epicsInterface = EPICSMagnetInterface();
	pvStruct setIPV;
	setIPV.fullPVName = "VM-CLA-C2V-MAG-VCOR-01";
	setIPV.pvRecord = "SETI";
	epicsInterface.retrieveCHID(setIPV);
	epicsInterface.retrieveCHTYPE(setIPV);
	epicsInterface.retrieveCOUNT(setIPV);
	pvStruct getSetIPV;
	getSetIPV.fullPVName = "VM-CLA-C2V-MAG-VCOR-01";
	getSetIPV.pvRecord = "GETSETI";
	epicsInterface.retrieveCHID(getSetIPV);
	epicsInterface.retrieveCHTYPE(getSetIPV);
	epicsInterface.retrieveCOUNT(getSetIPV);
	if (ca_state(setIPV.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(ca_read_access(setIPV.CHID), 1);
		BOOST_CHECK_EQUAL(ca_write_access(setIPV.CHID), 1);
		srand(time(NULL));
		double currentToSet = rand() % 10 + 1.0;
		epicsInterface.putValue(setIPV, currentToSet);
		double returnValue;
		ca_get(getSetIPV.CHTYPE, getSetIPV.CHID, &returnValue);
		ca_pend_io(CA_PEND_IO_TIMEOUT);
		BOOST_CHECK_EQUAL(returnValue, currentToSet);
	}
	else
	{
		TEST_LOGGER.printMessage("CANNOT CONNECT TO EPICS");
	}
}

BOOST_AUTO_TEST_CASE(epics_magnet_interface_monitor_channel_type_test)
{
	EPICSMagnetInterface epicsInterface = EPICSMagnetInterface();
	pvStruct getSetIPV;
	getSetIPV.fullPVName = "VM-CLA-C2V-MAG-VCOR-01";
	getSetIPV.pvRecord = "GETSETI";
	getSetIPV.monitor = true;
	epicsInterface.retrieveCHID(getSetIPV);
	epicsInterface.retrieveCHTYPE(getSetIPV);
	epicsInterface.retrieveCOUNT(getSetIPV);

	pvStruct getRPowerPV;
	getRPowerPV.fullPVName = "VM-CLA-C2V-MAG-VCOR-01";
	getRPowerPV.pvRecord = "RPOWER";
	getRPowerPV.monitor = true;
	epicsInterface.retrieveCHID(getRPowerPV);
	epicsInterface.retrieveCHTYPE(getRPowerPV);
	epicsInterface.retrieveCOUNT(getRPowerPV);

	if (ca_state(getSetIPV.CHID) == cs_conn && ca_state(getRPowerPV.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(getSetIPV.MonitorCHTYPE, DBR_TIME_DOUBLE);
		BOOST_CHECK_EQUAL(getRPowerPV.MonitorCHTYPE, DBR_TIME_ENUM);
	}
	else
	{
		TEST_LOGGER.printMessage("CANNOT CONNECT TO EPICS");
	}
}