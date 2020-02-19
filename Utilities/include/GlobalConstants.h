#ifndef GLOBAL_CONSTANTS_H_
#define GLOBAL_CONSTANTS_H_
#include <limits>
#include <map>
#include <string>
#include <chrono>


// where i sthe best to put this ??
// http://forums.codeguru.com/showthread.php?369447-can-t-assign-numeric_limits-lt-double-gt-max()
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

/*! @defgroup utils Utilities
 *  @brief A collection of classes for providing often used functions, constants, and enums throughout CATAP.
 *  @{*/
/*!
	Namespace to define constants that are used throughout CATAP for ease-of-use.
*/

namespace GlobalConstants 
{
	/*! Minimum possible value of a double */
	const double double_min = std::numeric_limits<double>::min();
	/*! Maximum possible value of a double */
	const double double_max = std::numeric_limits<double>::max();

	/*! Double-type variable for 0.0 */
	const double zero_double = 0.0;
	/*! Double-type variable for 1.0 */
	const double one_double = 1.0;
	/*! Double-type variable for 0.1 */
	const double zero_point_one_double = 0.1;
	/*! Integer-type variable for 0 */
	const int zero_int= 0;
	/*! Integer-type variable for 1 */
	const int one_int = 1;

	/*! Unsigned Integer-type for 0 */
	const size_t zero_sizet = 0;
	/*! Unsigned Integer-type for 1 */
	const size_t one_sizet = 1;

	/*! Unsigned Short-type to represent ACTIVATE to EPICS */
	const unsigned short EPICS_ACTIVATE = 1;
	/*! Unsigned Short-type to represent SEND to EPICS */
	const unsigned short EPICS_SEND = 0;
	/*! Unsigned Short-type to represent RESET to EPICS after ACTIVATE and SEND have been sent */
	const unsigned short EPICS_RESET = 1;

	/*! 2 second pause */
	const std::chrono::milliseconds STANDARD_PAUSE(2000);
	/*! 1 second pause */
	const std::chrono::milliseconds PAUSE_1000(1000);
	/*! 0.5 second pause */
	const std::chrono::milliseconds PAUSE_500(500);
	/*! 0.3 second pause */
	const std::chrono::milliseconds PAUSE_300(300);
	/*! 0.05 second pause */
	const std::chrono::milliseconds PAUSE_50(50);
	/*! 0.002 second pause */
	const std::chrono::milliseconds PAUSE_2(2);
	/*! 0.001 second pause */
	const std::chrono::milliseconds PAUSE_1(1);

	/*! Arithmetic time-type capable of representing time 0 */
	const time_t TIMET_0 = 0;
	/*! Arithmetic time-type capable of representing time 45 */
	const time_t TIMET_45 = 45;
	/*! Arithmetic time-type capable of representing time 60 */
	const time_t TIMET_60 = 60;


}
/*! @}*/
#endif // GLOBAL_CONSTANTS_H_
