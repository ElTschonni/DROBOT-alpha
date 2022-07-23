/*================================================================================
  o   ___          _   _         _   _             .
  -`^'-      o  <_*_>        '\\-//`       '\\-//`        ,-_-|
  (o o)      o  (o o)         (o o)         (o o)        ([o o])
  |--ooO--(_)--Ooo--8---(_)--Ooo-ooO--(_)--Ooo-ooO--(_)--Ooo-ooO--(_)--Ooo- --------|
  |Titel:         Delta Robot Project Teko 2022                                                     |
  |File name:     LibraryDROBOT.h                                            |
  |Autor Name:    Jonathan Ernst & Reto Bissig  
  |MechanicalTeam: Lorenz, Alain, 
  |Datum:         29.06.2022                                                        |
  |Hardware:      M5Stack Core2 (Mikrocontroller: ESP32)                         |
  |Software:      PlatformIO C++  VSCode (www.arduino.cc)                              |
  |Beschreibung:  DROBOT Library
  |---------------------------------------------------------------------------------|
  |---------------------------------------------------------------------------------|

    |--------------------------------------------------------------------------------|*/

#ifndef DROBOT_h
#define DROBOT_h
#include "Arduino.h"
#include "M5Core2.h"
#include "Adafruit_MCP23X17.h"


#define bouncingTime 50
#define TMotor_Position_min 0
#define TMotor_Position_max 90686
//MotorClass Definitions
#define Motor_Turn_right 1
#define Motor_Turn_left 0
#define Motor_Position_right 1
#define Motor_Position_left 0


//=========| Instancing Classes: |==================================================
//=========| Generating Object |==================================================
//=========| Classes |==================================================
//1: CLSM Class: Closed_Loop_Step_Motor
//2: MMC: Motor_Movement_Calc

//Constructor:

//=========| 1: CLSM Class: Closed_Loop_Step_Motor  |=============
class Closed_Loop_Step_Motor  //CLSM
{
private:
  int us_timer_state;
  unsigned long us_timer_value;
  unsigned long us_timer_start;
  //Motor Pin Configuration
  int Motor_Enable_Pin;  //Hardware configuration of the Enable pin
  int Motor_Pull_Pin;    //Pull Pin Puls to move by one step
  int Motor_Dir_Pin;     //Direction pin - set to high to move right, low for left
  int Motor_Alarm_Pin;   //Alarm Pin
  int Motor_Ped_Pin;     // Confirmation bit for reaching the new stepp

  //  Adafruit_MCP23X17 GPIO_I2C_Exp;
  Adafruit_MCP23X17 *GPIO_I2C_Exp;


  long int delayNumber = 0;

  //Motor Pin Values
  bool Motor_Enable_Value = false;
  bool Motor_Pull_Value = false;
  bool Motor_Dir_Value = Motor_Turn_right;
  bool Motor_Alarm_Value = false;
  bool Motor_Ped_Value = false;
  bool Motor_Position_Value = Motor_Position_right;  //1 = right. 0 = left

  int Dip_Switch_Value = 0;

  //Error Code
  /* 1 = Alarm Pin Motor
    2 = Old Angle Value Wrong
    3=  New Angle Value Wrong
    4= Step failed
    5= Step Time out of Tollerance
    6= 
    7= 
    8= 
    9= 
    10= 
  */

  //according to the Datasheet iHSS57
  double Dip_Switch_Array[17][3] = {
    { 40000, 0.009 },
    { 20000, 0.018 },
    { 10000, 0.036 },
    { 8000, 0.045 },
    { 5000, 0.072 },
    { 4000, 0.09 },
    { 2000, 0.18 },
    { 1000, 0.36 },
    { 51200, 0.00703125 },
    { 25600, 0.0140625 },
    { 12800, 0.028125 },
    { 6400, 0.05625 },
    { 3200, 0.1125 },
    { 1600, 0.225 },
    { 800, 0.45 },
    { 0, 0 }
  };

