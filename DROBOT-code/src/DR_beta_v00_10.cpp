/*================================================================================
  o   ___          _   _         _   _             .
  -`^'-      o  <_*_>        '\\-//`       '\\-//`        ,-_-|
  (o o)      o  (o o)         (o o)         (o o)        ([o o])
  |--ooO--(_)--Ooo--8---(_)--Ooo-ooO--(_)--Ooo-ooO--(_)--Ooo-ooO--(_)--Ooo- --------|
  |Titel:         Delta Robot Project Teko 2022                                                     |
  |File name:     2022-06-19_DROBOT_v00.002.ino                                            |
  |Autor Name:    Jonathan Ernst & Reto Bissig  
  |MechanicalTeam: Lorenz, Alain, 
  |Datum:         29.06.2022                                                        |
  |Hardware:      M5Stack Core2 (Mikrocontroller: ESP32)                         |
  |Software:      Arduino IDE 1.8.19 (www.arduino.cc)                               |
  |Beschreibung:  Delta Zeichnungsroboter 

  //Change Log 
2022-06-29_DROBOT_v00.002.ino  - Setup Modbus Server
2022-07-08_DROBOT_v00.007.ino  - I2C Library
2022-07-10_DROBOT_v00.010.cpp -- portation to platformio. - small changes to test I2C library, so far LED on.

  |---------------------------------------------------------------------------------|
  //=========| include librarys |==================================================*/
#include "DROBOT.h"  // Robot controll
#include "Wire.h"    // I2C
#include "M5Core2.h"
#include "WiFi.h"
#include "ModbusIP_ESP8266.h"
#include "WiFiMulti.h"
#include <Adafruit_MCP23X17.h>


//=========| definiere Werte |==================================================*/
#define ON HIGH
#define OFF LOW
#define baudRate 2000000  //set baud rate for serial communiction

#define WLAN_SSID "Wildbachhuis 2.4"  // define: suchen und ersetzen im Code "L-TEKO"
#define WLAN_PASSWD "Milkyway29!"     //"teko2016"
#define MCP23017_ADDR 0x20            //I2C Addrese
#define I2C_Clock_Speed 1700000       // 1.7 MHz

#define Mright 1
#define Mleft 0

#define Mright_DIPSW_Value 0b0010  //initial Motor Speed in RPM
#define Mleft_DIPSW_Value 0b0010   //initial Motor Speed in RPM

//Robot Measurements:
#define length_BaseToTip_Value 630  // length from the rotation axis to the tool tip in mm
#define max_TimeForComand_value 60  //maximum time for one command in s

//IOPorts //Adresses of Pins for Adafruit MCP23017
#define Mright_Pull_Pin 8      //	GPB0	MCP23017
#define Mright_Dir_Pin 9       //	GPB1	MCP23017
#define Mright_En_Pin 10       //	GPB2	MCP23017
#define Mright_Alarm_Pin 11    //	GPB3	MCP23017
#define Mright_Ped_Pin 12      //	GPB4	MCP23017
#define Reed_Sensor_Pin 13     //	GPB5	MCP23017
#define Mvertical_Down_Pin 14  //	GPB6	MCP23017
#define Mvertical_Up_Pin 15    //	GPB7	MCP23017
#define Clamp_Servo_Pin 0      //	GPA0	MCP23017
#define End_Button2_Pin 1      //	GPA1	MCP23017
#define End_Button1_Pin 2      //	GPA2	MCP23017
#define Mleft_Ped_Pin 3        //	GPA3	MCP23017
#define Mleft_Alarm_Pin 4      //	GPA4	MCP23017
#define Mleft_En_Pin 5         //	GPA5	MCP23017
#define Mleft_Dir_Pin 6        //	GPA6	MCP23017
#define Mleft_Pull_Pin 7       //	GPA7	MCP23017





//Sensors:

