#define BOOST_TEST_MODULE MagnetFactoryTest

#include <boost/test/unit_test.hpp>
#include "BPMFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(BPMFactoryTestSuite)

BOOST_AUTO_TEST_CASE(bpm_factory_set_and_check_sa1_test)
{
	std::string testBPMName = "VM-CLA-S02-DIA-BPM-01";
	BPMFactory bpmfac = BPMFactory(true);
	bpmfac.setup("nominal");
	srand(time(NULL));
	double sa1ToSet = rand() % 10;
	bpmfac.setSA1(testBPMName, sa1ToSet);
	std::this_thread::sleep_for(std::chrono::seconds(10));
	BOOST_CHECK_EQUAL(bpmfac.getSA1(testBPMName), sa1ToSet);
}

//BOOST_AUTO_TEST_CASE(bpm_factory_set_and_check_data_test)
//{
//	BPM bpm = BPM();
//	srand(time(NULL));
//	std::vector< double > dataToSet(9);
//	for (auto& it : dataToSet)
//	{
//		it = rand() % 10 + 1.0;
//	}
//	bpm.setData(dataToSet);
//	BOOST_CHECK_EQUAL_COLLECTIONS(bpm.getData().begin(), bpm.getData().end(), dataToSet.begin(), dataToSet.end());
//}
//
//BOOST_AUTO_TEST_CASE(bpm_factory_set_and_check_data_vector_test)
//{
//	BPM bpm = BPM();
//	srand(time(NULL));
//	std::vector< std::vector< double > > dataToSet(2);
//	bpm.setVectorSize(2);
//	for (auto&& it : dataToSet)
//	{
//		it.resize(9);
//		for (auto&& it2 : it)
//		{
//			it2 = rand() % 10 + 1.0;
//		}
//	}
//	bpm.setData(dataToSet.back());
//	std::vector< double > get = bpm.getData();
//	std::vector< double > set = dataToSet.back();
//	BOOST_CHECK_EQUAL_COLLECTIONS(get.begin(), get.end(), set.begin(), set.end());
//}

//BOOST_AUTO_TEST_CASE(magnet_factory_read_i_magnet_test)
//{
//	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
//	MagnetFactory magFac = MagnetFactory(true);
//	magFac.setup("nominal");
//	srand(time(NULL));
//	double currentToSet = rand() % 10 + 1.0;
//	magFac.setCurrent(testMagnetName, currentToSet);
//	std::this_thread::sleep_for(std::chrono::seconds(10));
//	BOOST_CHECK_CLOSE(magFac.getRICurrent(testMagnetName), currentToSet, 10);
//}
//
//BOOST_AUTO_TEST_CASE(magnet_factory_rilk_state_test)
//{
//	std::string testMagnetName = "VM-CLA-C2V-MAG-HCOR-01";
//	MagnetFactory magFac = MagnetFactory(true);
//	magFac.setup("nominal");
//	BOOST_CHECK_EQUAL(magFac.getILKState(testMagnetName), 0);
//}

BOOST_AUTO_TEST_SUITE_END();