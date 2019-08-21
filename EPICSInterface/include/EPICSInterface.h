#ifndef EPICS_INTERFACE_H
#define EPICS_INTERFACE_H
#include "LoggingSystem.h"
// EPICS include
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#ifndef __CINT__
#include <cadef.h>
#include <epicsTime.h>
#endif
#ifndef PV_H_
#include "PV.h"
#endif

class PV;
#define CA_PEND_IO_TIMEOUT 5.0
class EPICSInterface
{
	public:
		EPICSInterface();
		EPICSInterface(const bool& shouldStartEpics, const bool& shouldStartVirtualMachine);
		EPICSInterface(const bool& shouldStartEpics, const bool& shouldStartVirtualMachine, LoggingSystem& messaging);
		~EPICSInterface();
		double get_CA_PEND_IO_TIMEOUT() const;
		void set_CA_PEND_IO_TIMEOUT(double value);
		void retrieveCHID(pvStruct &pvStruct) const;
		void retrieveCHTYPE(pvStruct &pvStruct) const;
		void retrieveCOUNT(pvStruct &pvStruct) const;
		void createSubscription(Hardware &hardware, pvStruct &pvStruct) const;
		static double getEPICSTime(const epicsTimeStamp& stamp);
	protected:
		bool shouldStartEpics = true;
		bool shouldStartVirtualMachine = true;
		LoggingSystem messenger;
		unsigned short EPICS_ACTIVATE, EPICS_SEND, EPICS_RESET;

		// some other stuff might be needed here, need to check interface.h from VELA-CLARA Controllers

	#ifndef __CINT__
		ca_client_context *thisCaContext;
		void attachTo_thisCAContext();
		void detachFrom_thisCAContext();
		void addILockChannels(const int numILocks,
			const std::string& pvRoot,
			const std::string& objectName
			// map_ilck_pvstruct& ILockPVStructs,
			);
		template<typename T> T getDBR(const event_handler_args args)
		{
			T dbr_holder;
			dbr_holder = *(T*)(args.dbr);
			return dbr_holder;
		}
	#endif
};
#endif