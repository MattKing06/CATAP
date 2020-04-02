#include <LaserMirrorPythonInterface.h>

BOOST_PYTHON_MODULE(_LaserMirror)
{
	BOOST_PYTHON_LASER_MIRROR_INCLUDE::expose_laser_mirror_object();
	BOOST_PYTHON_LASER_MIRROR_INCLUDE::expose_laser_mirror_factory_object();
}