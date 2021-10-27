#include <boost/test/unit_test.hpp>
#include <string>
#include <EPICSIMGInterface.h>
#include <boost/math/special_functions/fpclassify.hpp>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif

BOOST_AUTO_TEST_SUITE(EPICSIMGInterfaceTestSuite)
BOOST_AUTO_TEST_CASE(epics_img_interface_get_pressure_test)
{
	BOOST_TEST_MESSAGE("------	IMG INTERFACE: GET PRESSURE TEST	------");
	EPICSIMGInterface epicsInterface = EPICSIMGInterface();
	pvStruct setPV;
	setPV.fullPVName = "VM-EBT-LLV-VAC-IMG-01:PRES";
	epicsInterface.retrieveCHID(setPV);
	epicsInterface.retrieveCHTYPE(setPV);
	epicsInterface.retrieveCOUNT(setPV);
	std::cout << "SENDING TO EPICS..." << std::endl;
	EPICSInterface::sendToEPICS();
	std::cout << "SENT TO EPICS..." << std::endl;
	if (ca_state(setPV.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(ca_read_access(setPV.CHID), 1);
		BOOST_CHECK_EQUAL(ca_write_access(setPV.CHID), 1);
		double returnValue;
		ca_get(setPV.CHTYPE, setPV.CHID, &returnValue);
		ca_pend_io(CA_PEND_IO_TIMEOUT);
		BOOST_CHECK(isnan(returnValue) != true);
	}
	else
	{
		epicsInterface.messenger.printMessage(setPV.fullPVName + ": CANNOT CONNECT TO EPICS");
	}
}

BOOST_AUTO_TEST_CASE(epics_img_interface_monitor_channel_type_test)
{
	BOOST_TEST_MESSAGE("------	IMG INTERFACE: MONITOR CHANNEL TYPE TEST	------");
	EPICSIMGInterface epicsInterface = EPICSIMGInterface();
	pvStruct getPPV;
	getPPV.fullPVName = "VM-EBT-INJ-VAC-IMG-03:P";
	getPPV.pvRecord = "P";
	getPPV.monitor = true;
	epicsInterface.retrieveCHID(getPPV);
	EPICSInterface::sendToEPICS();
	epicsInterface.retrieveCHTYPE(getPPV);
	epicsInterface.retrieveCOUNT(getPPV);

	pvStruct getStaPV;
	getStaPV.fullPVName = "VM-EBT-INJ-VAC-IMG-03:STA";
	getStaPV.pvRecord = "STA";
	getStaPV.monitor = true;
	epicsInterface.retrieveCHID(getStaPV);
	EPICSInterface::sendToEPICS();
	epicsInterface.retrieveCHTYPE(getStaPV);
	epicsInterface.retrieveCOUNT(getStaPV);

	if (ca_state(getStaPV.CHID) == cs_conn && ca_state(getPPV.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(getPPV.monitorCHTYPE, DBR_TIME_DOUBLE);
		BOOST_CHECK_EQUAL(getStaPV.monitorCHTYPE, DBR_TIME_DOUBLE);
	}
	else
	{
		epicsInterface.messenger.printMessage(getStaPV.fullPVName + ": CANNOT CONNECT TO EPICS");
	}
}
BOOST_AUTO_TEST_SUITE_END()

