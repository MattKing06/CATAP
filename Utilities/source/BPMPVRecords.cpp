#include "BPMPVRecords.h"


std::string const BPMRecords::X= "X";
std::string const BPMRecords::Y= "Y";
std::string const BPMRecords::RA1 = "RA1";
std::string const BPMRecords::RA2 = "RA2";
std::string const BPMRecords::RD1 = "RD1";
std::string const BPMRecords::RD2 = "RD2";
std::string const BPMRecords::SA1 = "SA1";
std::string const BPMRecords::SA2 = "SA2";
std::string const BPMRecords::SD1 = "SD1";
std::string const BPMRecords::SD2 = "SD2";
std::string const BPMRecords::AWAK = "AWAK";
std::string const BPMRecords::RDY = "RDY";
std::string const BPMRecords::DATA = "DATA";

namespace BPMRecords
{
	std::vector<std::string> bpmRecordList = { X,Y,RA1,RA2,RD1,RD2,SA1,SA2,SD1,SD2,AWAK,RDY,DATA };
	std::vector<std::string> bpmRecordListVirtual = { X,Y,RA1,RA2,RD1,RD2,SA1,SA2,SD1,SD2,DATA };
}

