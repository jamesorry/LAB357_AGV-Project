#ifndef _DIFF_PID_CONTROLLER_H_
#define _DIFF_PID_CONTROLLER_H_

#include "hmi.h"

/* PID setpoint info For a Motor */
typedef struct {
  double TargetTicksPerFrame;    // target speed in ticks per frame 目标转速
  long Encoder;                  // encoder count 编码器计数
  long PrevEnc;                  // last encoder count 上次的编码器计数

  /*
  * Using previous input (PrevInput) instead of PrevError to avoid derivative kick,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
  */
  int PrevInput;                // last input
  //int PrevErr;                   // last error

  /*
  * Using integrated term (ITerm) instead of integrated error (Ierror),
  * to allow tuning changes,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  */
  //int Ierror;
  int ITerm;                    //integrated term

  long output;                    // last motor setting
}
SetPointInfo;

void resetPID();
void doPID(SetPointInfo * p);
void updatePID();
#endif