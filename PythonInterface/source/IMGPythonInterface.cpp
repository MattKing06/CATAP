#include <IMGPythonInterface.h>

BOOST_PYTHON_MODULE(_IMG)
{
	BOOST_PYTHON_IMG_INCLUDE::expose_img_object();
	BOOST_PYTHON_IMG_INCLUDE::expose_img_factory_object();
}