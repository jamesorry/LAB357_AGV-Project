#include <avr/wdt.h>
#include "SoftwareSerial.h"
#include "UserCommand.h"
#include "EEPROM_Function.h"
#include "MainProcess.h"
#include "hmi.h"

#define X0 5
#define X1 4
#define X2 3
#define X3 2
#define X4 1
#define X5 0
#define pwm1 4
#define pwm2 6

#define USER_COMMAND_DEBUG  1

extern HardwareSerial *cmd_port;
extern MainDataStruct maindata;
extern RuntimeStatus runtimedata;
extern DigitalIO digitalio;


CMD g_cmdFunc[] = {
//在這新增function name 以及所呼叫的function
	{"adc", getAdc},
	{"getgpio", getGpio},
	{"setgpio", setGpio},
	{"reset", resetArduino},
	{"getmicros", getMicros},
	{"ver", cmd_CodeVer},
	{"echoon", echoOn},
	{"echooff", echoOff},
	{"reset", resetArduino},
	{"getmicros", getMicros},
	{"out", cmdOutput},
	{"in", cmdInput},
    {"cmdSend",cmdSend},
    {"cmdTest", cmdTest},
    {"GetSetADC", cmdGetSetADC},
    {"8", cmdCarFront},
    {"2", cmdCarBack},
    {"4", cmdCarLeft},
    {"6", cmdCarRight},
    {"0", cmdCarStop},
    {"5", cmdCarStart},
    {"CarError", cmdCarError},
	{"?", showHelp}
};

void cmdCarFront()
{
	String arg1;
	int value;
	if(!getNextArg(arg1))
	{
	  cmd_port->println("No parameter 1");
	  return;
	}
    value = arg1.toInt();
    digitalWrite(OutputPin[X1], 1);
    digitalWrite(OutputPin[X2], 1);
    digitalWrite(OutputPin[X3], 0);
    digitalWrite(OutputPin[X4], 1);
    analogWrite(ADC_PWMPin[pwm1],value);
    analogWrite(ADC_PWMPin[pwm2],value);
}
void cmdCarBack()
{
	String arg1;
	int value;
	if(!getNextArg(arg1))
	{
	  cmd_port->println("No parameter 1");
	  return;
	}
    value = arg1.toInt();
    digitalWrite(OutputPin[X1], 1);
    digitalWrite(OutputPin[X2], 1);
    digitalWrite(OutputPin[X3], 1);
    digitalWrite(OutputPin[X4], 0);
    analogWrite(ADC_PWMPin[pwm1],value);
    analogWrite(ADC_PWMPin[pwm2],value);
}

void cmdCarLeft()
{
	String arg1;
	int value;
	if(!getNextArg(arg1))
	{
	  cmd_port->println("No parameter 1");
	  return;
	}
    value = arg1.toInt();
    digitalWrite(OutputPin[X1], 1);
    digitalWrite(OutputPin[X2], 1);
    digitalWrite(OutputPin[X3], 1);
    digitalWrite(OutputPin[X4], 1);
    analogWrite(ADC_PWMPin[pwm1],value);
    analogWrite(ADC_PWMPin[pwm2],value);
}
void cmdCarRight()
{
	String arg1;
	int value;
	if(!getNextArg(arg1))
	{
	  cmd_port->println("No parameter 1");
	  return;
	}
    value = arg1.toInt();
    digitalWrite(OutputPin[X1], 1);
    digitalWrite(OutputPin[X2], 1);
    digitalWrite(OutputPin[X3], 0);
    digitalWrite(OutputPin[X4], 0);
    analogWrite(ADC_PWMPin[pwm1],value);
    analogWrite(ADC_PWMPin[pwm2],value);
}
void cmdCarStop()
{
    digitalWrite(OutputPin[X1], 0);
    digitalWrite(OutputPin[X2], 0);
    digitalWrite(OutputPin[X3], 1);
    digitalWrite(OutputPin[X4], 0);
    analogWrite(ADC_PWMPin[pwm1],0);
    analogWrite(ADC_PWMPin[pwm2],0);
}

void cmdCarStart()
{
    digitalWrite(OutputPin[X0], 1);    
    delay(10);
}

void cmdCarError()
{
    
    digitalWrite(OutputPin[X5], 1); 
    delay(10);
    digitalWrite(OutputPin[X5], 0); 
    delay(10);
    digitalWrite(OutputPin[X5], 1); 
    delay(10);
}
String g_inputBuffer0 = "";
String* g_inputBuffer = NULL;
String g_cmd = "";
String g_arg = "";

