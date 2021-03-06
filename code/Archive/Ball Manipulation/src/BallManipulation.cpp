/*
 * BallManipulation.cpp
 *
 *  Created on: Feb 6, 2016
 *      Author: Elaine
 */
#include "WPILib.h"
#include "BallManipulation.h"

#define PIVOT_BALL_SPEED 0.5
#define PIVOT_DEFENSE_SPEED 0.5
#define BALL_MOTOR_SPEED 0.75
#define CHANGED_PIVOT_MOTOR_SPEED 0.5
#define STOP_PLACE 55


BallManipulation::BallManipulation(int ballMotorChannel, int pivotMotorChannel, int pivotEncoderChannelA,
		int pivotEncoderChannelB, int topLSChannel, int bottomLSChannel) {
	ballMotor = new CANTalon(ballMotorChannel); // is actually a victor
	pivotMotor = new CANTalon(pivotMotorChannel);
	pivotMotor->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	pivotEncoder = new Encoder(pivotEncoderChannelA, pivotEncoderChannelB);
	topLS = new DigitalInput(topLSChannel);
	bottomLS = new DigitalInput(bottomLSChannel);
	resetted = false;
}

BallManipulation::~BallManipulation(void) {
	delete ballMotor;
	delete pivotMotor;
	delete pivotEncoder;
	delete topLS;
	delete bottomLS;
}

void BallManipulation::ChangeSpeed(void) {
	// changes speed of pivot mechanism
	float currentSpeed = pivotMotor->Get();
	if (currentSpeed == PIVOT_DEFENSE_SPEED || currentSpeed == (PIVOT_BALL_SPEED)) {
		pivotMotor->Set(CHANGED_PIVOT_MOTOR_SPEED);
	} else if (currentSpeed == (-PIVOT_DEFENSE_SPEED) || currentSpeed == (-PIVOT_BALL_SPEED)) {
		pivotMotor->Set(-CHANGED_PIVOT_MOTOR_SPEED);
	}
}

void BallManipulation::PivotBall(void) {
	// pivots intake mechanism to a point, uses encoder

	printf("\n encoder: %d", GetPivotEncoder());
	if (resetted == false && GetBottomLS() == true) {
		pivotMotor->Set(-PIVOT_BALL_SPEED);
	}
	else if (resetted == false && GetBottomLS() == false) {
		pivotMotor->Set(0.0);
		pivotEncoder->Reset();
		resetted = true;
		pivotMotor->Set(PIVOT_BALL_SPEED);
	}
	else if (abs(GetPivotEncoder()) >= STOP_PLACE && resetted == true) {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::PullIn(void) {
	// pull in ball
	ballMotor->Set(-BALL_MOTOR_SPEED);
}

void BallManipulation::PushOut(void) {
	// push out ball
	ballMotor->Set(BALL_MOTOR_SPEED);
}

void BallManipulation::DefenseUp(void) {
	// pivot pivotMotor forward if top limit switch not pressed
	if (GetTopLS() == true) {
		pivotMotor->Set(PIVOT_DEFENSE_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::DefenseDown(void) {
	// pivot pivotMotor backwards if bottom limit switch not pressed
	if (GetBottomLS() == true) {
		pivotMotor->Set(-PIVOT_DEFENSE_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::ResetPivotEncoder(void) {
	// resets pivot encoder if bottom limit switch pressed
	if (GetBottomLS() == false) {
		pivotEncoder->Reset();
	}
}

bool BallManipulation::GetTopLS(void) {
	// return top limit switch value
	return topLS->Get();
}

bool BallManipulation::GetBottomLS(void) {
	// return bottom limit switch value
	return bottomLS->Get();
}

int BallManipulation::GetPivotEncoder(void) {
	// return pivot encoder value
	return pivotEncoder->Get();
}

void BallManipulation::StopPivotMotor(void) {
	// stop the pivot motor
	pivotMotor->Set(0.0);
}

void BallManipulation::StopBallMotor(void) {
	// stop the ball motor
	ballMotor->Set(0.0);
}


