#include <EPICSTools.h>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(epics_tools_simple_test)
{
	std::string pv = "VM-CLA-C2V-MAG-HCOR-01:GETSETI";
	EPICSTools epicsTools = EPICSTools();
	epicsTools.monitor(pv);
	// need to find a way to slow down the construction maybe?
	// if i put a while loop (0 < i < 10000000) then I get the right
	// value returned. otherwise the value variable isn't set fast enough..
	if (ca_state(epicsTools.listenerMap[pv].pv.CHID) == cs_conn)
	{
		Listener monitor = epicsTools.getMonitor(pv);
		std::cout << monitor.pv.fullPVName << " = " << monitor.getValue<double>() << std::endl;
	}
	else
	{
		std::cout << pv << " COULD NOT CONNECT TO EPICS." << std::endl;
	}

}