//Set Constants for modbus
// G01 X30.00 Y100.00 Z10.00 F100
const int BEFEHL_HREG = 0;  // Hreg 0 für den Befehl, z.B. G01
const int X_HREG = 1;       // Hreg 1 für die X-Koordinate
const int Y_HREG = 2;       // Hreg 2 für die Y-Koordinate
const int Z_HREG = 3;       // Hreg 3 für die Z-Koordinate
const int F_HREG = 4;       // Hreg 4 für die Geschwindigkeit
const int STATUS_HREG = 5;  // Hreg 5 für den Zustand der StateMachine


//=========| initializing Classes |==================================================*/
//ModbusIP object
ModbusIP mb;
WiFiMulti wiFiMulti;
Adafruit_MCP23X17 GPIO_Ports_Instanz;                         //creating the MCP23017 Instance
Closed_Loop_Step_Motor Motor_Left_Ins(&GPIO_Ports_Instanz);   //creating the Left Motor Instance
Closed_Loop_Step_Motor Motor_Right_Ins(&GPIO_Ports_Instanz);  //creating the Right Motor Instance
Motor_Movement_Calc MM_Calc_Ins;                              //create the motor movement calculation instance


//==============================================================================*/
//=========| deklaration von Variabeln |========================================*/
int programSelector = 6;  //Program selection variable

int StartUp = 1;


bool Start_StateMachine = false;
int Status = 1;  // 1 ready
uint Befehl = 0;


//Timer Variabeln:
unsigned long Timer_Start_01;
unsigned long Timer_Start_02;


unsigned long Timer_Interval_01 = 1000000;
unsigned long Timer_Interval_02 = 10000;


//ModBus

float X, Y, Z, F;
float Alpha;
float Px, Xaktuell, Xneu;
float Py, Yaktuell, Yneu;


// Motor Variabeln
int Mright_Speed_Value = 1;  //initial Motor Speed in RPM
int Mleft_Speed_Value = 1;   //initial Motor Speed in RPM

//==============================================================================*/
//=========| Funktionen ModBus |=========================================*/

//Return new GCode_Command ReadModbusRegister CBneuerBefehl


//Return new Client_Command ReadModbusRegister CBneuerBefehl
uint16_t CBneuerBefehl(TRegister* reg, uint16_t val) {
  if (Start_StateMachine == false) {
    Serial.println("Neuer Befehl!");
    programSelector = 2;
    Befehl = mb.Hreg(BEFEHL_HREG);
    Start_StateMachine = true;
  }
  return val;
}

uint16_t CBneuerParameterX(TRegister* reg, uint16_t val) {
  Serial.println("Neue Parameter X!");
  X = mb.Hreg(X_HREG) / 100.0;  // X in 100stel Millimeter
  return val;
}
uint16_t CBneuerParameterY(TRegister* reg, uint16_t val) {
  Serial.println("Neue Parameter Y!");
  Y = mb.Hreg(Y_HREG) / 100.0;
  return val;
}
uint16_t CBneuerParameterZ(TRegister* reg, uint16_t val) {
  Serial.println("Neue Parameter Z!");
  Z = mb.Hreg(Z_HREG) / 100.0;
  return val;
}
uint16_t CBneuerParameterF(TRegister* reg, uint16_t val) {
  Serial.println("Neue Parameter F!");
  F = mb.Hreg(F_HREG);  // Geschwindigkeit in mm/min
  return val;
}



//=========| Funktionen Program |=========================================*/
void moveToNewPositionLeft() {
  unsigned long counterTollerance = 0;
  bool exitStep = false;

  do {
    //   Serial.print(" Number of Steps: ");
    //   Serial.print(counterTollerance++);

    do {  //do one step
      if (Motor_Left_Ins.OneStepDir() == 0) exitStep = false;
      else { exitStep = true; }
      if (Motor_Left_Ins.errorcode > 0) exitStep = true;
    } while (exitStep == false);

  } while (!Motor_Left_Ins.Angle_Tollerance_Value);

  counterTollerance = 0;
}

