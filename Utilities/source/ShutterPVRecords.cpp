#include <ShutterPVRecords.h>


std::string const ShutterRecords::Open = "Open";
std::string const ShutterRecords::Close = "Close";
std::string const ShutterRecords::State = "State";
std::string const ShutterRecords::Cmi = "Cmi";

std::vector<std::string> ShutterRecords::shutterRecordList = { ShutterRecords::Open, ShutterRecords::Close, ShutterRecords::State, ShutterRecords::Cmi };