  double Old_Angle_Value = 0;
  double New_Angle_Value = 0;
  double Max_Angle_Value = 360;
  double Min_Angle_Value = 0;


public:
  //Attributes
  double Motor_Angle_Value = 0;
  double Angle_Per_step = 0;  //	360 / StepsPerRotation
  double Steps_Per_rev = 0;
  double RPM_SetPoint_Value = 0;         //	setpoint value
  double RPM_ProcessVariable_Value = 0;  //	calculating the actual value
  double RPM_Error_Value = 0;            //	calculate the difference between setpoint and actual value
  double Time_PerRotation_Value = 0;     //	RPM_SetPoint_Value - RPM_ProcessVariable_Value
  double Time_PerStep_Value = 0;         //	(60 / RPM_ProcessVariable_Value) / StepsPerRotation:
  double Frequency_Pulse_Value = 0;      //	1/Time_PerStep_Value
  double Time_PulseHigh_Value = 0;       //	Time_PerStep_Value * 0.5
  double Time_PulseLow_Value = 0;        //	Time_PerStep_Value * 0.5
  double Frequency_I2CBus_Value = 0;     //	Frequency_Pulse_Value * 2

  bool Angle_Tollerance_Value = 0;  //1 = Angle in Tollerance; 0 = Angle outside Tollerance

  int errorcode = 0;


  Closed_Loop_Step_Motor(Adafruit_MCP23X17 *GPIO_instance) {

    GPIO_I2C_Exp = GPIO_instance;
  }




  //Methods
  //Method to configure the Motor Object. - Hardware Pin Mapping and DipSwitch Position
  void setupMotor(bool MotorPos, int EnPin, int PullPin, int DirPin, int AlmPin, int PedPin, int RPM, int DipSwitch) {  //Motor Pin Configuration
    Motor_Enable_Pin = EnPin;
    Motor_Pull_Pin = PullPin;
    Motor_Dir_Pin = DirPin;
    Motor_Alarm_Pin = AlmPin;
    Motor_Ped_Pin = PedPin;
    Dip_Switch_Value = DipSwitch;
    Angle_Per_step = Dip_Switch_Array[DipSwitch][1];  // Read Value Angle per Step from DipSwitch Array
    Steps_Per_rev = Dip_Switch_Array[DipSwitch][0];   //Read Value Steps per Revolution from Dip Switch Array
    Motor_Position_Value = MotorPos;

    RPM_SetPoint_Value = RPM;


    Time_PerRotation_Value = 60 / RPM_SetPoint_Value;
    Time_PerStep_Value = (60 / RPM_SetPoint_Value) / Steps_Per_rev;
    Frequency_Pulse_Value = 1 / Time_PerStep_Value;
    Time_PulseHigh_Value = Time_PerStep_Value * 0.5;
    Time_PulseLow_Value = Time_PerStep_Value * 0.5;
    Frequency_I2CBus_Value = Frequency_Pulse_Value * 2;
  }

  void test() {
    Serial.print("MotorClassTest: ");
    M5.lcd.print("MotorClassTest: ");

    GPIO_I2C_Exp->digitalWrite(0, LOW);
    /*
   GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 1);
    GPIO_I2C_Exp->digitalWrite(Motor_Dir_Pin, 1);  //set direction 1= right 0 =left
    delayMicroseconds(1);

    GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 0);  //send one Pulse
    GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 1);
      delayMicroseconds(5);
    GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 0);
     */
    GPIO_I2C_Exp->digitalWrite(0, HIGH);


    //disableMotor();
  }

  void enableMotor() {
    Motor_Enable_Value = LOW;
    Serial.println(" MotorEnabled ");
    M5.lcd.println(" MotorEnabled ");
    GPIO_I2C_Exp->digitalWrite(Motor_Enable_Pin, Motor_Enable_Value);
  }
  void disableMotor() {
    Motor_Enable_Value = HIGH;
    Serial.println(" MotorDisabled ");
    M5.lcd.println(" MotorDisabled ");
    GPIO_I2C_Exp->digitalWrite(Motor_Enable_Pin, Motor_Enable_Value);
  }

