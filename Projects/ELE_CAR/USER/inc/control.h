#ifndef __COUNTROL_H__
#define __COUNTROL_H__

void PID_Calculate(float, float, float, float, float);
void Servo_Control_By_Error(float);
void Go(float, unsigned int, unsigned int);

#endif