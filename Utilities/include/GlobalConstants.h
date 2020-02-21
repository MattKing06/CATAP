#ifndef GLOBAL_constANTS_H_
#define GLOBAL_constANTS_H_
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

namespace GlobalConstants 
{
	const double double_min = std::numeric_limits<double>::min();
	const double double_max = std::numeric_limits<double>::max();

	const double zero_double = 0.0;
	const double one_double = 1.0;
	const double zero_point_one_double = 0.1;

	const int zero_int= 0;
	const int one_int = 1;

	const unsigned short zero_ushort = 0;
	const unsigned short one_ushort = 1;

	//const epicsUInt16 zero_epicsUInt16= 0;
	//const epicsUInt16 one_epicsUInt16 = 1;


	


	const size_t zero_sizet = 0;
	const size_t one_sizet = 1;
	const size_t eighty_sizet = 80;


	const std::chrono::milliseconds STANDARD_PAUSE(2000);
	const std::chrono::milliseconds PAUSE_1000(1000);
	const std::chrono::milliseconds PAUSE_300(300);
	const std::chrono::milliseconds PAUSE_500(500);
	const std::chrono::milliseconds PAUSE_50(50);
	const std::chrono::milliseconds PAUSE_1(1);
	const std::chrono::milliseconds PAUSE_2(2);

	const time_t TIMET_0 = 0;
	const time_t TIMET_45 = 45;
	const time_t TIMET_60 = 60;

	const std::string COMMENT = "COMMENT";
	const std::string TIMESTAMP = "TIMESTAMP";
	const std::string HEADER = "HEADER";
	const std::string FILE_ALIAS = "FILE_ALIAS";

	const std::string DBURT_FILEPATH = "\\\\fed.cclrc.ac.uk\\org\\NLab\\ASTeC\\Projects\\VELA\\Snapshots\\DBURT\\";
	const std::string DUMMY_NAME = "DUMMY_NAME";


}
#endif // GLOBAL_constANTS_H_
