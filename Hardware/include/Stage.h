#ifndef STAGE_H_
#define STAGE_H_

#include <Hardware.h>
#include <EPICSStageInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <GlobalFunctions.h>
#include <boost/make_shared.hpp>
#include <StagePVRecords.h>

class EPICSStageInterface;

typedef boost::shared_ptr<EPICSStageInterface> EPICSStageInterface_sptr;
/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control charge diagnostics on VELA/CLARA.
	 A collection of these classes will be stored in map in the ChargeFactory class.
 */
class Stage : public Hardware
{
public:
	Stage();
	Stage(const std::map<std::string, std::string>& paramMap, STATE mode);
	Stage(const Stage& copyStage);
	~Stage();
	void setPVStructs();
	EPICSStageInterface_sptr epicsInterface;
	std::map<std::string, std::string> StageParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	std::pair<epicsTimeStamp, double> getCurrentPosition();
	void setCurrentPosition(std::pair<epicsTimeStamp, double> newPosition);
	std::pair<epicsTimeStamp, double> getPositionSetpoint();
	void setPositionSetpoint(std::pair<epicsTimeStamp, double> newSetpoint);
	void moveOverRange(double startPosition, double endPosition, int numberOfSteps);
	void setNewPositions(std::vector<double> positions);
	void setNewPosition(double newPosition);
	bool canStageMove(double newPosition);
	bool moveToDevice(const std::string& device);
	bool isMoving();
	bool isAtDevice(const std::string& device);
	std::vector<std::string> getDevices();
	boost::python::list getDevices_Py();
	bool isReadPositionEqualToSetPosition();
	double getMinPosition();
	double getMaxPosition();
	double getInPosition();
	double getOutPosition();
	size_t getStageNumber();
	bool doesHaveYag();
	size_t getPrecision();
	void updateAliases(const std::string& alias);
	std::vector<std::string> getAliases();
	std::string getFullNameFromAlias(const std::string& alias);
	std::map<std::string, double> getDevicesAndPositions();
	float getDevicePosition(const std::string& device);
	bool clearForBeam();

	friend class EPICSStageInterface;

private:
	std::pair<epicsTimeStamp, double> currentPosition;
	std::pair<epicsTimeStamp, double> positionSetpoint;
	std::vector<std::string> aliases;
	bool moving;
	bool atDevice;
	double minPosition;
	double maxPosition;
	double inPosition;
	double outPosition;
	size_t stageNumber;
	bool hasYag;
	size_t precision;
	std::map<std::string, double> deviceAndPositionMap;
};

//namespace StageDevices
//{
//	extern std::string const CLEAR_FOR_BEAM;
//	extern std::string const YAG_UP;
//	extern std::string const YAG_DOWN;
//	extern std::string const YAG_RECTICLE;
//	extern std::string const COLL_D2_5MM;
//	extern std::string const COLL_D2MM;
//	extern std::string const COLL_D1_5MM;
//	extern std::string const COLL_D1MM;
//	extern std::string const OTR;
//	extern std::string const GAS_JET;
//	extern std::string const DCP;
//	extern std::string const DTH;
//	extern std::string const YAG_DCP_5x5;
//	extern std::string const V_SLIT;
//	extern std::string const H_SLIT;
//	extern std::string const EO_CRYSTAL;
//	extern std::string const EO_YAG;
//	extern std::vector<std::string> devices;
//}
#endif //STAGE_H_
/**\copydoc Hardware*/
/**@}*/