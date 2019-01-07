///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Targeting for Rebound Rumble 2012

#ifndef _JANKYTURRET_H
#define _JANKYTURRET_H

#include "jankyRobot.h"

class JankyTurret : public frc::Victor {
public:
	JankyTurret(int JagPort, int LimLeft, int LimRight );
	virtual ~JankyTurret(void);
	
	//Jaguar TurretMotor;
	//Victor TurretMotor;
	frc::DigitalInput TurretLeft;
	frc::DigitalInput TurretRight;
	float Deadband;
	
	virtual void PIDWrite(float writeval) { Set(writeval); };
	virtual void Set(float DesiredMotorValue);
	void SetDeadband(float DB);
	
};

#endif
