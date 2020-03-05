#include <IMG.h>
#include <IMGPVRecords.h>
IMG::IMG()
{
}

IMG::IMG(const std::map<std::string, std::string>& paramMap, STATE mode) : Hardware(paramMap, mode),
state(std::make_pair(epicsTimeStamp(), STATE::ERR)),
pressure(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min))
{

}

IMG::IMG(const IMG& copyIMG)
{
}

IMG::~IMG()
{
}

void IMG::setPVStructs()
{
	for (auto&& record : IMGRecords::imgRecordList)
	{

	}
}

void IMG::debugMessagesOn()
{
}

void IMG::debugMessagesOff()
{
}

void IMG::messagesOn()
{
}

void IMG::messagesOff()
{
}