  bool startSurvilance() {
    //  If motor Alarm Pin is high -> alarm
    Motor_Alarm_Value =GPIO_I2C_Exp->digitalRead(Motor_Alarm_Pin);
    
    if (Motor_Alarm_Value == true) {
      Serial.println("ALARM");
      M5.lcd.println("ALARM");
      errorcode = 1;
      return (0);
    } else {
      return (1);
    }
  }


  bool setOldAngelValue(double Angle) {

    while (Angle >= 360) { Angle = Angle - 360; }
    //keeps Angle below 360°
    if ((Motor_Position_Value == 1) and ((Angle <= 90) or (Angle >= 270))) {  //if motorposition is right / 90°=>x<=270°
      Old_Angle_Value = Angle;
      Serial.print("setOldAngleValue r ");
      Serial.println(Old_Angle_Value);
      M5.lcd.print("setOldAngleValue r ");
      M5.lcd.println(Old_Angle_Value);
      return (1);
    } else if ((Motor_Position_Value == 0) and (Angle >= 90) and (Angle <= 270))  //if motorposition is left / 90°=<x<=270°
    {
      Old_Angle_Value = Angle;
      Serial.print("setOldAngleValue l ");
      Serial.println(Old_Angle_Value);
      M5.lcd.print("setOldAngleValue l ");
      M5.lcd.println(Old_Angle_Value);
      return (1);
    } else {
      return (0);  //returns error Message (Angle Value Wrong)
      errorcode = 2;
    }
  }

  double getOldAngelValue() {
    return (Old_Angle_Value);
    Serial.println("getOldAngleValue");
    M5.lcd.println("getOldAngleValue");
  }


  bool setNewAngelValue(double Angle) {

    while (Angle >= 360) { Angle = Angle - 360; }                     //keeps Angle below 360°
    if (Motor_Position_Value and (Angle <= 90) and (Angle >= 270)) {  //if motorposition is right / 90°=>x>=-90°
      Angle = Angle + 90;
      if (Angle >= 360) Angle = Angle - 360;  //if Angle between 270 and 360, it will be changed to a value between 0 and 90
      New_Angle_Value = Angle - 90;           //Angle between 0 and 180, will be changed to a value between -90 and 90
      Serial.print("setNewAngleValue r ");
      Serial.println(New_Angle_Value);
      M5.lcd.print("setNewAngleValue r ");
      M5.lcd.println(New_Angle_Value);
      return (1);
    } else if (!Motor_Position_Value and (Angle >= 90) and (Angle <= 270))  //if motorposition is left / 90°=<x<=270°
    {
      New_Angle_Value = Angle;  //
      Serial.print("setNewAngleValue l ");
      Serial.println(New_Angle_Value);
      M5.lcd.print("setNewAngleValue l ");
      M5.lcd.println(New_Angle_Value);
      return (1);
    } else {
      return (0);  //returns error Message (Angle Value Wrong)
      errorcode = 3;
    }
  }

  double getNewAngelValue() {
    return (New_Angle_Value);
    Serial.println("getNewAngleValue");
    M5.lcd.println("getNewAngleValue");
  }

  //MotorMovement Methodes
  bool moveOneStep(bool direction) {
    long int counter = 0;

    if (startSurvilance()) {
      GPIO_I2C_Exp->digitalWrite(Motor_Dir_Pin, direction);  //set direction 1= right 0 =left
      delayMicroseconds(5);
      GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 1);  //send one Pulse
      delayMicroseconds(5);
      GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 0);

      // maybe needs to be an Interupt
      do {
        Motor_Ped_Value = GPIO_I2C_Exp->digitalRead(Motor_Ped_Pin);
           counter++;
                  Serial.print(" Motor_Ped_Pin: ");
        Serial.print(GPIO_I2C_Exp->digitalRead(Motor_Ped_Pin));
         Serial.print(" counter: ");
           Serial.print(counter);
      } while (((Motor_Ped_Value == false)) and (counter < 100000));  //wait until position has been reached

