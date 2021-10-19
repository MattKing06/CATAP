#include "ValvePVRecords.h"


std::string const ValveRecords::OPEN = "Open";
std::string const ValveRecords::CLOSE = "Close";
std::string const ValveRecords::STA = "Sta";


namespace ValveRecords
{
	std::vector<std::string> valveRecordList = {OPEN,CLOSE,STA};
}

