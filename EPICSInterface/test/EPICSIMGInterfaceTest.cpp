#include <boost/test/unit_test.hpp>
#include <string>
#include <EPICSIMGInterface.h>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif

BOOST_AUTO_TEST_CASE(epics_img_interface_get_pressure_test)
{
	EPICSIMGInterface epicsInterface = EPICSIMGInterface();
	pvStruct setPV;
	setPV.fullPVName = "VM-EBT-INJ-VAC-IMG-01";
	setPV.pvRecord = "P";
	epicsInterface.retrieveCHID(setPV);
	epicsInterface.retrieveCHTYPE(setPV);
	epicsInterface.retrieveCOUNT(setPV);
	if (ca_state(setPV.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(ca_read_access(setPV.CHID), 1);
		BOOST_CHECK_EQUAL(ca_write_access(setPV.CHID), 1);
		double returnValue;
		ca_get(setPV.CHTYPE, setPV.CHID, &returnValue);
		ca_pend_io(CA_PEND_IO_TIMEOUT);
		BOOST_CHECK_EQUAL(returnValue, 3.0);
	}
	else
	{
		epicsInterface.messenger.printMessage("CANNOT CONNECT TO EPICS");
	}
}

BOOST_AUTO_TEST_CASE(epics_img_interface_monitor_channel_type_test)
{
	EPICSIMGInterface epicsInterface = EPICSIMGInterface();
	pvStruct getPPV;
	getPPV.fullPVName = "VM-EBT-INJ-VAC-IMG-01";
	getPPV.pvRecord = "P";
	getPPV.monitor = true;
	epicsInterface.retrieveCHID(getPPV);
	epicsInterface.retrieveCHTYPE(getPPV);
	epicsInterface.retrieveCOUNT(getPPV);

	pvStruct getStaPV;
	getStaPV.fullPVName = "VM-EBT-INJ-VAC-IMG-01";
	getStaPV.pvRecord = "Sta";
	getStaPV.monitor = true;
	epicsInterface.retrieveCHID(getStaPV);
	epicsInterface.retrieveCHTYPE(getStaPV);
	epicsInterface.retrieveCOUNT(getStaPV);

	if (ca_state(getStaPV.CHID) == cs_conn && ca_state(getPPV.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(getPPV.monitorCHTYPE, DBR_TIME_DOUBLE);
		BOOST_CHECK_EQUAL(getStaPV.monitorCHTYPE, DBR_TIME_DOUBLE);
	}
	else
	{
		epicsInterface.messenger.printMessage("CANNOT CONNECT TO EPICS");
	}
}


//BOOST_AUTO_TEST_CASE(epics_img_interface_retrieve_update_test)
//{
//	EPICSIMGInterface epicsInterface = EPICSIMGInterface();
//	pvStruct getPPV;
//	getPPV.fullPVName = "VM-EBT-INJ-VAC-IMG-01";
//	getPPV.pvRecord = "P";
//	getPPV.monitor = true;
//	epicsInterface.retrieveCHID(getPPV);
//	epicsInterface.retrieveCHTYPE(getPPV);
//	epicsInterface.retrieveCOUNT(getPPV);
//  epicsInterface.retrieveFunctionForRecord(getPPV)

//}