      if (counter > 10000) {
        Serial.print(" Warning: Slow PED Response: ");  //Warning it took more than 10000 tries
        M5.lcd.print(" Warning: Slow PED Response ");
        Serial.print(counter);
        Serial.print("Motor_Ped_Pin: ");
        Serial.print(GPIO_I2C_Exp->digitalRead(Motor_Ped_Pin));
      }
          return (1);  //it was successfull
    }
else{ return (0); }



    if (counter > 90000) {
      return (0);  //error - try again
      Serial.print(" Error: Step failed");
      M5.lcd.print(" Error: Step failed");
      errorcode = 4;
    }
  }

  bool OneStepDir(/*bool direction*/) {
    bool I_did_one_Step = false;
    unsigned long counter = 0;
    unsigned long Step_Time_Value;
    bool survilance = 1;
    double Max_AngleDev_value = 0;
    double Min_AngleDev_value = 0;



    //start timer
    us_timer_state = 1;
    us_timer_value = Time_PerStep_Value * 1000000;
    us_timer_start = micros();
    Max_AngleDev_value = New_Angle_Value + Angle_Per_step * 1.05;
    Min_AngleDev_value = New_Angle_Value - Angle_Per_step * 1.05;

    //enableMotor();
    // survilance = startSurvilance();

    if (survilance) {
      Serial.print(" New_Angle_Value: ");
      Serial.print(New_Angle_Value);
      Serial.print(" Old_Angle_Value: ");
      Serial.print(Old_Angle_Value);

      M5.lcd.print(" New_Angle_Value: ");
      M5.lcd.print(New_Angle_Value);
      M5.lcd.print(" Old_Angle_Value: ");
      M5.lcd.print(Old_Angle_Value);

      //aslong as the old angle is out of new angle tollerance
      if (Old_Angle_Value < Min_AngleDev_value) {
        Angle_Tollerance_Value = 0;

                while ((I_did_one_Step == 0) and (counter < 100000) and startSurvilance()) {  // try as until you did one Stepbut try not more than 100000 times.
          I_did_one_Step = moveOneStep(Motor_Turn_left);


          counter++;
        };
        counter = 0;
        I_did_one_Step = 0;
        Old_Angle_Value = Old_Angle_Value + Angle_Per_step;

      } else if (Old_Angle_Value > Max_AngleDev_value)  //if the new Angle is smaller than the old turn the motor left to make the difference smaller.
      {
        Angle_Tollerance_Value = 0;
       
        while ((I_did_one_Step == 0) and (counter < 100000) and survilance) {  // try as until you did one Stepbut try not more than 100000 times.
                                                                               //  survilance = startSurvilance();
          I_did_one_Step = moveOneStep(Motor_Turn_right);
          counter++;
        };
        counter = 0;
        I_did_one_Step = 0;  //normaly 0
        Old_Angle_Value = Old_Angle_Value - Angle_Per_step;

      } else {

        Angle_Tollerance_Value = 1;
        Serial.print(" InTollerance ");
        M5.lcd.print(" InTollerance ");
      }



      while (us_timer_state and survilance) {  //wait if time per step is to short
        survilance = startSurvilance();

        if ((us_timer_value) < (micros() - us_timer_start)) {
          us_timer_state = 0;


          Serial.print(" us_timer_value: ");
          Serial.print(us_timer_value);
          Serial.print(" < micros: ");
          Serial.print(micros() - us_timer_start);


          Step_Time_Value = (micros() - us_timer_start) / 1000;
          Serial.print(" StepTime: ");
          Serial.println(Step_Time_Value);

          M5.lcd.print(" StepTime: ");
          M5.lcd.println(Step_Time_Value);
       

          if ((Step_Time_Value) >= (us_timer_value / 990)) errorcode = 5;

          us_timer_state = 0;
          // disableMotor();
          return (1);
         
        }
      }
    }
    else {return (1);}
    Serial.println();
    M5.lcd.println();
  }
};

#endif