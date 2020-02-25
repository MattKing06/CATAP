#ifndef LASER_MIRROR_FACTORY_H_
#define LASER_MIRROR_FACTORY_H_

#include <ConfigReader.h>
#include <LaserMirror.h>
#include <GlobalStateEnums.h>
#include <vector>
class LaserMirrorFactory
{
public:
	LaserMirrorFactory();
	LaserMirrorFactory(STATE mode);
	LaserMirrorFactory(const LaserMirrorFactory& copyFactory);
	~LaserMirrorFactory();
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, LaserMirror> LaserMirrorMap;
};

#endif // LASER_MIRROR_FACTORY_H_