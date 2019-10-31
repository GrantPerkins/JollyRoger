#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port2,           left,          tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port3,           leftFollow,    tmotorVex393, openLoop)
#pragma config(Motor,  port4,           right,         tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port5,           rightFollow,   tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

void drive(int l, int r) {
	motor[right] = r;
	motor[rightFollow] = r;
	motor[left] = l;
	motor[leftFollow] = l;
}

int max(int a, int b) {
	if (a>b) {
		return a;
	}
	return b;
}

int min(int a, int b) {
	if (a<b) {
		return a;
	}
	return b;
}

int absolute(int x) {
	if (x < 0) {
		return -x;
	}
	return x;
}

void pDrive(int heading){
	float ENC_PER_INCH = 360. / (4.*3.14);
	const float kP = 1.5;
	const int threshold = 10;
	int target = ENC_PER_INCH * (heading);
	int startLeft = nMotorEncoder(left);
	int startRight = nMotorEncoder(right);
	float error = 0;
	int correct = 0;
	int iter = 0;
	const int ZERO_VELOCITY = 100;
	const int MAX_ITER = 50;

	// the 'while' is here to ensure velocity = 0, so we don't drift too far.
	while (correct < ZERO_VELOCITY) {
		iter++;
		int errorRight = target + (startRight - nMotorEncoder(right));
		int errorLeft = target - (startLeft - nMotorEncoder(left));
		errorRight = max(min(kP * errorRight, 127), -127);
		errorLeft = max(min(kP * errorLeft, 127), -127);
		drive(errorLeft, errorRight*.65);
		if (absolute(errorLeft) < threshold+5 && absolute(errorRight) < threshold) {
			correct++;
		} else {
			correct = 0;
		}
	}
}

void turnTo(float heading) {
	const int ENC_PER_ROTATION = 2450;
	const float kP = 1.5;
	const int threshold = 5;
	int target = ENC_PER_ROTATION * (heading / 360);
	int startLeft = nMotorEncoder(left);
	int startRight = nMotorEncoder(right);
	float error = 0;
	int correct = 0;
	int iter = 0;
	const int ZERO_VELOCITY = 50;
//	const int MAX_ITER = 1000;

	// the 'while' is here to ensure velocity = 0, so we don't drift too far.
	while (correct < ZERO_VELOCITY) {
		iter++;
		error = target - (startRight - nMotorEncoder(right)) - (startLeft - nMotorEncoder(left));
		error = max(min(kP * error, 64), -64);
		drive(error, -error);
		if (absolute(error) < threshold) {
			correct++;
		} else {
			correct = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{

//const int target = 1000;
	while (true) {
		pDrive(54);
		pDrive(-4);
		turnTo(26);
		pDrive(-32);
		turnTo(-43);
		pDrive(33);
		turnTo(12);

		wait1Msec(500);
		wait1Msec(10000);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	// User control code here, inside the loop

	while (true)
	{
	  // This is the main execution loop for the user control program. Each time through the loop
	  // your program should update motor + servo values based on feedback from the joysticks.

	  // .....................................................................................
	  // Insert user code here. This is where you use the joystick values to update your motors, etc.
	  // .....................................................................................

	  UserControlCodePlaceholderForTesting(); // Remove this function call once you have "real" code.
	}
}
