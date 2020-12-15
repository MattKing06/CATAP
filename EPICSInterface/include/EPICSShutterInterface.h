#ifndef EPICS_SHUTTER_INTERFACE_H_
#define EPICS_SHUTTER_INTERFACE_H_
#include <EPICSInterface.h>

class EPICSShutterInterface : public EPICSInterface
{
	public:
		EPICSShutterInterface();
		EPICSShutterInterface(const EPICSShutterInterface& copyInterface);
		~EPICSShutterInterface();
		std::string ownerName;

		/*! sends a pulse (1,0) to the PV associated with the state provided
		* @param[in] value : The state we want to set the valve to (OPEN,CLOSED)
		* @param[in] value : The pvStruct associated with the valve state PV*/
		static void updateShutterState(const struct event_handler_args args);

		static void updateShutterCmi(const struct event_handler_args args);

		void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;

	private:
		/*! messenger object for EPICSValveInterface messaging. */
		static LoggingSystem messenger;

};

#endif //EPICS_SHUTTER_INTERFACE_H_