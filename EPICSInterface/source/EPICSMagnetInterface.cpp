#include "EPICSMagnetInterface.h"

Magnet* EPICSMagnetInterface::owner = NULL;

EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{
}
void EPICSMagnetInterface::updateCurrent(struct event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cerr << "Something went wrong with update function!" << std::endl;
	}
	else if (args.type == DBR_DOUBLE)
	{
		EPICSMagnetInterface::owner = static_cast<Magnet*>(args.usr);
		owner->setCurrent(*(double*)(args.dbr));
	}

}