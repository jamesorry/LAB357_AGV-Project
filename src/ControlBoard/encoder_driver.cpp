#include "encoder_driver.h"

volatile long left_count = 0L;
volatile long right_count = 0L;

void initEncoders(){
    pinMode(LEFT_A, INPUT_PULLUP); // 2  --- 0
    pinMode(LEFT_B, INPUT_PULLUP); // 3  --- 1
    pinMode(RIGHT_A, INPUT_PULLUP);// 19  --- 4
    pinMode(RIGHT_B, INPUT_PULLUP);// 18  --- 5

    attachInterrupt(INT_LEFT_A, leftEncoderEventA, CHANGE);
    attachInterrupt(INT_LEFT_B, leftEncoderEventB, CHANGE);
    attachInterrupt(INT_RIGHT_A, rightEncoderEventA, CHANGE);
    attachInterrupt(INT_RIGHT_B, rightEncoderEventB, CHANGE);
}
void leftEncoderEventA(){
    if(digitalRead(LEFT_A) == HIGH){
        if(digitalRead(LEFT_B) == HIGH){
            left_count++;
        } else {
            left_count--;
        }
    } else {
        if(digitalRead(LEFT_B) == LOW){
            left_count++;
        } else {
            left_count--;
        }
    }
}
void leftEncoderEventB(){
    if(digitalRead(LEFT_B) == HIGH){
        if(digitalRead(LEFT_A) == LOW){
            left_count++;
        } else {
            left_count--;
        }
    } else {
        if(digitalRead(LEFT_A) == HIGH){
            left_count++;
        } else {
            left_count--;
        }
    }
}
void rightEncoderEventA(){
    if(digitalRead(RIGHT_A) == HIGH){
        if(digitalRead(RIGHT_B) == HIGH){
            right_count++;
        } else {
            right_count--;
        }
    } else {
        if(digitalRead(RIGHT_B) == LOW){
            right_count++;
        } else {
            right_count--;
        }
    }  
}
void rightEncoderEventB(){
    if(digitalRead(RIGHT_B) == HIGH){
        if(digitalRead(RIGHT_A) == LOW){
            right_count++;
        } else {
            right_count--;
        }
    } else {
        if(digitalRead(RIGHT_A) == HIGH){
            right_count++;
        } else {
            right_count--;
        }
    }  
}

long readEncoder(int i) {
    if (i == LEFT) return left_count;
    else return right_count;
}

void resetEncoder(int i) {
    if (i == LEFT){
        left_count = 0L;
        return;
    } else { 
        right_count = 0L;
        return;
    }
}
void resetEncoders() {
    resetEncoder(LEFT);
    resetEncoder(RIGHT);
}