void moveToNewPositionRight() {
  unsigned long counterTollerance = 0;
  bool exitStep = false;

  do {
    //   Serial.print(" Number of Steps: ");
    //   Serial.print(counterTollerance++);

    do {  //do one step
      if (Motor_Right_Ins.OneStepDir() == 0) exitStep = false;
      else { exitStep = true; }
      if (Motor_Right_Ins.errorcode > 0) exitStep = true;
    } while (exitStep == false);

  } while (!Motor_Right_Ins.Angle_Tollerance_Value);

  counterTollerance = 0;
}

//=========| Funktionen StateMachine |=========================================*/



int Standby()  //State 1
{
  Serial.println("------ Standby -------");
  programSelector = 1;

  mb.task();
  mb.Hreg(STATUS_HREG, Status);
  Serial.print(STATUS_HREG);
  Serial.print(Status);
  M5.lcd.print(Status);

  Serial.print("- Roboter_State: ");
  Serial.println(Status);
  Serial.print("- GCode_Command: ");
  Serial.println(Befehl);
  Serial.print("- Start Statemachine: ");
  Serial.println(Start_StateMachine);
  //GPIO_Ports_Instanz.digitalWrite(Clamp_Servo_Pin, HIGH);



  return (programSelector);
}

int Receive()  //State 2 Verarbeitung
{
  mb.task();
  Serial.println("------ State Receive------");
  M5.lcd.println("------ State Receive: ");
  Serial.print("- Roboter_State: ");
  Serial.println(Status);
  Serial.print("- GCode_Command: ");
  Serial.println(Befehl);
  Serial.print("- Start Statemachine: ");
  Serial.println(Start_StateMachine);


  if (Status == 1 and Start_StateMachine) {
    Serial.println("- StateMachine: Started");
    M5.lcd.println("- StateMachine: Started: ");

    if (Befehl == 0 or Befehl == 1) {
      Serial.println("- Receive: Data");
      M5.lcd.println("- Receive: Data ");
      Serial.print(" G");
      Serial.print(Befehl);
      Serial.print(" X");
      Serial.print(X);
      Serial.print(" Y");
      Serial.print(Y);
      Serial.print(" Z");
      Serial.print(Z);
      Serial.print(" F");
      Serial.println(F);

      MM_Calc_Ins.X2_Target_Value = X;  // Neues X von Modbus
      MM_Calc_Ins.Y2_Target_Value = Y;  // Neues Y von Modbus
      MM_Calc_Ins.Done_Steps_Value = 0;
      Status = 2;  // gehe zu draw
      return (3);

    } else {
      Status = 3;  // falscher Befehl
      return (4);
    }
  }

  /*
  if (Timer_Start_01 + Timer_Interval_01 < micros()) {
    Timer_Start_01 = micros();
    Timer_Interval_01 = 1000000;
  }*/
}

int Draw() {  //State 3
  Serial.println("------ Draw -------");
  double old_Angle_value = 0;
  double new_Angle_value = 0;

  //delay(500);
  Serial.println("Robot is Calculationg Coordinates ");

  //MotorControll Motor Left
  Motor_Left_Ins.test();  //Motor Class Test

  delay(500);
  Serial.println("Robot is Calculationg Route ");

  //297mm*420mm
  MM_Calc_Ins.setTargetCoordinates(MM_Calc_Ins.X2_Target_Value, MM_Calc_Ins.Y2_Target_Value);  //test coordinates
  MM_Calc_Ins.Interpolate();

  /*
  //1. set new angle
  if (Motor_Left_Ins.setNewAngelValue(random(0, 360))) Serial.println("Motor Left Set Angle: Succeed");
  else {
    Serial.println("Motor Left Set Angle: Failed");
    return (5);
  }
*/

  Serial.println("Robot is drawing ");
  Serial.print("Old Angle: ");
  Serial.println(Motor_Left_Ins.getOldAngelValue());
  Serial.print("New Angle: ");
  Serial.println(Motor_Left_Ins.getNewAngelValue());

  Serial.print("Tollerance: ");
  Serial.println(Motor_Left_Ins.Angle_Tollerance_Value);
  //move from old angle to new angle
  /*
moveToNewPositionLeft();
moveToNewPositionRight();
*/
  //moveToNewPosition();

//exit conditions:
  Start_StateMachine = false;

  if (Motor_Left_Ins.errorcode > 0) {
    Status = 3;
    return (5);
  }
  //if the current coordinates are not equal to the target coordinates do the loop again
  else if ((MM_Calc_Ins.Xz_NextStep_Value < MM_Calc_Ins.X2_Target_Value) and (MM_Calc_Ins.Yz_NextStep_Value < MM_Calc_Ins.Y2_Target_Value)) {
    Status = 2;
    return (3);
  } else if ((MM_Calc_Ins.Xz_NextStep_Value == MM_Calc_Ins.X2_Target_Value) and (MM_Calc_Ins.Yz_NextStep_Value == MM_Calc_Ins.Y2_Target_Value)) {

    Serial.print("New Coordinates have been reached: Xz ");
    Serial.print(MM_Calc_Ins.Xz_NextStep_Value);
    Serial.print("Yz ");
    Serial.print(MM_Calc_Ins.Yz_NextStep_Value);
    Serial.print(" / X2 ");
    Serial.print(MM_Calc_Ins.X2_Target_Value);
    Serial.print(" / Y2 ");
    Serial.println(MM_Calc_Ins.Y2_Target_Value);
    Serial.print(" Waiting for new Coordinates ");
    Status = 1;
    return (1);
  }
}

