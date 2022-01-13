#include "ros_bridge_command.h"
#include "diff_pid_controller.h"
extern SetPointInfo leftPID, rightPID;

/* Encoders*/
#include "encoder_driver.h"

/* Motor driver*/
#include "motor_driver.h"

#include "MainProcess.h"
extern HardwareSerial *cmd_port;
extern HardwareSerial *ros_cmd_port;
extern MainDataStruct maindata;
extern RuntimeStatus runtimedata;

// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];

// The arguments converted to integers
long arg1;
long arg2;

/* Clear the current command parameters */
void resetCommand() {
    cmd = NULL;
    memset(argv1, 0, sizeof(argv1));
    memset(argv2, 0, sizeof(argv2));
    arg1 = 0;
    arg2 = 0;
    arg = 0;
    index = 0;
}

/* Run a command.  Commands are defined in commands.h */
int runCommand() {
    int i = 0;
    char *p = argv1;
    char *str;
    int pid_args[4];
    arg1 = atoi(argv1);
    arg2 = atoi(argv2);

    switch(cmd) {
        case GET_BAUDRATE:
            ros_cmd_port->println(BAUDRATE);
            break;
        case ANALOG_READ:
            ros_cmd_port->println(analogRead(arg1));
            break;
        case DIGITAL_READ:
            ros_cmd_port->println(digitalRead(arg1));
            break;
        case ANALOG_WRITE:
            analogWrite(arg1, arg2);
            ros_cmd_port->println("OK"); 
            break;
        case DIGITAL_WRITE:
            if (arg2 == 0) digitalWrite(arg1, LOW);
            else if (arg2 == 1) digitalWrite(arg1, HIGH);
            ros_cmd_port->println("OK"); 
            break;
        case PIN_MODE:
            if (arg2 == 0) pinMode(arg1, INPUT);
            else if (arg2 == 1) pinMode(arg1, OUTPUT);
            ros_cmd_port->println("OK");
        break;
        case PING:
            Serial.println(VENDOR);
            break;
        case READ_ENCODERS:
            ros_cmd_port->print(readEncoder(LEFT));
            ros_cmd_port->print(" ");
            ros_cmd_port->println(readEncoder(RIGHT));
            break;
        case RESET_ENCODERS:
            resetEncoders();
            resetPID();
            ros_cmd_port->println("OK");
            break;
        case MOTOR_SPEEDS:
            /* Reset the auto stop timer */
            runtimedata.lastMotorCommand = millis();
            if (arg1 == 0 && arg2 == 0) {
              setMotorSpeeds(0, 0);
              resetPID();
              runtimedata.moving = 0;
            }
            else runtimedata.moving = 1;
            leftPID.TargetTicksPerFrame = arg1;
            rightPID.TargetTicksPerFrame = arg2;
            ros_cmd_port->println("OK"); 
        break;
        case UPDATE_PID:
            while ((str = strtok_r(p, ":", &p)) != '\0') {
               pid_args[i] = atoi(str);
               i++;
            }
            maindata.Kp = pid_args[0];
            maindata.Kd = pid_args[1];
            maindata.Ki = pid_args[2];
            maindata.Ko = pid_args[3];
            ros_cmd_port->println("OK");
            break;
        default:
            ros_cmd_port->println("Invalid Command");
            break;
    }
}

void ROS_CommandProcess()
{
    while (ros_cmd_port->available() > 0) {
        // Read the next character
        chr = ros_cmd_port->read();

        // Terminate a command with a CR
        if (chr == 13) {
            if (arg == 1) argv1[index] = NULL;
            else if (arg == 2) argv2[index] = NULL;
            runCommand();
            resetCommand();
        }
        // Use spaces to delimit parts of the command
        else if (chr == ' ') {
            // Step through the arguments
            if (arg == 0) arg = 1;
            else if (arg == 1)  {
            argv1[index] = NULL;
            arg = 2;
            index = 0;
            }
            continue;
        }
        else {
            if (arg == 0) {
                // The first arg is the single-letter command
                cmd = chr;
            }
            else if (arg == 1) {
                // Subsequent arguments can be more than one character
                argv1[index] = chr;
                index++;
            }
            else if (arg == 2) {
                argv2[index] = chr;
                index++;
            }
        }
    }
}

