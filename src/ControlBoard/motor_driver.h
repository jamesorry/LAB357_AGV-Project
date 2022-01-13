#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include "hmi.h"
#define MOTOR_DRIVER_ENABLE 0
#define RIGHT_MOTOR_ENABLE 1
#define LEFT_MOTOR_ENABLE 2
#define DIRA 3
#define DIRB 4

#define PWMA 8
#define PWMB 10


void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);


#endif
