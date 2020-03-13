#ifndef LIGHTING_FACTORY_H_
#define LIGHTING_FACTORY_H_

#include <ConfigReader.h>
#include <Lighting.h>
#include <GlobalStateEnums.h>
#include <vector>
class LightingFactory
{
public:
	LightingFactory();
	LightingFactory(STATE mode);
	LightingFactory(const LightingFactory& copyFactory);
	~LightingFactory();
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, Lighting> LightingMap;
};

#endif // LIGHTING_FACTORY_H_