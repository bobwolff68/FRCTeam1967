///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyAutonomous State Machine - 2014 example for state machine

#ifndef _JANKYAUTONOMOUSSTATE_H
#define _JANKYAUTONOMOUSSTATE_H
#include "frc/WPILib.h"
#include "jankyStateMachine.h"
#include "jankyRobotTemplate.h"
#include "jankyFoxLiftState.h"

#define DRIVE_SIDEWAYS_TIME 1.0
#define DRIVE_FORWARD_TIME 0.2
#define TURN_TIME 0.75
#define DRIVE_TO_AUTO_TIME 1.0
#define FORKLIFT_TIME 2.0
#define BINGULATE_PISTON 5
#define BINGULATE_SERVO 9
#define DRIVE_BACKWARD_TIME 2.2//was 2.2//2.8
#define BINGULATE_TIME 3.0//3.5
#define MOVE_RIGHT_TIME 0.5
class JankyFoxliftState;

class JankyAutonomousState : public JankyStateMachine	{
public:
	JankyAutonomousState(frc::RobotDrive * pt, JankyFoxliftState * foxlift);
	virtual ~JankyAutonomousState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		HugIdle,
		BingulateDown,
		BingulateWait,
		BingulateUp,
		DriveBackward,
		RollersIn,
		MoveRightandRollIn,
		DriveSideways,
		DriveForward,
		DownTote,
		LiftTote,
		LowerTote,
		TurnToAuto,
		DriveToAuto,
		BingulateEnd,
		End,
		TheRealEnd,
		
	};
	frc::Timer * driveForwardTimer;
	frc::Timer * driveSidewaysTimer;
	frc::Timer * turnTimer;
	frc::Timer * driveToAutoTimer;
	frc::Timer * driveBackwardTimer;
	frc::Timer * forkliftTimer;
	frc::Timer*bingulateTimer;
	frc::Timer*moveRightTimer;
	frc::Solenoid*binPiston;
	frc::Servo*binServo;
	frc::RobotDrive * ptRobot;
	JankyFoxliftState * ptFoxLift;
	bool driveOnce;
	bool goDownOnce;
	int timesRepeated;
	float extraBinTime;
	
	//Member functions
	void RetractBinPiston();
	void ExtendBinPiston();
	void BinServoSetStart();
	void BinServoSetEnd();
	void StateEngine(int curState);
	void GoForBox(void);
	void GoForHug(void);
	void StartBinAuto();
	void GoSideways(void);
	void GoForward(void);
	void GoLiftTote(void);
	void StartAuto(void);
	void GoTurnToAuto(void);
    void GoForwardToAuto(void);
    void BringInRollers(void);

};

#endif

