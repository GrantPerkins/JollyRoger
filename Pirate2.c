#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port4,           right,         tmotorVex393HighSpeed_MC29, openLoop, driveRight, encoderPort, I2C_2)
#pragma config(Motor,  port5,           rightFollow,   tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port2,           left,          tmotorVex393HighSpeed_MC29, openLoop, driveLeft, encoderPort, I2C_1)
#pragma config(Motor,  port3,           leftFollow,    tmotorVex393_MC29, openLoop, driveLeft)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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

void turnTo(float heading) {
	const int ENC_PER_ROTATION = 2400;
	const int kP = 1.0;
	const int threshold = 10;
	int target = ENC_PER_ROTATION * (heading / 360);
	int startLeft = 0;
	int startRight = 0;
	int error = 0;
	int correct = 0;

	// the 'while' is here to ensure velocity = 0, so we don't drift too far.
	while (correct < 4) {
		error = target - (startRight - nMotorEncoder(right)) - (startLeft - nMotorEncoder(left));
		error = min(max(kP * error, 127), -127);
		drive(error, -error);
		if (error < threshold) {
			correct++;
		} else {
			correct = 0;
		}
	}
}

task main()
{
	if (vexRT[Btn8D]) {
		turnTo(90);
	} else {
		drive(vexRT[Ch3], vexRT[Ch2]);
	}

}