int WrongCommand()  //State 4, steuert die Motoren
{
  Serial.println("------ Wrong Command -------");
  Status = 1;
  return (1);
}

int Error() {  //State 5
  Serial.println("------ Error -------");
  Serial.print(" Error Code Motor Left: ");
  Serial.println(Motor_Left_Ins.errorcode);
  Serial.print(" Error Code Motor Right: ");
  Serial.println(Motor_Right_Ins.errorcode);
Motor_Left_Ins.errorcode=0;
  Status = 1;

  return (1);
}


int Init() {  // State 6
  Serial.println("------ Initialize -------");
  M5.lcd.println("------ Initialize -------");
  //Initialize Motor Left
  Serial.println("Init: Motor Left");
  M5.lcd.println("Init: Motor Left");

  Motor_Left_Ins.setupMotor(
    Mleft,
    Mleft_En_Pin,
    Mleft_Pull_Pin,
    Mleft_Dir_Pin,
    Mleft_Alarm_Pin,
    Mleft_Ped_Pin,
    Mleft_Speed_Value,
    Mleft_DIPSW_Value);

  Serial.print(" Mleft: ");
  M5.lcd.print(" Mleft: ");

  Serial.print(" RPM: ");
  M5.lcd.print(" RPM: ");
  Serial.println(Mleft_Speed_Value);
  M5.lcd.println(Mleft_Speed_Value);

  Serial.print(" DIPSW: ");
  M5.lcd.print(" DIPSW: ");
  Serial.println(Mleft_DIPSW_Value);
  M5.lcd.println(Mleft_DIPSW_Value);

  Serial.print(" MStepsPRev: ");
  M5.lcd.print(" MStepsPRev: ");
  Serial.println(Motor_Left_Ins.Steps_Per_rev);
  M5.lcd.println(Motor_Left_Ins.Steps_Per_rev);


  Serial.print(" MAngle: ");
  M5.lcd.print(" MAngle: ");
  Serial.println(Motor_Left_Ins.Motor_Angle_Value);
  M5.lcd.println(Motor_Left_Ins.Motor_Angle_Value);

  Serial.print(" MAnglePS: ");
  M5.lcd.print(" MAnglePS: ");
  Serial.println(Motor_Left_Ins.Angle_Per_step);
  M5.lcd.println(Motor_Left_Ins.Angle_Per_step);


  Serial.print(" MRPM SP: ");
  M5.lcd.print(" MRPM SP: ");
  Serial.println(Motor_Left_Ins.RPM_SetPoint_Value);
  M5.lcd.println(Motor_Left_Ins.RPM_SetPoint_Value);

  Serial.print(" MRPM PV: ");
  M5.lcd.print(" MRPM PV: ");
  Serial.println(Motor_Left_Ins.RPM_ProcessVariable_Value);
  M5.lcd.println(Motor_Left_Ins.RPM_ProcessVariable_Value);

  Serial.print(" MRPM EV: ");
  M5.lcd.print(" MRPM EV: ");
  Serial.println(Motor_Left_Ins.RPM_Error_Value);
  M5.lcd.println(Motor_Left_Ins.RPM_Error_Value);


  Serial.print(" MTPR: ");
  M5.lcd.print(" MTPR: ");
  Serial.println(Motor_Left_Ins.Time_PerRotation_Value);
  M5.lcd.println(Motor_Left_Ins.Time_PerRotation_Value);


  Serial.print(" MTPS: ");
  M5.lcd.print(" MTPS: ");
  Serial.println(Motor_Left_Ins.Time_PerStep_Value);
  M5.lcd.println(Motor_Left_Ins.Time_PerStep_Value);

  Serial.print(" MFtreq: ");
  M5.lcd.print(" MFtreq: ");
  Serial.println(Motor_Left_Ins.Frequency_Pulse_Value);
  M5.lcd.println(Motor_Left_Ins.Frequency_Pulse_Value);

  Serial.print(" MFtreqI2C: ");
  M5.lcd.print(" MFtreqI2C: ");
  Serial.println(Motor_Left_Ins.Frequency_I2CBus_Value);
  M5.lcd.println(Motor_Left_Ins.Frequency_I2CBus_Value);

  Serial.println("Init: Motor Left Succeed");
  M5.lcd.println("Init: Motor Left Succeed");

  Serial.print("PEDL: ");
  Serial.println(GPIO_Ports_Instanz.digitalRead(Mleft_Ped_Pin));
  Serial.print("ALML: ");
  Serial.println(GPIO_Ports_Instanz.digitalRead(Mleft_Alarm_Pin));


  //Initialize Motor Right
  Serial.println("Init: Motor Right");
  M5.lcd.println("Init: Motor Right");

  Motor_Right_Ins.setupMotor(
    Mright,
    Mright_En_Pin,
    Mright_Pull_Pin,
    Mright_Dir_Pin,
    Mright_Alarm_Pin,
    Mright_Ped_Pin,
    Mright_Speed_Value,
    Mright_DIPSW_Value);

  Serial.print(" Mright: ");
  M5.lcd.print(" Mright: ");

  Serial.print(" RPM: ");
  M5.lcd.print(" RPM: ");
  Serial.println(Mright_Speed_Value);
  M5.lcd.println(Mright_Speed_Value);

  Serial.print(" DIPSW: ");
  M5.lcd.print(" DIPSW: ");
  Serial.println(Mright_DIPSW_Value);
  M5.lcd.println(Mright_DIPSW_Value);

  Serial.print(" MStepsPRev: ");
  M5.lcd.print(" MStepsPRev: ");
  Serial.println(Motor_Right_Ins.Steps_Per_rev);
  M5.lcd.println(Motor_Right_Ins.Steps_Per_rev);


  Serial.print(" MAngle: ");
  M5.lcd.print(" MAngle: ");
  Serial.println(Motor_Right_Ins.Motor_Angle_Value);
  M5.lcd.println(Motor_Right_Ins.Motor_Angle_Value);

  Serial.print(" MAnglePS: ");
  M5.lcd.print(" MAnglePS: ");
  Serial.println(Motor_Right_Ins.Angle_Per_step);
  M5.lcd.println(Motor_Right_Ins.Angle_Per_step);


  Serial.print(" MRPM SP: ");
  M5.lcd.print(" MRPM SP: ");
  Serial.println(Motor_Right_Ins.RPM_SetPoint_Value);
  M5.lcd.println(Motor_Right_Ins.RPM_SetPoint_Value);

  Serial.print(" MRPM PV: ");
  M5.lcd.print(" MRPM PV: ");
  Serial.println(Motor_Right_Ins.RPM_ProcessVariable_Value);
  M5.lcd.println(Motor_Right_Ins.RPM_ProcessVariable_Value);

  Serial.print(" MRPM EV: ");
  M5.lcd.print(" MRPM EV: ");
  Serial.println(Motor_Right_Ins.RPM_Error_Value);
  M5.lcd.println(Motor_Right_Ins.RPM_Error_Value);


  Serial.print(" MTPR: ");
  M5.lcd.print(" MTPR: ");
  Serial.println(Motor_Right_Ins.Time_PerRotation_Value);
  M5.lcd.println(Motor_Right_Ins.Time_PerRotation_Value);


  Serial.print(" MTPS: ");
  M5.lcd.print(" MTPS: ");
  Serial.println(Motor_Right_Ins.Time_PerStep_Value);
  M5.lcd.println(Motor_Right_Ins.Time_PerStep_Value);

  Serial.print(" MFtreq: ");
  M5.lcd.print(" MFtreq: ");
  Serial.println(Motor_Right_Ins.Frequency_Pulse_Value);
  M5.lcd.println(Motor_Right_Ins.Frequency_Pulse_Value);

  Serial.print(" MFtreqI2C: ");
  M5.lcd.print(" MFtreqI2C: ");
  Serial.println(Motor_Right_Ins.Frequency_I2CBus_Value);
  M5.lcd.println(Motor_Right_Ins.Frequency_I2CBus_Value);


  Serial.println("Init: Motor Right Succeed");
  M5.lcd.println("Init: Motor Right Succeed");

  //init calculation class
  Serial.println("Init: Calculation");
  M5.lcd.println("Init: Calculation");

  MM_Calc_Ins.setupInterpolate(Motor_Left_Ins.Steps_Per_rev, Motor_Left_Ins.Angle_Per_step, length_BaseToTip_Value);
  MM_Calc_Ins.setMaxTime(max_TimeForComand_value);
  MM_Calc_Ins.setDoneSteps(0);
  Serial.println("Init: Calculation Succeed");
  M5.lcd.println("Init: Calculation Succeed");
  return (7);
}

