#include <EPICSTools.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(EPICSToolTests)

BOOST_AUTO_TEST_CASE(epics_tools_listener_test)
{
	std::string pv = "VM-CLA-C2V-MAG-HCOR-01:GETSETI";
	EPICSTools epicsTools = EPICSTools();
	epicsTools.monitor(pv);
	// need to find a way to slow down the construction maybe?
	// if i put a while loop (0 < i < 10000000) then I get the right
	// value returned. otherwise the value variable isn't set fast enough..
	if (ca_state(epicsTools.listenerMap[pv].pv.CHID) == cs_conn)
	{
		Listener& monitor = epicsTools.getMonitor(pv);
		boost::detail::Sleep(unsigned long(1.0));
		BOOST_CHECK_NE(monitor.getValue<double>(), GlobalConstants::double_min);
	}
	else
	{
		std::cout << pv << " COULD NOT CONNECT TO EPICS." << std::endl;
	}

}

BOOST_AUTO_TEST_CASE(epics_tools_getter_test)
{
	std::string pv = "VM-CLA-C2V-MAG-HCOR-01:RILK";
	EPICSTools epicsTools = EPICSTools();
	auto value = epicsTools.get<unsigned short>(pv);
	BOOST_CHECK_EQUAL(value, unsigned short(0));

}

BOOST_AUTO_TEST_SUITE_END()