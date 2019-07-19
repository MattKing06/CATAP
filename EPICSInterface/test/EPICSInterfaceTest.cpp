#define BOOST_TEST_MODULE EPICSInterfaceTest

#include <boost/test/unit_test.hpp>
#include "HardwareFactory.h"
#include "Magnet.h"
#include <string>
#include <vector>

LoggingSystem TEST_LOGGER(true, true);

BOOST_AUTO_TEST_CASE(epics_interface_test)
{
	BOOST_TEST((true && true));
}

BOOST_AUTO_TEST_CASE(fill_hardware_pv_data_with_chids)
{
	HardwareFactory componentFactory = HardwareFactory();
	MagnetFactory magFac = componentFactory.getMagnetFactory();
	//EPICSInterface *ei = new EPICSInterface();
	bool status;
	status = magFac.setup("nominal");
	BOOST_CHECK(status);
	double current = magFac.getCurrent("CLA-C2V-MAG-HCOR-01");
	//Magnet* magnet = magFac->getMagnet(std::string("CLA-C2V-MAG-VCOR-01"));
	//std::vector<pvStruct> *pvStructs = magnet->getPVStructs();
	//for (auto pv = pvStructs->begin(); pv != pvStructs->end(); pv++)
	//{
	//	pv->CHID = magnet->epicsInterface->retrieveCHID(pv->fullPVName + ":" + pv->pvRecord);
	//	status = ca_pend_io(1.0);
	//}
	//
	//for (auto pv = pvStructs->begin(); pv != pvStructs->end(); pv++)
	//{
	//	TEST_LOGGER.printMessage("PV FROM OBJECT: " + pv->fullPVName + ":" + pv->pvRecord);
	//	std::string name = std::string(ca_name(pv->CHID));
	//	TEST_LOGGER.printMessage("PV FROM CHID: " + name);
	//	printf("read(%d) write(%d) state(%d) \n",
	//		ca_read_access(pv->CHID),
	//		ca_write_access(pv->CHID),
	//		ca_state(pv->CHID));
	//	
	//	//if (name == "CLA-C2V-MAG-VCOR-01:READI")
	//	//{
	//	//	double current = magFac->getCurrent("CLA-C2V-MAG-HCOR-01");
	//	//	//ca_task_initialize();
	//	//	//ca_get(DBR_DOUBLE, pv->CHID, &current);
	//	//	//ca_pend_io(15.0);
	//	//	TEST_LOGGER.printMessage(name + " READI = " + std::to_string(current));
	//	//}
	//}
}
