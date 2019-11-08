#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    pot1,           sensorPotentiometer)
#pragma config(Sensor, in2,    pot2,           sensorPotentiometer)
#pragma config(Sensor, dgtl1,  limit,          sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           left,          tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port3,           leftFollow,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           right,         tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port5,           rightFollow,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           arm1,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           arm2,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           claw,          tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

float getLeftEnc() {
	return ((float)nMotorEncoder(left)/-2800.0)*99.0;
}

float getRightEnc() {
	return ((float)nMotorEncoder(right)/2800.0)*99.0;
}

enum Position{
	kSilver,
	kGold,
	kDump,
	kNone
};

//Math Stuff
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

//Driving Controls
void drive(int l, int r) {
	motor[right] = r;
	motor[rightFollow] = r;
	motor[left] = l;
	motor[leftFollow] = l;
}

int pDrive(int inches, float initLeft, float initRight) {
	const int kP = 5.0;
	float leftError = inches - (getLeftEnc() - initLeft);
	float rightError = inches - (getRightEnc() - initRight);
	drive(min(107,max(-107, kP * leftError)), kP * rightError);
	return (leftError + rightError) / 2.0;
}

float degreesToUnits(float degrees) {
	const float inchesPerTurn = 83.0;
	return (degrees / 360.0) * inchesPerTurn;
}

float turnTo(float target, float initLeft, float initRight) {
	int kP = 22.5;
	target = degreesToUnits(target);
	float error = kP * (target - ((getLeftEnc() - initLeft) - (getRightEnc() - initRight)));
	drive(error, -error);
	return error;
}

void armTo(Position current){
	float kP = 0.4;
	float error1 = 0;
	float error2 = 0;
	int target = 0;
	int silverTarget = 0;
	int goldTarget = 700;
	int backTarget = 2000;
	bool limitSpeed = false;
	float value;
	float avgError;
	float speed = 0;


		switch (current){
			case kSilver:
				target = silverTarget;
				break;
			case kGold:
				target = goldTarget;
				break;
			case kDump:
				target = backTarget;
				limitSpeed = true;
				break;
			default:
				break;
		}
		error1 = SensorValue(pot1);
		error2 = SensorValue(pot2);
		value = (error1 + error2)/2.0;
		avgError = target - value;

		// speed will be relative to effect of gravity and offset from target
		speed = kP * avgError;

		motor[arm1] = speed;
		motor[arm2] = speed;
}

void closeClaw(){
	motor[claw] = 30;
}

void openClaw() {
	if (!SensorValue(limit))
		motor[claw] = -40;
	else
		motor[claw] = 0;
}

Position getInputPosition() {
	if (vexRT[Btn8U])
		return kDump;
	if (vexRT[Btn8D])
		return kSilver;
	if (vexRT[Btn8L] || vexRT[Btn8R])
		return kGold;
	return kNone;
}

enum ClawState {
	kOpen,
	kClose
};

ClawState getClawState() {
	if (vexRT[Btn6U])
		return kOpen;
	if (vexRT[Btn5U])
		return kClose;
	return kNone;
}

void pre_auton()
{
	bStopTasksBetweenModes = true;
}

/*

Auto template

	// TURNING
	target = REPLACE_ME;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = turnTo(target, initLeft, initRight);
		if (absolute(error) < 5)
			correct++;
		else
			correct = 0;
	}


	// DRIVING
	target = REPLACE_ME;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
	}

*/

task autonomous()
{
	float target;
	float initLeft = getLeftEnc();
	float initRight = getRightEnc();
	int correct = 0;
	float error;

	// Drive 66, open claw, raise arm
	target = 58.0;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
		armTo(kGold);
		openClaw();
	}

	// Close claw
	drive(0,0);
	closeClaw();
	wait1Msec(500);

	// Drive -4
	target = -15.0;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
		armTo(kGold);
		closeClaw();
	}

	// Turn 30
	target = 30;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = turnTo(target, initLeft, initRight);
		if (absolute(error) < 5)
			correct++;
		else
			correct = 0;
		armTo(kGold);
	}

	// Drive -32
	target = -30;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
		armTo(kGold);
		closeClaw();
	}

	// Arm dump
	clearTimer(T1);
	while (time1[T1] < 1000) {
		armTo(kDump);
		closeClaw();
	}
	openClaw();
	wait1Msec(500);



	// Turn -43
	target = 30;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = turnTo(target, initLeft, initRight);
		if (absolute(error) < 5)
			correct++;
		else
			correct = 0;
		armTo(kGold);
	}
	/*

	// Drive 33
	target = REPLACE_ME;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
	}

	// Turn 12
	// Drive -30
	target = REPLACE_ME;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
	}

	// Turn 45
	// Drive 45
	target = REPLACE_ME;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
	}

	// Turn -40
	// Drive 60
	target = REPLACE_ME;
	error = 100;
	initLeft = getLeftEnc();
	initRight = getRightEnc();
	correct = 0;
	while (correct < 50) {
		error = pDrive(target, initLeft, initRight);
		if (absolute(error) < .5)
			correct++;
		else
			correct = 0;
	}
	*/
}



task usercontrol() {
	Position currentPosition = kNone;
	Position inputPosition = kNone;
	ClawState currentClaw = kNone;
	ClawState inputClaw = kNone;
	while (true) {
		//teleop drive
		drive(vexRT[Ch3], vexRT[Ch2]);

		// teleop 4 bar
		inputPosition = getInputPosition();
		if (inputPosition != (int)kNone) {
				currentPosition = inputPosition;
		}
		if (currentPosition != (int)kNone) {
			armTo(currentPosition);
		}

		//teleop claw
		inputClaw = getClawState();
		if (inputClaw != (int)kNone) {
			currentClaw = inputClaw;
		}
		if (currentClaw == (int)kOpen)
			openClaw();
		else if (currentClaw == (int)kClose)
			closeClaw();
	}
}
