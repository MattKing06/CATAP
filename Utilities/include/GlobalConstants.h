#ifndef GLOBAL_CONSTANTS_H_
#define GLOBAL_CONSTANTS_H_
#include <limits>
#include <map>
#include <string>


// where i sthe best to put this ??
// http://forums.codeguru.com/showthread.php?369447-can-t-assign-numeric_limits-lt-double-gt-max()
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
/*! A namespace for defining constants used throughout CATAP */
namespace GlobalConstants 
{
	const double double_min = std::numeric_limits<double>::min(); //!< Minimum possible value for double-type
	const double double_max = std::numeric_limits<double>::max(); //!< Maximum possible value for double-type

}
#endif // GLOBAL_CONSTANTS_H_