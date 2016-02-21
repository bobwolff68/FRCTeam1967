#include "WPILib.h"
#include "JankyScaling.h"
#include "JankyTask.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "jankyEncoder.h"

#define DRIVE_JOYSTICK_PORT 1
#define GC_JOYSTICK_PORT 0
#define SCALING_ENCODER_CHANNELA 0		//will be 2 in final bot
#define SCALING_ENCODER_CHANNELB 1		//will be 3 in final bot
#define SCALING_MOTOR_CHANNELA 1		//will be 7 in final bot
#define SCALING_MOTOR_CHANNELB 2		//will be 8 in final bot
#define SCALING_PISTON_CHANNEL 1

class Robot:public IterativeRobot{

	jankyDrivestick*drivestick;			//creating pointer for jankyDrivestick to be called drivestick
	jankyXboxJoystick*joystick;			//creating pointer for jankyXboxJoystick to be called joystick
	jankyScaling*scaling;				//creating pointer for jankyScaling to be called scaling

public:

	Robot(){
			drivestick = NULL;
			joystick = NULL;
			scaling = NULL;
		}
	~Robot(){
			delete drivestick;
			delete joystick;
			delete scaling;

			drivestick = NULL;
			joystick = NULL;
			scaling = NULL;
		}

private:
	void RobotInit(){
		printf("Made it to RobotInit\n");
		drivestick = new jankyDrivestick(DRIVE_JOYSTICK_PORT);
		joystick = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		scaling = new jankyScaling(SCALING_ENCODER_CHANNELA, SCALING_ENCODER_CHANNELB, SCALING_MOTOR_CHANNELA, SCALING_MOTOR_CHANNELB, SCALING_PISTON_CHANNEL);
	}

	void TeleopInit(){
		printf("Made it to TeleopInit\n");
		scaling->ScalingStart();
	}

	void TeleopPeriodic(){
		if (joystick->GetButtonX() == true){			//scaling Release; when button A on the Xbox controller is pressed, Release will be enabled
			printf("Going to Release now\n");
			scaling->Release();
		}
		if (joystick->GetButtonY() == true){			//scaling WindUp; when button B on the Xbox controller is pressed, WindUp will be enabled
			printf("Going to Wind Up now\n");
			scaling->WindUp();
		}
		if (joystick->GetButtonB() == true){
			printf("Stopping WindUp\n");
			scaling->StopWU();
		}
	}
};

START_ROBOT_CLASS(Robot)