#ifndef _ENCODER_DRIVER_H_
#define _ENCODER_DRIVER_H_

#include "hmi.h"
#define INT_LEFT_A 0
#define INT_LEFT_B 1
#define INT_RIGHT_A 4
#define INT_RIGHT_B 5

#define LEFT_A 2
#define LEFT_B 3
#define RIGHT_A 19
#define RIGHT_B 18

void initEncoders();
void leftEncoderEventA();
void leftEncoderEventB();
void rightEncoderEventA();
void rightEncoderEventB();

long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();

#endif
