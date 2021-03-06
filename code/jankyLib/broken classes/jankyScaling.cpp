/*
 * jankyScaling.cpp
 *
 *  Created on: Feb 6, 2016
 *      Author: Gillian
 */
#include "WPILib.h"
#include "jankyScaling.h"
#include <math.h>
#include "jankyTask.h"
#include "jankyEncoder.h"
#include <cmath>

#define SCALING_WINDS -15					//Scaling winch needs to turn 360 degrees exactly 16 times
#define FW_WIND_SPEED -1.0
#define BW_WIND_SPEED 1.0
#define RELEASE_TIME 1.0

//Constructor
jankyScaling::jankyScaling(int encoderChannelA, int encoderChannelB, int motorChannelA, int motorChannelB, int pistonChannel)
{
	piston = new Solenoid(19, pistonChannel);
	printf("About to set piston to false\n");

	motorEncoder = new JankyEncoder(encoderChannelA, encoderChannelB, motorChannelA, motorChannelB);
	printf("Created new motorEncoder\n");
	printf("Set revolution\n");
	ScalingStart();

}

//Destructor
jankyScaling::~jankyScaling()
{
	//piston->Set(false);
	delete piston;
	delete motorEncoder;

	piston = NULL;
	motorEncoder = NULL;
}

void jankyScaling::ScalingStart(){
	piston->Set(false);
	motorEncoder->Reset();
	motorEncoder->setRevolution(SCALING_WINDS);
	// motorEncoder->SetMaxTime(4.0);
	motorEncoder->Start();
	motorEncoder->motorGo();
}

void jankyScaling::Release(){
	printf("Releasing piston now\n");
	piston->Set(true);												//Pushing out piston to hit the CAM; no need to bring it back in we believe
}

void jankyScaling::LiftUp()											//pulling the robot up
{
	motorEncoder->setSpeed(BW_WIND_SPEED);
	motorEncoder->setRevolution(SCALING_WINDS - abs(motorEncoder->pEncoder->Get()/360));
	motorEncoder->motorGo();
	motorEncoder->Go();
}

void jankyScaling::DropDn()											//extend the robot towards down
{
	motorEncoder->setSpeed(FW_WIND_SPEED);
	motorEncoder->startMotor();
	//motorEncoder->setRevolution(abs((motorEncoder->pEncoder->Get())/360));
	//motorEncoder->motorGo();
	//motorEncoder->ReverseGo();
}

void jankyScaling::StopWU(){
	motorEncoder->stopMotor();											//Find a way to stop the motor without having to reset——stopMotor isn't working bc Go is overriding it?
	printf("Stopping wind\n");
}


void jankyScaling::ManualLiftUp()
	{
		motorEncoder->setSpeed(BW_WIND_SPEED);
		motorEncoder->startMotor();
	}

void jankyScaling::ManualDropDown()
	{
		motorEncoder->setSpeed(FW_WIND_SPEED);
		motorEncoder->startMotor();
	}

