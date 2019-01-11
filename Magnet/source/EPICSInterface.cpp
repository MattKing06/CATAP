#include "EPICSInterface.h"

EPICSInterface::EPICSInterface()
{
	//shouldStartEpics and shouldStartVirtualMachine are true by default

	//setup logging system for EPICSInterface: DebugOn(true), MessagingOn(true)
	EPICSInterface::messaging = LoggingSystem(true, true);
}

EPICSInterface::EPICSInterface(bool& startEpics, bool& startVirtualMachine)
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	EPICSInterface::messaging = LoggingSystem(true, true);
}

EPICSInterface::EPICSInterface(bool& startEpics, bool& startVirtualMachine, LoggingSystem& messager)
{
	EPICSInterface::shouldStartEpics = startEpics;
	EPICSInterface::shouldStartVirtualMachine = startVirtualMachine;
	EPICSInterface::messaging = messager;
}