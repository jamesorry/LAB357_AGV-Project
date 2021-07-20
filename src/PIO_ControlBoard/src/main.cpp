#include <Arduino.h>
#include "UserCommand.h"
#include "EEPROM_Function.h"
#include "MainProcess.h"
#include "hmi.h"
#include "Timer.h"

HardwareSerial *cmd_port;

extern MainDataStruct maindata;
extern RuntimeStatus runtimedata;

void setup() {
    Serial.begin(9600);
	cmd_port = &CMD_PORT;
	cmd_port->begin(CMD_PORT_BR);
    Serial1.begin(115200);
    Serial3.begin(115200);
	READ_EEPROM();
	MainProcess_Init();
	cmd_port->println("TestMaindataValue: " + String(maindata.TestMaindataValue));
	cmd_port->println("TestRuntimedataValue: " + String(runtimedata.TestRuntimedataValue));
	buzzerPlay(500);
    TimerInit(1, 10000);
}

void loop() {
	UserCommand_Task();
	MainProcess_Task();
	if(runtimedata.UpdateEEPROM)
	{
		runtimedata.UpdateEEPROM = false;
		WRITE_EEPROM(); //maindata內的值都會寫到EEPROM
		delay(1000);
	}
}
ISR(TIMER1_COMPA_vect)
{
}