int Calibrate() {  //State 7
  Serial.println("------ Calibration -------");
  //1. move Motor left to endpoint 1


  //2.save Angle of Motor left

  if (Motor_Left_Ins.setOldAngelValue(90)) Serial.println("Motor Left Set Angle: Succeed");
  else {
    Serial.println("Motor Left Set Angle: Failed");
    return (5);
  }

  //3. move Motor left to position 180°
  if (Motor_Left_Ins.setOldAngelValue(180)) Serial.println("Motor Left Set Angle: Succeed");
  else {
    Serial.println("Motor Left Set Angle: Failed");
    return (5);
  }

  moveToNewPositionLeft();

  //4. move Motor right to enpoint 2


  //5. save position of Motor Right

  if (Motor_Right_Ins.setOldAngelValue(90)) Serial.println("Motor Right Set Angle: Succeed");
  else {
    Serial.println("Motor Right Set Angle: Failed");
    return (5);
  }
  //6. move to position 0/0
  //inverse Kinematic



  //set origin Coordinates to x0/y0
  MM_Calc_Ins.setOriginCoordinates(0, 0);

  Serial.print(" Calibration: New Coordinates: X: ");
  Serial.print(MM_Calc_Ins.X1_Origin_Value);
  Serial.print(" Y: ");
  Serial.println(MM_Calc_Ins.Y1_Origin_Value);

  Serial.print(" Calibration: Successfull ");
  Serial.println(Motor_Right_Ins.errorcode);




  Status = 1;
  return (1);
}



