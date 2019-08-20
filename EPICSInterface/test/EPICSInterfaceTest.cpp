#define BOOST_TEST_MODULE EPICSInterfaceTest

#include <boost/test/unit_test.hpp>
#include "HardwareFactory.h"
#include "Magnet.h"
#include <string>
#include <vector>

LoggingSystem TEST_LOGGER(true, true);
BOOST_AUTO_TEST_CASE(check_chid_state_from_epics_interface_test)
{
	EPICSInterface epicsInterface = EPICSInterface();
	pvStruct pv;
	pv.fullPVName = "VM-CLA-C2V-MAG-VCOR-01";
	pv.pvRecord = "GETSETI";
	epicsInterface.retrieveCHID(pv);
	if (ca_state(pv.CHID) == cs_conn)
	{
		BOOST_CHECK_EQUAL(ca_read_access(pv.CHID), 1);
		BOOST_CHECK_EQUAL(ca_write_access(pv.CHID), 1);
	}
	else
	{
		TEST_LOGGER.printMessage("CANNOT CONNECT TO EPICS");
	}
}
