/*       CargoManip.cpp
*   created on: Jan 19, 2019
*    by: Sandhya, Isha, GC2    */

#include "Settings.h"
#include "frc/WPILib.h" 
#include <iostream> 
#include <frc/smartdashboard/SmartDashboard.h>
#include "CargoManip.h"
#include "ctre/Phoenix.h" 
#include "hal/Constants.h"
#include <math.h>
#include <frc/Encoder.h>
#include <jankyXboxJoystick.h>

#define MOTOR_ROLL_F_SPEED -1.0   //roller intake speed
#define MOTOR_ROLL_R_SPEED 1.0   //roller outtake speed
#define MOTOR_PIVOT_F_SPEED -1.0  //mech out of bot speed
#define MOTOR_PIVOT_R_SPEED 1.0 //mech in bot speed
#define MOTOR_STOP_SPEED 0.0  // stops motor
#define ENCODER_COUNTS_PER_REVOLUTION 4096

CargoManip::CargoManip(int motorRollChannel, int motorPivotChannel){
  motorRoll = new WPI_VictorSPX(motorRollChannel);
  pivotMotor = new WPI_TalonSRX(motorPivotChannel);

  //pid initialization for pivot motor
  kTimeoutMs = 50;
  kPIDLoopIdx = 0;
  encoderCount = 0.0;
	encoderAngle = 0.0;
  //desiredAngle = 0;
  desiredAnglePulses = 0;

  int absolutePosition = pivotMotor -> GetSelectedSensorPosition(0);
  pivotMotor -> SetSelectedSensorPosition(absolutePosition, kPIDLoopIdx, kTimeoutMs);
	pivotMotor -> ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx, kTimeoutMs);
	pivotMotor -> SetSensorPhase(false);
  pivotMotor -> ConfigNominalOutputForward(0, kTimeoutMs);
	pivotMotor -> ConfigNominalOutputReverse(0, kTimeoutMs);
	pivotMotor -> ConfigPeakOutputForward(0.8, kTimeoutMs); //going in? test
	pivotMotor -> ConfigPeakOutputReverse(-0.8, kTimeoutMs);
 
	pivotMotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs); //not using feedforward
	pivotMotor->Config_kP(kPIDLoopIdx, 0.1, kTimeoutMs); //p val: 0.01 (tune)
	pivotMotor->Config_kI(kPIDLoopIdx, 0, kTimeoutMs); //i val: 0
	pivotMotor->Config_kD(kPIDLoopIdx, 0, kTimeoutMs); //d val: 0 (use if needed while tuning p)
  pivotMotor -> SelectProfileSlot(0, kPIDLoopIdx); //kpidloopidx = pidloopidx?

  pivotMotor -> Set(ControlMode::Position, desiredAnglePulses);

  //pivotMotor-> ConfigSelectedFeedbackSensor(Analog, 0, 0);
  //limSwitchInside = new frc::DigitalInput(limSwitchInsideChannel);
  //limSwitchOutside = new frc::DigitalInput(limSwitchOutsideChannel);
  //encoderRoll = new frc::Encoder();
  //encoderPivot = new frc::Encoder();
}

CargoManip::~CargoManip(){
  delete motorRoll;
  delete pivotMotor;
  //delete limSwitchInside;
  //delete limSwitchOutside;
  //delete encoderRoll;
  //delete encoderPivot;
}


/*void CargoManip::ButtonVals(){

}*/

void CargoManip::RollersIn(){
  motorRoll -> Set(MOTOR_ROLL_F_SPEED);
}

void CargoManip::RollersOut(){
  motorRoll -> Set(MOTOR_ROLL_R_SPEED);
}

void CargoManip::RollersStop(){
  motorRoll -> Set(MOTOR_STOP_SPEED);
}

/*int CargoManip::GetLimSwitchInside(){
  return limSwitchInside -> Get();
}

int CargoManip::GetLimSwitchOutside(){
  return limSwitchOutside -> Get();
}*/

/*void CargoManip::CargoMechOutRobot(){
  pivotMotor -> Set(MOTOR_PIVOT_F_SPEED);
  cargoMechExtended = true;
  cargoMechGoingForward = true;
  cargoMechGoingBackward = false;
}

void CargoManip::CargoMechInRobot(){
  pivotMotor -> Set(MOTOR_PIVOT_R_SPEED);
  cargoMechExtended = false;
  cargoMechGoingForward = false;
  cargoMechGoingBackward = true;
}*/

/*void CargoManip::CargoMechStop(){
  pivotMotor -> Set(MOTOR_STOP_SPEED);
  cargoMechExtended = false;
  cargoMechGoingForward = false;
  cargoMechGoingBackward = false;
}*/

/*void CargoManip::CargoMechStopWithLimSwitch(){
	if ((GetLimSwitchOutside()==1) && cargoMechGoingForward) {
		CargoMechStop();
	}
	else if ((GetLimSwitchInside()==1) && cargoMechGoingBackward) {
		CargoMechStop();
	}
}*/

/*bool CargoManip::GetCargoMechPosition(){
  return cargoMechExtended;
}*/

void CargoManip::FindEncoderCount(){
  pivotMotor -> SetSelectedSensorPosition(0, 0, 10);
  double encoderCount = pivotMotor -> GetSensorCollection().SetQuadraturePosition(0, 10);
  frc::SmartDashboard::PutNumber("Current Encoder Count:", encoderCount);
}

void CargoManip::FindEncoderAngle(){
  FindEncoderCount();
  float encoderAngle = ((encoderCount / ENCODER_COUNTS_PER_REVOLUTION) * 360);
  frc::SmartDashboard::PutNumber("Current Pivot Angle:", encoderAngle);
}

void CargoManip::SetPIDAngle(float pivotangle){
  desiredAnglePulses = ((pivotangle / 360) * ENCODER_COUNTS_PER_REVOLUTION);
  pivotMotor -> Set(ControlMode::Position, desiredAnglePulses);
  frc::SmartDashboard::PutNumber("Desired Pivot Angle:", pivotangle);
}

void CargoManip::CargoMechIn(){
  FindEncoderAngle();
  if (encoderAngle >= 60){ //current angle should be 0 from view, 90 zeroed 
      SetPIDAngle(20);
  }
  else if (encoderAngle >= 10 && encoderAngle < 60){ //current angle should be 70, 20 zeroed
      SetPIDAngle(0);
  }
}

void CargoManip::CargoMechOut(){
  FindEncoderAngle();
    if (encoderAngle < 15){ //currently angle should be 90 from view, 0 zeroes
      SetPIDAngle(20);
    }
    else if (encoderAngle >= 15 && encoderAngle < 85){ //current angle should be 70
      SetPIDAngle(90);
  }
}

/*void CargoManip::StartInit(){
  pivotMotor -> GetSensorCollection().SetQuadraturePosition(0,10);

}*/

float CargoManip::GetHatchPanelDistance(){
  return (pivotMotor -> GetSensorCollection().GetAnalogIn());
}