#define BOOST_TEST_MODULE EPICSInterfaceTest

#include <boost/test/unit_test.hpp>
#include <LoggingSystem.h>
#include <EPICSInterface.h>
#include <string>
#include <vector>

BOOST_AUTO_TEST_CASE(check_chid_state_from_epics_interface_test)
{
	LoggingSystem::messagesOn();
	EPICSInterface epicsInterface = EPICSInterface();
	pvStruct pv;
	pv.fullPVName = "VM-CLA-C2V-MAG-VCOR-01";
	pv.pvRecord = "READI";
	epicsInterface.retrieveCHID(pv);
	if (ca_state(pv.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(ca_read_access(pv.CHID), 1);
		BOOST_CHECK_EQUAL(ca_write_access(pv.CHID), 1);
	}
	else
	{
		LoggingSystem::printMessage("CANNOT CONNECT TO EPICS");
	}
}

BOOST_AUTO_TEST_CASE(check_put_value_from_epics_interface_test)
{
	EPICSInterface epicsInterface = EPICSInterface();
	pvStruct pvToSet;
	pvToSet.fullPVName = "VM-CLA-C2V-MAG-VCOR-01";
	pvToSet.pvRecord = "SETI";
	epicsInterface.retrieveCHID(pvToSet);
	epicsInterface.retrieveCHTYPE(pvToSet);
	epicsInterface.retrieveCOUNT(pvToSet);
	if (ca_state(pvToSet.CHID) == cs_conn)
	{
		srand(time(NULL));
		double currentToSet = rand() % 10 + 1.0;
		double returnedCurrent;
		epicsInterface.putValue(pvToSet, currentToSet);
		ca_get(pvToSet.CHTYPE, pvToSet.CHID, &returnedCurrent);
		ca_pend_io(CA_PEND_IO_TIMEOUT);
		BOOST_CHECK_CLOSE(currentToSet, returnedCurrent, 1e-10);
	}
	else
	{
		LoggingSystem::printMessage("CANNOT CONNECT TO EPICS");
	}
}
