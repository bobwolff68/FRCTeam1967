///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyActuator for a simple robot for 2013

#ifndef _JANKYACTUATOR_H
#include "jankyTask.h"
#include "WPILib.h"
#define _JANKYACTUATOR_H

#define CYCLE_TIME 3500
#define ACTUATION_TIME 1200
#define PISTON_CHANNEL 1

class JankyActuator : public JankyTask	{
public:
	JankyActuator(void);
	virtual ~JankyActuator(void);
	
	void Go();
	virtual void Run();
	
	//Member variables
	bool bActuating;
	Timer cycleTimer;
	Solenoid * pPiston;
	
};

#endif
