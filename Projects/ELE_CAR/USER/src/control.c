#include "main.h"
#include "pins.h"
#include <MATH.H>
float adcL, adcR, sumOfError, lastError, output;
uint16_t midDutyOfServo = 730, maxChangeDuty = 130;

void PID_Calculate(float error, float kp, float ki, float kd, float maxAbsI)
{
	sumOfError += error;
	if (sumOfError > maxAbsI)
		sumOfError = maxAbsI;
	else if (sumOfError < -maxAbsI)
		sumOfError = -maxAbsI;
	output = kp * error +
			 ki * sumOfError +
			 kd * (error - lastError);
	if (output > 1)
		output = 1;
	else if (output < -1)
		output = -1;
	lastError = error;
}

void Servo_Control_By_Error(float errorInner)
{
	if (errorInner > 1)
		errorInner = 1;
	else if (errorInner < -1)
		errorInner = -1;
	int duty = midDutyOfServo + maxChangeDuty * errorInner;
	PWM_SET_Frequency(SERVO, 50, duty);
}

void Go(uint16_t minSpeed, uint16_t maxSpeed)
{
	int speed = (int)((maxSpeed - minSpeed) * abs(lastError) / 2) + minSpeed;
	if(speed > maxSpeed)
		speed = maxSpeed;
	else if (speed < minSpeed)
		speed = minSpeed;
	PWM_SET_Duty(MOTOR_L, 10000 - speed);
	PWM_SET_Duty(MOTOR_R, 10000 - speed);
}