//=========| MAIN |=============================================================*/
//=========| setup |============================================================*/
void setup() {
  int error;


  M5.begin();

  delay(1000);


  //serial communiction
  Serial.begin(baudRate);      // open the serial port
  while (!Serial) delay(100);  // wait for native usb
  Serial.println("-----!StartingUp!------");

  //Start I2C
  Serial.println("-Start: I2C");
  M5.lcd.println("-Start: I2C");

  if (!GPIO_Ports_Instanz.begin_I2C()) {
    Serial.println("Error.I2C");
    M5.lcd.println("Error.I2C");
    while (1)
      ;
  }
  //Pinconfiguration :
  Serial.println("-I2C_ Set Input/Output Pins");
  M5.lcd.println("-I2C_ Set Input/Output Pins ");

  GPIO_Ports_Instanz.pinMode(Mright_Pull_Pin, OUTPUT);         //	Pull+ Blue Motor Left
  GPIO_Ports_Instanz.pinMode(Mright_Dir_Pin, OUTPUT);          //	Dir+ White Motor Left
  GPIO_Ports_Instanz.pinMode(Mright_En_Pin, OUTPUT);           //	En + BrownBlack Motor Left&Right
  GPIO_Ports_Instanz.pinMode(Mright_Alarm_Pin, INPUT_PULLUP);  //	Alarm+ Whiteblack Motor Left
  GPIO_Ports_Instanz.pinMode(Mright_Ped_Pin, INPUT_PULLUP);    //	Ped+ GreenBlack Motor Left
  GPIO_Ports_Instanz.pinMode(Reed_Sensor_Pin, INPUT_PULLUP);   //	Reed / Push Sensor
  GPIO_Ports_Instanz.pinMode(Mvertical_Down_Pin, OUTPUT);      //	Vertical Down
  GPIO_Ports_Instanz.pinMode(Mvertical_Up_Pin, OUTPUT);        //	Vertical Up
  GPIO_Ports_Instanz.pinMode(Clamp_Servo_Pin, OUTPUT);         //	Clamp Orange Servo
  GPIO_Ports_Instanz.pinMode(End_Button2_Pin, INPUT_PULLUP);   //	End_Button 2 left
  GPIO_Ports_Instanz.pinMode(End_Button1_Pin, INPUT_PULLUP);   //	End_Button 1 right
  GPIO_Ports_Instanz.pinMode(Mleft_Ped_Pin, INPUT_PULLUP);     //	Ped+ GreenBlack Motor Left
  GPIO_Ports_Instanz.pinMode(Mleft_Alarm_Pin, INPUT_PULLUP);   //	Alarm+ Whiteblack Motor Left
  GPIO_Ports_Instanz.pinMode(Mleft_En_Pin, OUTPUT);            //	En + BrownBlack Motor Left&Right
  GPIO_Ports_Instanz.pinMode(Mleft_Dir_Pin, OUTPUT);           //	Dir+ White Motor Left
  GPIO_Ports_Instanz.pinMode(Mleft_Pull_Pin, OUTPUT);          //	Pull+ Blue Motor Left


  Serial.println("-I2C: Turn on LED 15");
  M5.lcd.println("-I2C: Turn on LED 15");
  GPIO_Ports_Instanz.digitalWrite(Clamp_Servo_Pin, LOW);

  Serial.println("-I2C: wait 1s");
  M5.lcd.println("-I2C: wait 1s");
  delay(1000);
  Serial.println("-I2C: Turn off LED 15");
  M5.lcd.println("-I2C: Turn off LED 15");
  GPIO_Ports_Instanz.digitalWrite(Clamp_Servo_Pin, HIGH);




  Serial.println("-Start: I2C - Succeed");
  M5.lcd.println("-Start: I2C - Succeed");

  //Start Wifi

  Serial.println("-Start: Wifi");
  M5.lcd.println("-Start: Wifi");
  Serial.println("-Connect: WiFi...");
  M5.lcd.print("Waiting connect to WiFi...");

  wiFiMulti.addAP(WLAN_SSID, WLAN_PASSWD);
  int sum = 0;

  while (wiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    M5.lcd.print(".");
    sum += 1;
    if (sum == 8) {
      Serial.println("!Connect: Failed!");
      M5.lcd.print("!Connect: Failed!");
      wiFiMulti.addAP(WLAN_SSID, WLAN_PASSWD);
    }
  }

  Serial.println("");
  Serial.println("-Connect: Wifi -Succeed");
  Serial.print("-Show: IP address: ");
  Serial.println(WiFi.localIP());  //The serial port outputs the IP address of the M5Core2.
  Serial.print("-Show: RSSI: ");
  Serial.println(WiFi.RSSI());

  M5.lcd.println("-Connect: Wifi -Succeed");
  M5.lcd.print("-Show: IP address: ");
  M5.lcd.println(WiFi.localIP());  //The serial port outputs the IP address of the M5Core2.
  M5.lcd.print("-Show: RSSI: ");
  M5.lcd.println(WiFi.RSSI());  //Output signal strength


  Serial.print("-Start: Timer ");
  M5.lcd.print("-Start: Timer ");
  //start timer
  Timer_Start_01 = micros();
  Timer_Start_02 = millis();

  unsigned char i;
  Serial.println("-Start: -Succeed ");
  M5.lcd.println("-Start: -Succeed ");

  //set Pin Mode

  //StartModBus Server
  Serial.print("-Start: ModBus Server ");
  M5.lcd.print("-Start: ModBus Server ");


  mb.server();
  mb.addHreg(BEFEHL_HREG, 0, 1);
  mb.addHreg(X_HREG, 0, 1);
  mb.addHreg(Y_HREG, 0, 1);
  mb.addHreg(Z_HREG, 0, 1);
  mb.addHreg(F_HREG, 0, 1);
  mb.addHreg(STATUS_HREG, 0, 1);
  mb.onSetHreg(BEFEHL_HREG, CBneuerBefehl);
  mb.onSetHreg(X_HREG, CBneuerParameterX);
  mb.onSetHreg(Y_HREG, CBneuerParameterY);
  mb.onSetHreg(Z_HREG, CBneuerParameterZ);
  mb.onSetHreg(F_HREG, CBneuerParameterF);




  Serial.println("!StartUp Complete!");

  delay(1000);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
}

