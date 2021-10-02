#include <boost/test/unit_test.hpp>
#include "MagnetFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(MagnetFactoryTestSuite)
BOOST_AUTO_TEST_CASE(start_test)
{
	BOOST_TEST_MESSAGE("------	RUNNING MAGNET FACTORY TESTS	------");
}

BOOST_AUTO_TEST_CASE(magnet_factory_get_magnet_properties_test)
{
	std::string testMagnetName = "CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	bool status = magFac.setup("nominal");
	if (status)
	{
		Magnet HCOR = magFac.getMagnet(testMagnetName);
		auto onlineProperties = HCOR.getOnlineProperties();
		auto offlineProperties = HCOR.getOfflineProperties();
		BOOST_CHECK(!onlineProperties.empty());
		BOOST_CHECK(!offlineProperties.empty());
	}
}

BOOST_AUTO_TEST_CASE(magnet_factory_turn_on_magnet_test)
{
	std::string testMagnetName = "CLA-C2V-MAG-HCOR-01";
	//std::string testMagnetName = "EBT-INJ-MAG-QUAD-12";
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	magFac.messagesOff();
	magFac.debugMessagesOff();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("SPOWER").CHID) == cs_conn)
		{
			magFac.switchOn(testMagnetName);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			BOOST_CHECK_EQUAL(magFac.getPSUState(testMagnetName), 1);
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
} 

BOOST_AUTO_TEST_CASE(magnet_factory_read_i_magnet_test)
{
	std::string testMagnetName = "CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	magFac.messagesOff();
	magFac.debugMessagesOff();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("SETI").CHID) == cs_conn)
		{
			srand(time(NULL));
			double currentToSet = rand() % 10 + 1.0;
			magFac.SETI(testMagnetName, currentToSet);
			std::this_thread::sleep_for(std::chrono::seconds(10));
			BOOST_CHECK_EQUAL(magFac.getSETI(testMagnetName), currentToSet);
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
}

BOOST_AUTO_TEST_CASE(magnet_factory_rilk_state_test)
{
	std::string testMagnetName = "CLA-C2V-MAG-HCOR-01";
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	magFac.messagesOff();
	magFac.debugMessagesOff();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("RILK").CHID) == cs_conn)
		{
			BOOST_CHECK_EQUAL(magFac.getILKState(testMagnetName), STATE::OK);
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
}

BOOST_AUTO_TEST_CASE(magnet_factory_get_all_magnet_currents_test)
{
	MagnetFactory magFac = MagnetFactory(STATE::VIRTUAL);
	std::string testMagnetName = "CLA-C2V-MAG-HCOR-01";
	magFac.messagesOff();
	magFac.debugMessagesOff();
	bool status = magFac.setup("nominal");
	if (status)
	{
		if (ca_state(magFac.getMagnet(testMagnetName).pvStructs.at("GETSETI").CHID) == cs_conn)
		{
			std::map<std::string, double> allMagCurrents = magFac.getAllSETI();
			BOOST_CHECK_NE(allMagCurrents.at(testMagnetName), std::numeric_limits<double>::min());
		}
		else
		{
			magFac.messenger.printDebugMessage("NOT CONNECTED TO EPICS");
		}
	}
	magFac.messenger.dumpToFile("MF_TEST_OUTPUT.txt");
}

//MagnetFactoryTestSuite/degauss_dips_and_quads_test

BOOST_AUTO_TEST_CASE(degauss_dips_and_quads_test)
{
	MagnetFactory magFac = MagnetFactory(STATE::PHYSICAL);
	//magFac.messagesOff();
	//magFac.debugMessagesOff();
	bool status = magFac.setup("nominal");
	if (status)
	{
	
		std::vector<std::string> quad_names = magFac.getAllQuadNames();
		// remove ba2 magnets 
		quad_names.erase(std::remove(quad_names.begin(), quad_names.end(), "EBT-INJ-MAG-QUAD-12"), quad_names.end());
		quad_names.erase(std::remove(quad_names.begin(), quad_names.end(), "EBT-INJ-MAG-QUAD-13"), quad_names.end());
		quad_names.erase(std::remove(quad_names.begin(), quad_names.end(), "EBT-INJ-MAG-QUAD-14"), quad_names.end());

		std::vector<std::string> dipole_names = magFac.getAllDipoleNames();
		// remove ba2 magnets 
		dipole_names.erase(std::remove(dipole_names.begin(), dipole_names.end(), "EBT-INJ-MAG-DIP-02"), dipole_names.end());
		dipole_names.erase(std::remove(dipole_names.begin(), dipole_names.end(), "EBT-INJ-MAG-DIP-03"), dipole_names.end());


		std::vector<std::string> corrector_names = magFac.getAllCorrectorNames();
		corrector_names.erase(std::remove(corrector_names.begin(), corrector_names.end(), "EBT-INJ-MAG-VCOR-10"), corrector_names.end());
		corrector_names.erase(std::remove(corrector_names.begin(), corrector_names.end(), "EBT-INJ-MAG-VCOR-11"), corrector_names.end());
		corrector_names.erase(std::remove(corrector_names.begin(), corrector_names.end(), "EBT-INJ-MAG-HCOR-10"), corrector_names.end());
		corrector_names.erase(std::remove(corrector_names.begin(), corrector_names.end(), "EBT-INJ-MAG-HCOR-11"), corrector_names.end());
		
		magFac.degauss(quad_names, false);
		magFac.degauss(dipole_names, false);
		magFac.degauss(corrector_names, false, false);

		bool still_degaussing = true;

		while (still_degaussing)
		{
			bool still_degaussing = false;

			for (const auto& name : quad_names)
			{
				if (magFac.isDegaussing(name))
				{
					still_degaussing = true;
				}
			}
			for (const auto& name : dipole_names)
			{
				if (magFac.isDegaussing(name))
				{
					still_degaussing = true;
				}
			}
			for (const auto& name : corrector_names)
			{
				if (magFac.isDegaussing(name))
				{
					still_degaussing = true;
				}
			}
			if (still_degaussing)
			{

			}
			else
			{
				break;
			}
		}
		size_t failure_count = 0;
		size_t success_count = 0;
		for (const auto& name : quad_names)
		{
			if (magFac.getLastDegaussSuccess(name))
			{
				success_count += 1;
			}
			else
			{
				failure_count +=1 ;
				std::cout << name << " failed to degauss" << std::endl;

			}
		}
		for (const auto& name : dipole_names)
		{
			if (magFac.getLastDegaussSuccess(name))
			{
				success_count += 1;
			}
			else
			{
				failure_count += 1;
				std::cout << name << " failed to degauss" << std::endl;
			}
		}
		for (const auto& name : corrector_names)
		{
			if (magFac.getLastDegaussSuccess(name))
			{
				success_count += 1;
			}
			else
			{
				failure_count += 1;
				std::cout << name << " failed to degauss" << std::endl;
			}
		}
		std::cout << "Total of " << failure_count << " magnets failed degaussing " << std::endl;
		std::cout << "Total of " << success_count << " magnets succeeded degaussing " << std::endl;
		BOOST_CHECK_EQUAL(failure_count, 0);
	}
	BOOST_CHECK_EQUAL(status, true);
}



BOOST_AUTO_TEST_SUITE_END();
