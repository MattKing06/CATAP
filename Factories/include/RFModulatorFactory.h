#ifndef RF_MODULATOR_FACTORY_H_
#define RF_MODULATOR_FACTORY_H_

#include <ConfigReader.h>
#include <RFModulator.h>
#include <GlobalStateEnums.h>
#include <vector>
class RFModulatorFactory
{
public:
	RFModulatorFactory();
	RFModulatorFactory(STATE mode);
	RFModulatorFactory(const RFModulatorFactory& copyFactory);
	~RFModulatorFactory();
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, RFModulator> RFModulatorMap;
};

#endif // RF_MODULATOR_FACTORY_H_