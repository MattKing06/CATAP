#include <EPICSTools.h>
#include <boost/test/unit_test.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
BOOST_AUTO_TEST_SUITE(EPICSToolTests)

BOOST_AUTO_TEST_CASE(epics_tools_listener_test)
{
	std::cout << "*** RUNNING LISTENER TEST ***" << std::endl;
	const std::string pv = "CLA-C2V-MAG-HCOR-01:GETSETI";
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
		boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
		BOOST_CHECK_NE(monitor.getValue<double>(), GlobalConstants::double_min);
	}
	else
	{
		std::cout << pv << " COULD NOT CONNECT TO EPICS." << std::endl;
	}

}

BOOST_AUTO_TEST_CASE(epics_tools_listener_array_test)
{
	std::cout << "*** RUNNING LISTENER ARRAY TEST ***" << std::endl;
	EPICSTools ET = EPICSTools();
	const std::string pv = "CLA-GUN-LRF-CTRL-01:app:time_vector";
	ET.monitor(pv);
	Listener& monitor = ET.getMonitor(pv);
	boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
	std::vector<double> data = monitor.getArray<double>();
	BOOST_CHECK_EQUAL(data.size(), monitor.pv.COUNT);
	for (auto& value : data)
	{
		BOOST_CHECK_NE(value, GlobalConstants::double_min);
	}
}

BOOST_AUTO_TEST_CASE(epics_tools_listener_buffer_test)
{
	std::cout << "*** RUNNING LISTENER BUFFER TEST ***" << std::endl;
	std::string monPV = "CLA-C2V-MAG-HCOR-01:READI";
	std::string powerPV = "CLA-C2V-MAG-HCOR-01:SPOWER";
	std::string setPV = "CLA-C2V-MAG-HCOR-01:SETI";
	EPICSTools epicsTools = EPICSTools(STATE::VIRTUAL);
	epicsTools.monitor(monPV);
	epicsTools.put(setPV, 10.0);
	epicsTools.put(powerPV, 1);
	Listener& monitor = epicsTools.getMonitor(monPV);
	if (monitor.isConnected())
	{
		for (auto& item : monitor.currentBuffer)
		{
			double val = boost::get<double>(item.second);
			BOOST_CHECK_NE(val, GlobalConstants::double_min);
		}
	}

}

BOOST_AUTO_TEST_CASE(epics_tools_putter_test)
{
	std::cout << "*** RUNNING PUTTER TEST ***" << std::endl;
	std::string pvStr = "CLA-C2V-MAG-HCOR-01:SETI";
	std::string checkPvStr = "CLA-C2V-MAG-HCOR-01:GETSETI";
	EPICSTools epicsTools = EPICSTools();
	pvStruct pv = pvStruct();
	pv.fullPVName = pvStr;
	pv.monitor = false;
	ca_create_channel(std::string("VM-").append(pvStr.c_str()).c_str(), NULL, NULL, CA_PRIORITY_DEFAULT, &pv.CHID);
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
	std::string pvStr = "CLA-C2V-MAG-HCOR-01:RILK";
	EPICSTools epicsTools = EPICSTools();
	// creating CHID to check ca_state as .get function
	// sets up channels AND retrieves EPICS value which
	// will throw a bad_get exception if not connected to EPICS.
	pvStruct pv = pvStruct();
	pv.fullPVName = pvStr;
	pv.monitor = false;
	ca_create_channel(std::string("VM-").append(pvStr.c_str()).c_str(), NULL, NULL, CA_PRIORITY_DEFAULT, &pv.CHID);
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

BOOST_AUTO_TEST_CASE(get_pid_scan_pv_test)
{
	const std::string pv = "CLA-L01-LRF-CTRL-01:vm:phase:pid.SCAN";
	EPICSTools ET = EPICSTools(STATE::VIRTUAL);
	ET.monitor(pv);
	Listener pid_scan = ET.getMonitor(pv);
	if (pid_scan.isConnected())
	{
		bool isEnum = pid_scan.isEnum();
		auto val = pid_scan.getValue<unsigned short>();
	}
}


BOOST_AUTO_TEST_CASE(get_dc_array_with_buffer_test)
{
	const std::string pv = "CLA-C09-IOC-CS-04:FMC_1_ADC_0_READ";
	EPICSTools ET = EPICSTools(STATE::PHYSICAL);
	ET.monitor(pv);
	Listener& mon = ET.getMonitor(pv);
	if (mon.isConnected())
	{
		auto arrayVal = mon.getArrayBuffer<long>();
		BOOST_CHECK_EQUAL(arrayVal.capacity(), DEFAULT_BUFFER_SIZE);
	}

}

BOOST_AUTO_TEST_CASE(get_bam_array_with_buffer_test)
{
	const std::string pv = "BAM-TEST-DAQ:DATA2";
	EPICSTools ET = EPICSTools(STATE::PHYSICAL);
	ET.monitor(pv);
	Listener& mon = ET.getMonitor(pv);
	if (mon.isConnected())
	{
		auto arrayVal = mon.getArrayBuffer<short>();
		BOOST_CHECK_EQUAL(arrayVal.capacity(), DEFAULT_BUFFER_SIZE);
	}

}

BOOST_AUTO_TEST_CASE(get_timestamped_enum_value_test)
{
	const std::string pv = "CLA-C2V-MAG-HCOR-01:RPOWER";
	EPICSTools ET = EPICSTools(STATE::PHYSICAL);
	std::pair<std::string, unsigned short> vl = ET.getTimestampedValue<unsigned short>(pv);
}

BOOST_AUTO_TEST_CASE(get_timestamped_double_vector_test)
{
	const std::string pv = "CLA-L01-LRF-CTRL-01:ad1:dod_demod_vec";
	EPICSTools ET = EPICSTools(STATE::VIRTUAL);
	std::pair<epicsTimeStamp, std::vector<double>> vl = ET.getTimestampedArray<double>(pv);
}


BOOST_AUTO_TEST_SUITE_END()
