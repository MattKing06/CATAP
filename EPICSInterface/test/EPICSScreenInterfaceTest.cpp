#include <boost/test/unit_test.hpp>
#include <string>
#include <EPICSBPMInterface.h>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif

BOOST_AUTO_TEST_CASE(epics_bpm_interface_put_and_get_value_test)
{
	EPICSBPMInterface epicsInterface = EPICSBPMInterface();
	pvStruct setSA1PV;
	setSA1PV.fullPVName = "VM-CLA-S02-DIA-BPM-01";
	setSA1PV.pvRecord = "SA1";
	epicsInterface.retrieveCHID(setSA1PV);
	epicsInterface.retrieveCHTYPE(setSA1PV);
	epicsInterface.retrieveCOUNT(setSA1PV);
	pvStruct getRA1PV;
	getRA1PV.fullPVName = "VM-CLA-S02-DIA-BPM-01";
	getRA1PV.pvRecord = "RA1";
	epicsInterface.retrieveCHID(getRA1PV);
	epicsInterface.retrieveCHTYPE(getRA1PV);
	epicsInterface.retrieveCOUNT(getRA1PV);
	if (ca_state(setSA1PV.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(ca_read_access(setSA1PV.CHID), 1);
		BOOST_CHECK_EQUAL(ca_write_access(setSA1PV.CHID), 1);
		srand(time(NULL));
		long sa1ToSet = rand() % 10;
		epicsInterface.putValue(setSA1PV, sa1ToSet);
		long returnValue;
		ca_get(getRA1PV.CHTYPE, getRA1PV.CHID, &returnValue);
		ca_pend_io(CA_PEND_IO_TIMEOUT);
		BOOST_CHECK_EQUAL(returnValue, sa1ToSet);
	}
	else
	{
		epicsInterface.messenger.printMessage("CANNOT CONNECT TO EPICS");
	}
}