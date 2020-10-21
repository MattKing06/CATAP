#ifndef SHUTTER_H_
#define SHUTTER_H_

#include <Hardware.h>

#include "EPICSShutterInterface.h"



#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

class Shutter;
struct ShutterState;
class EPICSShutterInterface;

typedef boost::shared_ptr<EPICSShutterInterface> EPICSShutterInterface_sptr;



struct ShutterState
{   // proviude a default constructor
	ShutterState() :
		name(GlobalConstants::DUMMY_NAME),
		state(STATE::UNKNOWN),
		Cmi(GlobalConstants::int_min)
	{};
	std::string name;
	STATE state;
	int Cmi;
	std::map<std::string, STATE>  interlock_states;
	boost::python::dict interlock_states_Py;
};


class Shutter : public Hardware
{
	public:
		/*! Default constructor call for Shutter object*/
		Shutter();
		/*! Custom constructor for Shutter object
			@param[in] paramMap strings defining parameters extracted from YAML config files
			@param[in] mode Defines the STATE of Shutter we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)			*/
		Shutter(const std::map<std::string, std::string>& paramMap, STATE mode);

		/*! Copy constructor for Shutter object
			@param[in] copyShutter references to Shutter to be copied*/
		Shutter(const Shutter& copyShutter);
		~Shutter();

		
		/*! Get the ShutterState (structured data with shutter name and latest, state, Cmi, and interlock bitmap
		@param[out] ShutterState structured data */
		ShutterState getShutterState()const;
		
		/*! Get the ShutterState data in a python dictionary
		@param[out] dict, dictionary of shutter state date, keyed by the name of each data type */
		boost::python::dict getShutterStateDictionary()const;

		/*! checks whether the current shutter_state is STATE::CLOSED.
		@param[out] bool true if shutter_state is STATE::CLOSED, false if shutter_state is not STATE::CLOSED */
		bool isClosed() const;
		/*! checks whether the current shutter_state is STATE::OPEN.
		@param[out] bool true if shutter_state is STATE::OPEN, false if shutter_state is not STATE::OPEN */
		bool isOpen() const;

		/*! get the state of the shutter.
		@param[out] STATE, one of OPEN, CLOSED, ERR or UNKNOWN*/
		STATE getState()const;

		/*! Try to open the shutter.
		@param[out] bool, true if command got sent to EPICS, (!!does not indicate whether the shutter opened succesfully!!)*/
		bool close();
		/*! Try to close the shutter.
		@param[out] bool, true if command got sent to EPICS, (!!does not indicate whether the shutter closed succesfully!!)*/
		bool open();

		/*! get the cmi.
		@param[out] bool, true if command got sent to EPICS, (does not indicate whether the shutter opened succesfully)*/
		int getCMI()const;

		/*! get a map of each cmi bit state, keyed by the interlock name (defiend in th emasterlattice yaml file. 
		@param[out] bool, true if command got sent to EPICS, (does not indicate whether the shutter opened succesfully)*/
		std::map<std::string, STATE>  getCMIBitMap()const;
		boost::python::dict  getCMIBitMap_Py()const;
		/*! get the name alises for this shutter
			@param[out] names, vector contianing all the alias names */
		std::vector<std::string> getAliases() const;
		/*! get the name alises for this shutter, Python version
			@param[out] names, list contianing all the alias names */
		boost::python::list getAliases_Py() const;

		/*! Get a reference to this shutter
			@param[out] Shutter objet reference */
		Shutter& getShutter();


		/*! Compare the energy interlock to STATE::GOOD (energy interlock name is hardcoded, and should match master lattice name.) 
			@param[out] bool, true if state is GOOD, otherwise false  */
		bool isEnergyInterlockGood()const;
		/*! Compare the energy interlock to STATE::BAD (energy interlock name is hardcoded, and should match master lattice name.)
			@param[out] bool, true if state is BAD, otherwise false  */
		bool isEnergyInterlockBad()const;

		/*! Compare the charge interlock to STATE::BAD (charge interlock name is hardcoded, and should match master lattice name.)
			@param[out] bool, true if state is GOOD, otherwise false  */
		bool isChargeInterlockGood()const;
		/*! Compare the charge interlock to STATE::BAD (charge interlock name is hardcoded, and should match master lattice name.)
			@param[out] bool, true if state is BAD, otherwise false  */
		bool isChargeInterlockBad()const;

		/*! Compare both the PS GLA, GLB interlocks to STATE::GOOD (names are hardcoded, and should match master lattice name.)
			@param[out] bool, true if both states are GOOD, otherwise false  */
		bool isPSInterlockGood()const;
		/*! Compare both the PS GLA, GLB interlocks to STATE::BAD (names are hardcoded, and should match master lattice name.)
			@param[out] bool, true if either states are BAD, otherwise false  */
		bool isPSInterlockBad()const;



		/*! enable debug-messages for this shutter 	*/
		void debugMessagesOn();
		/*! disbale debug0messages for this shutter */
		void debugMessagesOff();
		/*! enable messages for this shutter */
		void messagesOn();
		/*! disable messages for this shutter */
		void messagesOff();


		friend class EPICSShutterInterface;
		friend class ShutterFactory;
	protected:

		std::pair<epicsTimeStamp, STATE > shutter_state;
		std::pair<epicsTimeStamp, int > cmi;


	private:
		std::vector<int>  cmiBits;
		std::map<std::string, STATE>  cmi_bit_map;



		void setPVStructs();
		EPICSShutterInterface_sptr epicsInterface;
		std::map<std::string, std::string> ShutterParamMap;
		/*! alternative names for the shutter (usually shorter than the full PV root), defined in the master lattice yaml file	*/
		std::vector<std::string> aliases;
		std::vector<std::string> interlock_bit_names;
};


#endif //SHUTTER_H_