#include "motor_driver.h"

void initMotorController(){
    pinMode(OutputPin[DIRA], OUTPUT);
    pinMode(ADC_PWMPin[PWMA], OUTPUT);
    pinMode(OutputPin[DIRB], OUTPUT);
    pinMode(ADC_PWMPin[PWMB], OUTPUT);
}
void setMotorSpeed(int i, int spd){
    unsigned char reverse = 0;
    if (spd < MIN_PWM)
    {
        spd = -spd;
        reverse = 1;
    }
    if (spd > MAX_PWM)
        spd = MAX_PWM;

    if (i == LEFT) { 
        if (reverse == 0) { 
            digitalWrite(OutputPin[DIRA], HIGH);
        } else if (reverse == 1) { 
            digitalWrite(OutputPin[DIRA], LOW);
        }
        analogWrite(ADC_PWMPin[PWMA], spd);
    } else /*if (i == RIGHT) //no need for condition*/ {
        if (reverse == 0) { 
            digitalWrite(OutputPin[DIRB], LOW);        
        } else if (reverse == 1) { 
            digitalWrite(OutputPin[DIRB], HIGH);
        }
        analogWrite(ADC_PWMPin[PWMB], spd);
    }
}
void setMotorSpeeds(int leftSpeed, int rightSpeed){
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
}

