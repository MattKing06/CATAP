#ifndef GLOBAL_constANTS_H_
#define GLOBAL_constANTS_H_
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

namespace GlobalConstants 
{
	const double double_min = std::numeric_limits<double>::min();
	const double double_max = std::numeric_limits<double>::max();

	const double zero_double = 0.0;
	const double One_double = 1.0;

	const int zero_int= 0;
	const int one_int = 1;


}
#endif // GLOBAL_constANTS_H_
