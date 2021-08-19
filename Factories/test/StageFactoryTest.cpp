#include <boost/test/unit_test.hpp>
#include <StageFactory.h>
#include <cadef.h>

BOOST_AUTO_TEST_SUITE(StageFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_stage_factory_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING STAGE FACTORY TESTS	------");
}


BOOST_AUTO_TEST_CASE(construct_stage_factory_test)
{
	StageFactory fac = StageFactory(STATE::PHYSICAL);
	fac.setup("nominal");
	for (auto& stage : fac.stageMap)
	{
		std::cout << stage.first << std::endl;
		std::cout << "min_pos: " << stage.second.getMinPosition() << std::endl;
		std::cout << "max_pos: " << stage.second.getMaxPosition() << std::endl;
	}
	std::cout << "moving stage" << std::endl;
	std::string stageName = "EBT-BA1-MOT-COFF-H-04";
	Stage& stage = fac.getStage(stageName);
	std::cout << "getting stage: " << stageName << std::endl;
	std::pair<epicsTimeStamp, double> currentPosition = stage.getCurrentPosition();
	if (stage.getMinPosition() <= currentPosition.second - 1.0)
	{
		if (stage.getMaxPosition() >= currentPosition.second - 1.0)
		{
			stage.setNewPosition(currentPosition.second - 1.0);
		}
	}
	else
	{
		std::cout << stageName << " AT MAX POSITION " << std::endl;
	}


}

BOOST_AUTO_TEST_SUITE_END()