#include <boost/test/unit_test.hpp>
#include <StageFactory.h>

BOOST_AUTO_TEST_SUITE(StageFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_stage_factory_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING STAGE FACTORY TESTS	------");
}


BOOST_AUTO_TEST_CASE(construct_stage_factory_test)
{
	StageFactory fac = StageFactory(STATE::VIRTUAL);
	fac.setup("nominal");
	for (auto& stage : fac.stageMap)
	{
		std::cout << stage.first << std::endl;
		std::cout << "min_pos: " << stage.second.getMinPosition() << std::endl;
		std::cout << "max_pos: " << stage.second.getMaxPosition() << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()