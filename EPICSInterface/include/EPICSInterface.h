#ifndef EPICS_INTERFACE_H
#define EPICS_INTERFACE_H
#include <LoggingSystem.h>
#include <Hardware.h>
#include <functional>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#include <epicsTime.h>
#endif
#ifndef PV_H_
#include "PV.h"
#endif

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
	{							\
		ca_message(status);		\
		SEVCHK(status, NULL);   \
		exit(status);			\
	}							\
}								\



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
		void removeSubscription(pvStruct& pv);
		void removeChannel(pvStruct& pv);
		void detachFromContext();
		static std::string getEPICSTime(const epicsTimeStamp& stamp);
		static void setPVTimeStampFromArgs(pvStruct& pv, const struct event_handler_args args);
		
		#ifndef __CINT__
			ca_client_context *thisCaContext;
			void attachTo_thisCAContext();
			void detachFrom_thisCAContext();
			void addILockChannels(const int numILocks,
				const std::string& pvRoot,
				const std::string& objectName
				// map_ilck_pvstruct& ILockPVStructs,
				);
			template<typename T>
			void putValue(const pvStruct& pvStruct, const T& value) const
			{
				if (ca_state(pvStruct.CHID) == cs_conn)
				{
					int status = ca_put(pvStruct.CHTYPE, pvStruct.CHID, &value);
					MY_SEVCHK(status);
					status = ca_pend_io(CA_PEND_IO_TIMEOUT);
					MY_SEVCHK(status);
				}
			}
		#endif
			/*IS THIS BETTER THAN SPECIFIC FUNCTIONS????*/
			template<typename returnType> inline static returnType returnValueFromArgsAs(const struct event_handler_args args) { return(returnType)(args.dbr); }
		template<> inline static std::string returnValueFromArgsAs<std::string>(const struct event_handler_args args)
		{
			if (args.status != ECA_NORMAL)
			{
				std::cout << "Something went wrong with the update function!" << std::endl;
			}
			auto timeObject = (const struct dbr_time_string*)(args.dbr);
			return std::string(timeObject->value);
		}
		template<> inline static double returnValueFromArgsAs<double>(const struct event_handler_args args)
		{
			if (args.status != ECA_NORMAL)
			{
				std::cout << "Something went wrong with the update function!" << std::endl;
			}
			auto timeObject = (const struct dbr_time_double*)(args.dbr);
			return double(timeObject->value);
		}
		template<> inline static STATE returnValueFromArgsAs<STATE>(const struct event_handler_args args)
		{
			if (args.status != ECA_NORMAL)
			{
				std::cout << "Something went wrong with the update function!" << std::endl;
			}
			auto timeObject = (const struct dbr_time_enum*)(args.dbr);
			return STATE(timeObject->value);
		}
		template<> inline static long returnValueFromArgsAs<long>(const struct event_handler_args args)
		{
			if (args.status != ECA_NORMAL)
			{
				std::cout << "Something went wrong with the update function!" << std::endl;
			}
			auto timeObject = (const struct dbr_time_long*)(args.dbr);
			return long(timeObject->value);
		}
		template<> inline static float returnValueFromArgsAs<float>(const struct event_handler_args args)
		{
			if (args.status != ECA_NORMAL)
			{
				std::cout << "Something went wrong with the update function!" << std::endl;
			}
			auto timeObject = (const struct dbr_time_float*)(args.dbr);
			return float(timeObject->value);
		}
		template<typename hardwareType>
		static hardwareType* getHardwareFromArgs(const struct event_handler_args args)
		{
			return static_cast<hardwareType*>(args.usr);
		}
	protected:
		bool shouldStartEpics = true;
		bool shouldStartVirtualMachine = true;
		LoggingSystem messenger;
		unsigned short EPICS_ACTIVATE, EPICS_SEND, EPICS_RESET;

		// some other stuff might be needed here, need to check interface.h from VELA-CLARA Controllers
};
#endif

