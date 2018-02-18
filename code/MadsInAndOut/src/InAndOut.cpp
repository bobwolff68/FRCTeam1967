#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "InAndOut.h"
#include "math.h"
#include "jankyTask.h"

//Motor speeds
#define MOTOR_CLAW_F_SPEED 0.5
#define MOTOR_CLAW_R_SPEED -0.5
#define MOTOR_ROLL_F_SPEED 0.5
#define MOTOR_ROLL_R_SPEED -0.5
#define MOTOR_STOP_SPEED 0.0

//For distance per pulse in in/out mechanism's encoder
#define CLAW_PULSES_PER_REVOLUTION 4096
#define CLAW_CIRCUMFERENCE 0.399 * M_PI
#define CLAW_DISTANCE_PER_PULSE CLAW_CIRCUMFERENCE/CLAW_PULSES_PER_REVOLUTION

//Hysteresis for in/out mechanism
#define IO_HYSTERESIS_POS 0.001
#define IO_HYSTERESIS_NEG -0.001

InAndOut::InAndOut(int pistonDoorLeftChannel, int pistonDoorRightChannel, int motorRollChannel, int motorClawChannel){
	pistonDoorLeft = new Solenoid(9, pistonDoorLeftChannel);
	pistonDoorRight = new Solenoid(9, pistonDoorRightChannel);

	motorRoll = new WPI_TalonSRX(motorRollChannel);

	motorClaw = new WPI_TalonSRX(motorClawChannel);
	motorClaw -> ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	motorClaw -> SetSelectedSensorPosition(0, 0, 10);
	motorClaw -> GetSensorCollection().SetQuadraturePosition(0,10);

	motorClaw->ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 6, 0);
	motorClaw->ConfigReverseLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 6, 0);


	//UNUSED
	//	clawEncoder = new Encoder(clawEncoderChannel1, clawEncoderChannel2);
	//	clawEncoder -> SetDistancePerPulse(CLAW_DISTANCE_PER_PULSE);
	//	pistonInOut1 = new Solenoid(9, pistonInOut1Channel);
	//	pistonInOut2 = new Solenoid(9, pistonInOut2Channel);
	//	limSwitchInside = new DigitalInput(limSwitchInsideChannel);
	//	limSwitchOutside = new DigitalInput(limSwitchOutsideChannel);

}
InAndOut::~InAndOut(){
	delete motorClaw;
	delete motorRoll;
	delete pistonDoorLeft;
	delete pistonDoorRight;

	//  Unused
	//	delete limSwitchInside;
	//	delete limSwitchOutside;
}

void InAndOut::PistonDoorOpen(){
	pistonDoorLeft->Set(false); //Retract both pistons simultaneously
	pistonDoorRight->Set(false);
}

void InAndOut::PistonDoorClose(){
	pistonDoorLeft->Set(true); //Engage both pistons simultaneously
	pistonDoorRight->Set(true);
}

void InAndOut::MotorRollForward() {
	motorRoll -> Set(MOTOR_ROLL_F_SPEED); //Make the rollers go forwards
}

void InAndOut::MotorRollReverse() {
	motorRoll -> Set(MOTOR_ROLL_R_SPEED); //Make the rollers go backwards
}

void InAndOut::MotorRollStop() {
	motorRoll -> Set(MOTOR_STOP_SPEED); //Stop the rollers
}

void InAndOut::OutsideDistance() {
	desiredDistanceToMove = 0.25;
}

void InAndOut::InsideDistance() {
	desiredDistanceToMove = 0.0;
}

/*
void InAndOut::MoveClawMechanism() {
	amountToMoveClaw = desiredDistanceToMove - GetEncoderDistance();

	if (amountToMoveClaw > IO_HYSTERESIS_POS) {
		MotorClawOutOfRobot();
	}
	else if (amountToMoveClaw < IO_HYSTERESIS_NEG) {
		MotorClawIntoRobot();
	}
	else if ((amountToMoveClaw < IO_HYSTERESIS_POS) && (amountToMoveClaw > IO_HYSTERESIS_NEG)) {
		MotorClawStop();
	}
}
*/

int InAndOut::GetLimSwitchInside(){
	//	return limSwitchInside->Get(); //get value (true/false) of limit switch
	return motorClaw ->GetSensorCollection().IsFwdLimitSwitchClosed();
}

int InAndOut::GetLimSwitchOutside(){
	//	return limSwitchOutside->Get(); //get value (true/false) of limit switch
	return motorClaw ->GetSensorCollection().IsRevLimitSwitchClosed();
}

void InAndOut::MotorClawStop() {
	motorClaw -> Set(MOTOR_STOP_SPEED); //Stop the motors on the claw mechanism
	clawGoingForward = false;
	clawGoingBackward = false;
}

void InAndOut::MotorClawOutOfRobot(){
	motorClaw->Set(MOTOR_CLAW_F_SPEED); ///Spin the motors on the claw mechanism forward, making the claw go out of the robot
	clawGoingForward = true;
	clawGoingBackward = false;
	clawPositionIsOut = true;
}

void InAndOut::MotorClawIntoRobot(){
	motorClaw->Set(MOTOR_CLAW_R_SPEED); //Spin the motors on the claw mechanism backward, making the claw go out of the robot
	clawGoingBackward = true;
	clawGoingForward = false;
	clawPositionIsOut = false;
}

void InAndOut::MotorClawStopWithLimSwitches(){
	if ((GetLimSwitchOutside()==1) && clawGoingForward) {
		MotorClawStop();
	}
	else if ((GetLimSwitchInside()==1) && clawGoingBackward) {
		MotorClawStop();
	}
}

double InAndOut::GetEncoderDistance() {
	clawEncoderCount = motorClaw->GetSensorCollection().GetQuadraturePosition();
	clawEncoderDistance = (clawEncoderCount/CLAW_PULSES_PER_REVOLUTION)*CLAW_CIRCUMFERENCE;
	return clawEncoderDistance;
}

bool InAndOut::GetClawPosition() {
	return clawPositionIsOut;
}

void InAndOut::MotorClawMoveInAndOut() {
	if (clawPositionIsOut) {
		MotorClawOutOfRobot();
	}
	else if (clawPositionIsOut == false){
		MotorClawIntoRobot();
	}
}

void InAndOut::MoveClawDownInAuto(){
	MotorClawOutOfRobot();
	if (GetLimSwitchOutside() == 1) {
		MotorClawStop();
		needsToPutDownClaw = false;
	}
}

void InAndOut::StartUpInit() {
	clawEncoderCount = 0;
	clawEncoderDistance = 0;
	clawGoingForward = false;
	clawGoingBackward = false;
	needsToPutDownClaw = true;
}

void InAndOut::Run() {
	//Put the claw mechanism down once during auto
	if (needsToPutDownClaw) {
		MoveClawDownInAuto();
	}

	//Emergency stop the claw depending on what limit switches are pressed
	MotorClawStopWithLimSwitches();
}

//UNUSED
/*
void InAndOut::ClawDown() {
	pistonInOut1 -> Set(true);
	pistonInOut2 -> Set(true);
}

void InAndOut::ClawUp() {
	pistonInOut1 -> Set(false);
	pistonInOut2 -> Set(false);
}
double InAndOut::GetClawEncoderDistance(){
	return clawEncoder->GetDistance();
}

void InAndOut::ResetClawEncoder(){
	clawEncoder -> Reset();
}

double InAndOut::GetClawEncoderDistancePerPulse() {
	return clawEncoder -> GetDistancePerPulse();
}
 */
