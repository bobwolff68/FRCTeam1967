/*
 *
 * jankyAutonomousState.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Team1967
 */

#include "WPILib.h"
#include "jankyAutonomousState.h"
#include "jankyFoxLiftState.h"

/*
 * Constructor
 * 
*/
JankyAutonomousState::JankyAutonomousState(RobotDrive * pt, JankyFoxliftState * foxLift)
{
	//printf("JankyAutonomousState() constructor\n");
	ptRobot = pt;
	ptFoxLift = foxLift;
	driveOnce = false;
	goDownOnce = false;
	SetMachineName("JankyAutonomousStateMachine");
	printf("Set machine name\n");
	
	SetName(Idle,"Idle");
	SetName(BinIdle, "Idle for Bingulate");
	SetName(DriveSideways, "Just drive sideways");
	SetName(DriveForward,"Drive forward");
	SetName(DownTote, "Down tote");
	SetName(LiftTote, "Lift tote");
	SetName(TurnToAuto, "Turn toward auto");
	SetName(DriveToAuto, "Drive toward auto");
	SetName(End, "End - STOPPPPPP");
	printf("Set state names\n");
	
	driveForwardTimer = new Timer();
	driveSidewaysTimer = new Timer();
	binTimer = new Timer();
	driveBackTimer = new Timer();
	turnTimer = new Timer();
	driveToAutoTimer = new Timer();
	driveForwardTimer->Reset();
	driveSidewaysTimer->Reset();
	turnTimer->Reset();
	driveToAutoTimer->Reset();

	binPiston = new Solenoid(BIN_PISTON);

	printf("Newed/reset drive timer\n");
	
	//Starting JankyTask at end of constructor
	Start();
}

/*
 * Destructor
 */
JankyAutonomousState::~JankyAutonomousState()
{
	delete driveSidewaysTimer;
	delete driveForwardTimer;
	delete turnTimer;
	delete driveToAutoTimer;
	delete binTimer;
	delete driveBackTimer;
	delete binPiston;
}

void JankyAutonomousState::ExtendBingulate() {
	binPiston = true;
}
void JankyAutonomousState::RetractBingulate() {
	binPiston = false;
}
void JankyAutonomousState::StartAuto(){
	NewState(Idle, "starting and going in idle");
}
void JankyAutonomousState::Go()
{
	printf("Go() called\n");
	//GoSideways();
	NewState(DownTote, "Down for tote");
}

void JankyAutonomousState::GoSideways()
{
	printf("GoSideways() called\n");
	if (GetCurrentState() == Idle)
	{
		if(driveSidewaysTimer)
		{
			driveSidewaysTimer->Start();
			NewState(DriveSideways,"driveSidewaysTimerStart driving sideways");
		}
	}
	else
		printf("Can't go sideways\n");
}

void JankyAutonomousState::GoForward()
{
	printf("GoForward() called\n");
	if (GetCurrentState() == DriveSideways)
	{
		if (driveForwardTimer)
		{
			driveForwardTimer->Start();
			NewState(DriveForward,"Start driving forward");
		}
	}
	else
		printf("Can't go forward\n");
}

void JankyAutonomousState::GoLiftTote()
{
	printf("LiftTote() called\n");
	if (GetCurrentState() == DriveForward)
	{
		NewState(StateValue::LiftTote,"Start lifting tote");
	}
	else
		printf("Can't lift totes\n");
}

void JankyAutonomousState::GoTurnToAuto()
{
	printf("TurnToAuto() called\n");
	if (GetCurrentState() == LiftTote)
	{
		turnTimer->Start();
		NewState(StateValue::TurnToAuto,"Start turning to face auto zone");
	}
	else
		printf("Can't turn to auto zone\n");
}

void JankyAutonomousState::GoForwardToAuto()
{
	printf("DriveForwardToAuto() called\n");
	if (GetCurrentState() == TurnToAuto)
	{
		driveToAutoTimer->Start();
		NewState(StateValue::DriveToAuto,"Start driving to auto zone");
	}
	else
		printf("Can't drive to auto zone\n");
}

void JankyAutonomousState::StateEngine(int curState)
{
	switch(curState)
	{
		case Idle:
			ptFoxLift->GoMid();
			//Idle handled by GoForward() or StartAuto()
			printf("In Idle\n");
			break;
		case BinIdle:
			ExtendBingulate();
			if(ptFoxLift->!IsLSwitchTopClosed()) {
				foxlift
			}
			else if (ptFoxLift->IsLSwitchTopClosed()){
				NewState(Bingulate, "Limit switch top pressed");
			}
			break;
		case Bingulate:
			ptFoxLift->StopLift();
			RetractBingulate();
			binTimer->Reset();
			if(binTimer > BINGULATE_TIME) {
				NewState(DriveBackToAuto, "Bingulate done");
			}
			break;
		case DriveBackToAuto:
			if(driveBackTimer->Get() >= DRIVE_BACKWARD_TIME) {
				//NewState(End, "Ending Autonomous");
				NewState(Unbingulate,"Done driving backward");
			}
			else {
				ptRobot->MecanumDrive_Cartesian(0.0,0.5,0.0,0.0);
			}
			break;
		case Unbingulate:
			ptFoxLift->GoDown();
			ptRobot->MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);
			break;
		case DriveSideways:
			//printf("In state DriveSideways\n");
			//if(driveSidewaysTimer->Get() >= DRIVE_SIDEWAYS_TIME)
			//{
				//GoForward();
				//NewState(DriveForward, "Done driving sideways");
			//}
			//else
			//{
				ptRobot->MecanumDrive_Cartesian(-1.0,0.05,0.0,0.0);//fine tune the .05 when have chance
			//}
			break;
		case DriveForward:
			printf("In state DriveForward\n");
			if(driveForwardTimer->Get() >= DRIVE_FORWARD_TIME)
			{
				NewState(End, "Ending Autonomous");
				//NewState(LiftTote,"Done driving forward");
			}
			else
			{
				ptRobot->MecanumDrive_Cartesian(0.0,-0.5,0.0,0.0);
			}
			break;
		case DownTote:
			if(ptFoxLift->GetCurrentState()== JankyFoxliftState::StateValue::BottomStop){
				NewState(LiftTote, "tote up and turning");
			}
			if (!goDownOnce)
			{
				ptFoxLift->GoDown();
				goDownOnce = true;
			}


			break;
		case LiftTote:
			ptFoxLift->NewState(JankyFoxliftState::StateValue::Braking, "lifting tote");
			ptFoxLift->GoMid();
			if(ptFoxLift->IsLSwitchMidClosed()){
				turnTimer->Start();
				NewState(TurnToAuto, "tote up and turning");
			}
			//NewState(TurnToAuto,"Done lifting tote");
			break;
		case TurnToAuto:
			printf("In state TurnToAuto\n");
			if(turnTimer->Get() >= TURN_TIME)
			{
				GoForwardToAuto();
				//NewState(DriveToAuto,"Done driving forward");
			}
			else
			{
				ptRobot->MecanumDrive_Cartesian(0.0,0.0,1.0,0.0);
			}
			break;
		case DriveToAuto:
			printf("In state DriveToAuto\n");
			if(driveToAutoTimer->Get() >= DRIVE_TO_AUTO_TIME)
			{
				NewState(End,"Done driving to auto zone");
			}
			else
			{
				ptRobot->MecanumDrive_Cartesian(0.0,-1.0,0.0,0.0);
			}
			break;
		case End:
			if(ptRobot)
			{
				ptRobot->MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);
				Terminate();
			}
			break;
		default:
			printf("error current state = %d\n",curState);
			break;
	}
}