//=========| Hauptprogramm    |=================================================*/
void loop() {
  int cycleCounter = 0;
  int state = 1;

  while (1) {
    state = programSelector;
    M5.update();
    if (cycleCounter > 15) {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0);
      cycleCounter = 0;
    }
    cycleCounter++;
    Serial.print(" state: ");
    Serial.println(state);

    switch (state) {
      case 1: state = Standby(); break;  //Standby

      case 2: state = Receive(); break;  //receive new Coordinates

      case 3: state = Draw(); break;  //Draw according to the new command

      case 4: state = WrongCommand(); break;  //Received a Wrong Command

      case 5: state = Error(); break;  //Test Modus, Teil 3 (Leistungsmessung mit Last)

      case 6: state = Init(); break;  //Test Modus, Teil 4 (Überprüfung und Anpassung der Daten)

      case 7: state = Calibrate(); break;  //Test Modus, Teil 4 (Überprüfung und Anpassung der Daten)
    }
    programSelector = state;
  }
}




//==============================================================================

//interupt service routinen:
/*---| starte Interrupt Routinen |-----
  attachInterrupt( 3, check_S3DModul1, CHANGE);        //wenn sich Pin  3, Schalter S3D auf dem Modul 1 ändert, wird die ISR ausgeführt
  attachInterrupt( 6, check_S4DModul2, CHANGE);        //wenn sich Pin  6, Schalter S4D auf dem Modul 2 ändert, wird die ISR ausgeführt
  attachInterrupt( 9, check_S5DModul3, CHANGE);        //wenn sich Pin  9, Schalter S5D auf dem Modul 3 ändert, wird die ISR ausgeführt
  attachInterrupt(12, check_S6DModul4, CHANGE);        //wenn sich Pin 12, Schalter S6D auf dem Modul 4 ändert, wird die ISR ausgeführt
  attachInterrupt(24, check_AttachedModul1, CHANGE);   //24,
  attachInterrupt(27, check_AttachedModul2, CHANGE);   //27,
  attachInterrupt(30, check_AttachedModul3, CHANGE);   //30,
  attachInterrupt(33, check_AttachedModul4, CHANGE);   //33
*/