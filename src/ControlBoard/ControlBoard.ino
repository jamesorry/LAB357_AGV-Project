#include "UserCommand.h"
#include "EEPROM_Function.h"
#include "MainProcess.h"
#include "hmi.h"
#include "Timer.h"

HardwareSerial *cmd_port;
HardwareSerial *ros_cmd_port;
extern MainDataStruct maindata;
extern RuntimeStatus runtimedata;

void setup() {
	cmd_port = &CMD_PORT;
	cmd_port->begin(CMD_PORT_BR);
	ros_cmd_port = &ROS_CMD_PORT;
	ros_cmd_port->begin(ROS_CMD_PORT_BR);
    
	READ_EEPROM();
	MainProcess_Init();
	buzzerPlay(500);
    TimerInit(1, 10000); //10ms
    #ifdef USE_MOTOR
        initEncoders();
        initMotorController();
    #endif
    Serial.println("end of setup.");
}

void loop() {
	UserCommand_Task();
	MainProcess_Task();
	if(runtimedata.UpdateEEPROM)
	{
		runtimedata.UpdateEEPROM = false;
		WRITE_EEPROM();
	}
    
#ifdef USE_MOTOR
    ROS_CommandProcess();

    //如果当前时刻大于 nextPID,那么就执行PID调速，并在 nextPID 上自增一个PID调试周期
    if (millis() > runtimedata.nextPID) {
        updatePID();
        runtimedata.nextPID += runtimedata.PID_INTERVAL;
    }

    // Check to see if we have exceeded the auto-stop interval
    if ((millis() - runtimedata.lastMotorCommand) > AUTO_STOP_INTERVAL) {;
        setMotorSpeeds(0, 0);
        runtimedata.moving = 0;
    }
#endif
}
ISR(TIMER1_COMPA_vect)
{
    
}