bool g_echoOn = true;

uint32_t targetcnt = 0;

bool getNextArg(String &arg)
{
	if (g_arg.length() == 0)
		return false;
	if (g_arg.indexOf(" ") == -1)
	{
		arg = g_arg;
		g_arg.remove(0);
	}
	else
	{
		arg = g_arg.substring(0, g_arg.indexOf(" "));
		g_arg = g_arg.substring(g_arg.indexOf(" ") + 1);
	}
	return true;
}

void resetArduino(void)
{
	wdt_enable(WDTO_500MS);
	while (1);
}
void getMicros(void)
{
	cmd_port->println(String("micros:") + micros());
}

void showHelp(void)
{
	int i;

	cmd_port->println("");
	for (i = 0; i < (sizeof(g_cmdFunc) / sizeof(CMD)); i++)
	{
		cmd_port->println(g_cmdFunc[i].cmd);
	}
}

void getAdc(void)
{
	String arg1;
	int analogPin;
	int value;

	if (!getNextArg(arg1))
	{
		cmd_port->println("No parameter");
		return;
	}
	analogPin = arg1.toInt();
	value = analogRead(analogPin);
	cmd_port->print("ADC_");
	cmd_port->print(analogPin);
	cmd_port->print(" : ");
	cmd_port->println(value);
}

void getGpio(void)
{
  String arg1, arg2;
  int digitalPin, pullUp;
  int value;

  if (!getNextArg(arg1))
  {
    cmd_port->println("No parameter");
    return;
  }
  if (!getNextArg(arg2))
  {
    pullUp = 0;
  }
  else
  {
    pullUp = arg2.toInt();
  }
  digitalPin = arg1.toInt();
  if (arg2.compareTo("na") == 0)
  {
    cmd_port->println("pin mode keep original");
  }
  else
  {
    if (pullUp)
    {
      cmd_port->println("pull-up");
      pinMode(digitalPin, INPUT_PULLUP);
    }
    else
    {
      cmd_port->println("no-pull");
      pinMode(digitalPin, INPUT);
    }
  }

  cmd_port->print("GPIO:");
  cmd_port->println(arg1);

  value = digitalRead(digitalPin);

  cmd_port->print("input value:");
  cmd_port->println(value);
}

void setGpio(void)
{
  String arg1, arg2;
  int digitalPin;
  int value;

  if (!getNextArg(arg1))
  {
    cmd_port->println("No parameter 1");
    return;
  }
  if (!getNextArg(arg2))
  {
    cmd_port->println("No parameter 2");
    return;
  }
  digitalPin = arg1.toInt();
  value = arg2.toInt();

  cmd_port->print("GPIO:");
  cmd_port->println(arg1);
  cmd_port->print("level:");
  cmd_port->println(arg2);

  digitalWrite(digitalPin, value ? HIGH : LOW);
  pinMode(digitalPin, OUTPUT);
}

void echoOn(void)
{
  g_echoOn = true;
}

void echoOff(void)
{
  g_echoOn = false;
}

void cmd_CodeVer(void)
{
  cmd_port->println(VERSTR);
}

void cmdOutput(void)
{
	String arg1, arg2;
	int digitalPin;
	int value;

	if (!getNextArg(arg1))
	{
		cmd_port->println("No parameter 1");
		return;
	}
	if (!getNextArg(arg2))
	{
		cmd_port->println("No parameter 2");
		return;
	}
	digitalPin = arg1.toInt();
	value = arg2.toInt();

	cmd_port->print("PIN index:");
	cmd_port->println(arg1);
	cmd_port->print("level:");
	cmd_port->println(arg2);

	setOutput((uint8_t)digitalPin, (uint8_t)(value ? HIGH : LOW));
	cmd_port->println("");
}

void cmdInput(void)
{
	String arg1;
	unsigned long pinindex;

	getNextArg(arg1);
	if( (arg1.length()==0))
	{
		cmd_port->println("Please input enough parameters");
		return;
	}
	pinindex = arg1.toInt();
	cmd_port->println("Sensor: " + String(getInput(pinindex)));
}

void cmdTest(void)
{
	String arg1;
    long value;

	getNextArg(arg1);
	if( (arg1.length()==0))
	{
		cmd_port->println("Please input enough parameters");
		return;
	}
	value = arg1.toInt();
	Serial3.println("value: " + String(value));
}

void cmdSend(void)
{
	String arg1;
    long value;

	getNextArg(arg1);
	if( (arg1.length()==0))
	{
		cmd_port->println("Please input enough parameters");
		return;
	}
	value = arg1.toInt();
    Serial1.println("cmdTest");
	Serial1.println("cmdTest " + String(value));
}

