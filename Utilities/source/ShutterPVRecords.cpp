#include <ShutterPVRecords.h>

namespace ShutterRecords
{
	/*Defined as std::string const so that the value of the variable cannot be changed.*/

	/*! Used to search for "Open" key in pv_record_map */
	std::string const Open = "Open";
	/*! Used to search for "Close" key in pv_record_map */
	std::string const Close = "Close";
	/*! Used to search for "Sta" key in pv_record_map */
	std::string const State = "State";
	/* CMI i sthe interlock bits */
	std::string const Cmi = "Cmi";

	std::vector<std::string> shutterRecordList = { Open, Close, State, Cmi };
}