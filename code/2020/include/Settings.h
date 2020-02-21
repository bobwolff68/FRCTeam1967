//#define PROG_BOT  //COMMENT THIS OUT IF NOT TESTING ON PROGRAMMING BOT
#define JANKYCHASSIS

//Cameras
#define DRIVE_TEAM_CAM_1 //COMMENT THIS OUT IF CAMERA IS NOT ON BOT
//#define DRIVE_TEAM_CAM_2 //COMMENT THIS OUT IF CAMERA IS NOT ON BOT

// Ports of joysticks & sensors
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define COLOR_SENSOR_PORT 0x52

//Chassis
#ifdef PROG_BOT
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 3
#define REAR_RIGHT_MOTOR_CHANNEL 4
#endif

#ifdef JANKYCHASSIS
#define SHOOTING_LEFT_MOTOR_CHANNEL 4
#define INTAKE_LEFT_MOTOR_CHANNEL 0
#define SHOOTING_RIGHT_MOTOR_CHANNEL 1
#define INTAKE_RIGHT_MOTOR_CHANNEL 3
#endif

// Vision
#define VISION_DISTANCE "Distance to Tape"
#define VISION_OFFSET "Offset"
#define NO_DATA_DEFAULT -1000

//Intake
#define MOTOR_ROLL_CHANNEL 10 //change the channel accordingly 
#define LEFT_PISTON_CHANNEL 0
#define RIGHT_PISTON_CHANNEL 1
//Intake to turret transportation
#define CONVEYOR_BELT_CHANNEL 2
#define BRIDGE_CHANNEL 9

// Turret
#define TURRET_MOTOR_CHANNEL 7
#define LOWER_BOUND -3
#define UPPER_BOUND 3
//#define TURRET_USING_VISION

// Shooting
#define FLYWHEEL_CHANNEL 5