void cmdGetSetADC()
{
	String arg1, arg2;
	int Pin,value;
	if(!getNextArg(arg1))
	{
	  cmd_port->println("No parameter 1");
	  return;
	}
    Pin = arg1.toInt();
	if(!getNextArg(arg2))
	{
	    cmd_port->println("No parameter 2");
        value = analogRead(ADC_PWMPin[Pin]);
        cmd_port->println("Pin " + String(Pin) + ": " + String(value));
        return;
	}
	value = arg2.toInt();
    if(Pin%2 == 0){
        pinMode(ADC_PWMPin[Pin], OUTPUT);
        analogWrite(ADC_PWMPin[Pin], value);
        cmd_port->println("PWM value: " + String(Pin) + String(value));
    }
    else
    {
        pinMode(ADC_PWMPin[Pin], OUTPUT);
        analogWrite(ADC_PWMPin[Pin], value);
        cmd_port->println("ADC value: " + String(Pin) + String(value));
    }
}


uint8_t UserCommWorkindex = 0;

uint32_t UserCommandTimeCnt = 0;

void UserCommand_Task(void)
{
  int i, incomingBytes, ret, cmdPortIndex;
  char data[2] = {0};

  switch(UserCommWorkindex)
  {
    case 0:
    {
      
      if(cmd_port->available())
      {
        g_inputBuffer = &g_inputBuffer0;
        UserCommWorkindex ++;
        UserCommandTimeCnt = millis();
      }
      break;
    }
    case 1:
    {
      if((millis() - UserCommandTimeCnt) > 50)
        UserCommWorkindex ++;
      break;
    }
    case 2:
    {
      if ( incomingBytes = cmd_port->available() )
      {

//      cmd_port->println("cmd_port datalen: " + String(incomingBytes));
      Serial3.println("cmd_port datalen: " + String(incomingBytes));
      for ( i = 0; i < incomingBytes; i++ )
      {
        ret = cmd_port->read();
        if ( (ret >= 0x20) && (ret <= 0x7E) || (ret == 0x0D) || (ret == 0x0A) )
        {
        data[0] = (char)ret;
        (*g_inputBuffer) += data;
        if (g_echoOn)
        {
          if ( (data[0] != 0x0D) && (data[0] != 0x0A) )
          cmd_port->write(data);
        }
        }
        else if (ret == 0x08)
        {
        if (g_inputBuffer->length())
        {
          g_inputBuffer->remove(g_inputBuffer->length() - 1);
          if (g_echoOn)
          {
          data[0] = 0x08;
          cmd_port->write(data);
          }
        }
        }
      }
      if (g_inputBuffer->indexOf('\r') == -1)
      {
        if (g_inputBuffer->indexOf('\n') == -1)
        return;
      }
      g_inputBuffer->trim();
      while (g_inputBuffer->indexOf('\r') != -1)
        g_inputBuffer->remove(g_inputBuffer->indexOf('\r'), 1);
      while (g_inputBuffer->indexOf('\n') != -1)
        g_inputBuffer->remove(g_inputBuffer->indexOf('\n'), 1);
      while (g_inputBuffer->indexOf("  ") != -1)
        g_inputBuffer->remove(g_inputBuffer->indexOf("  "), 1);
    
      cmd_port->println();
    
      if (g_inputBuffer->length())
      {
        g_arg.remove(0);
        if (g_inputBuffer->indexOf(" ") == -1)
        g_cmd = (*g_inputBuffer);
        else
        {
        g_cmd = g_inputBuffer->substring(0, g_inputBuffer->indexOf(" "));
        g_arg = g_inputBuffer->substring(g_inputBuffer->indexOf(" ") + 1);
        }
        for (i = 0; i < (sizeof(g_cmdFunc) / sizeof(CMD)); i++)
        {
        //if(g_cmd==g_cmdFunc[i].cmd)
        if (g_cmd.equalsIgnoreCase(g_cmdFunc[i].cmd))
        {
          g_cmdFunc[i].func();
          cmd_port->print("ARDUINO>");
          break;
        }
        else if (i == (sizeof(g_cmdFunc) / sizeof(CMD) - 1))
        {
          cmd_port->println("bad command !!");
          cmd_port->print("ARDUINO>");
        }
        }
        *g_inputBuffer = "";
      }
      else
      {
        cmd_port->print("ARDUINO>");
      }
      UserCommWorkindex = 0;
      break;
    }
  }

  }
}

