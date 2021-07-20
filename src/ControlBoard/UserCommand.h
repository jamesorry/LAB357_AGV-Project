#ifndef _USER_COMMAND_H_
#define	_USER_COMMAND_H_

#include "Arduino.h"

typedef struct __CMD {
  const char* cmd;
  void (*func)(void);
} CMD, *PCMD;

void resetArduino(void);
void getMicros(void);
void getAdc(void);
void getGpio(void);
void setGpio(void);
void echoOn(void);
void echoOff(void);
void cmd_CodeVer(void);
void showHelp(void);
bool getNextArg(String &arg);
void cmdOutput(void);
void cmdInput(void);
void cmdTest(void);
void cmdSend(void);
void cmdGetSetADC();
void cmdCarFront();
void cmdCarBack();
void cmdCarLeft();
void cmdCarRight();
void cmdCarStop();
void cmdCarStart();
void cmdCarError();

void UserCommand_Task(void);
void UserCommand_Timer(void);
#endif //_USER_COMMAND_H_
