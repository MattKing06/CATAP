#include <EPICSTools.h>
#include <boost/test/unit_test.hpp>
#include <chrono>
#include <thread>
BOOST_AUTO_TEST_SUITE(EPICSToolTests)

BOOST_AUTO_TEST_CASE(epics_tools_listener_test)
{
	std::cout << "*** RUNNING LISTENER TEST ***" << std::endl;
	std::string pv = "VM-CLA-C2V-MAG-HCOR-01:GETSETI";
	EPICSTools epicsTools = EPICSTools();
	epicsTools.monitor(pv);
	// need to find a way to slow down the construction maybe?
	// if i put a while loop (0 < i < 10000000) then I get the right
	// value returned. otherwise the value variable isn't set fast enough..
	if (ca_state(epicsTools.listenerMap[pv].pv.CHID) == cs_conn)
	{
		Listener& monitor = epicsTools.getMonitor(pv);
		//TRY TO USE BOOST/STD THREAD SLEEP
		// IF THAT FAILS, TRY #IF WIN32 #IF UNIX STYLE.
		unsigned long waitTime(1.0);
		std::chrono::milliseconds timespan(waitTime);
		std::this_thread::sleep_for(timespan);
		BOOST_CHECK_NE(monitor.getValue<double>(), GlobalConstants::double_min);
	}
	else
	{
		std::cout << pv << " COULD NOT CONNECT TO EPICS." << std::endl;
	}

}

BOOST_AUTO_TEST_CASE(epics_tools_putter_test)
{
	std::cout << "*** RUNNING PUTTER TEST ***" << std::endl;
	std::string pvStr = "VM-CLA-C2V-MAG-HCOR-01:SETI";
	std::string checkPvStr = "VM-CLA-C2V-MAG-HCOR-01:GETSETI";
	EPICSTools epicsTools = EPICSTools();
	pvStruct pv = pvStruct();
	pv.fullPVName = pvStr;
	pv.monitor = false;
	ca_create_channel(pvStr.c_str(), NULL, NULL, CA_PRIORITY_DEFAULT, &pv.CHID);
	EPICSInterface::sendToEPICS();
	if (ca_state(pv.CHID) == cs_conn)
	{
		double setCurrent = 10.0;
		epicsTools.put<double>(pvStr, setCurrent);
		BOOST_CHECK_EQUAL(epicsTools.get<double>(checkPvStr), setCurrent);
	}
	else
	{
		std::cout << pvStr << " COULD NOT CONNECT TO EPICS." << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(epics_tools_getter_test)
{
	std::cout << "*** RUNNING GETTER TEST ***" << std::endl;
	std::string pvStr = "VM-CLA-C2V-MAG-HCOR-01:RILK";
	EPICSTools epicsTools = EPICSTools();
	// creating CHID to check ca_state as .get function
	// sets up channels AND retrieves EPICS value which
	// will throw a bad_get exception if not connected to EPICS.
	pvStruct pv = pvStruct();
	pv.fullPVName = pvStr;
	pv.monitor = false;
	ca_create_channel(pvStr.c_str(), NULL, NULL, CA_PRIORITY_DEFAULT, &pv.CHID);
	EPICSInterface::sendToEPICS();
	// need to check CHID state without constructing get...
	if (ca_state(pv.CHID) == cs_conn)
	{
		auto value = epicsTools.get<unsigned short>(pvStr);
		unsigned short checkValue(0);
		BOOST_CHECK_EQUAL(value, checkValue);
	}
	else
	{
		std::cout << pvStr << " COULD NOT CONNECT TO EPICS." << std::endl;
	}

}

BOOST_AUTO_TEST_SUITE_END()