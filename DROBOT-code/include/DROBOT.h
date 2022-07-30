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
#define right_Arm 1
#define left_Arm 0


#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

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
  long us_timer_state;
  unsigned long us_timer_value;
  unsigned long us_timer_start;
  //Motor Pin Configuration
  unsigned int Motor_Enable_Pin;  //Hardware configuration of the Enable pin
  unsigned int Motor_Pull_Pin;    //Pull Pin Puls to move by one step
  unsigned int Motor_Dir_Pin;     //Direction pin - set to high to move right, low for left
  unsigned int Motor_Alarm_Pin;   //Alarm Pin
  unsigned int Motor_Ped_Pin;     // Confirmation bit for reaching the new stepp

  //  Adafruit_MCP23X17 GPIO_I2C_Exp;
  Adafruit_MCP23X17 *GPIO_I2C_Exp;


  //Motor Pin Values
  bool Motor_Enable_Value = false;
  bool Motor_Pull_Value = false;
  bool Motor_Dir_Value = Motor_Turn_right;
  bool Motor_Alarm_Value = false;
  bool Motor_Ped_Value = false;
  bool Motor_Position_Value = Motor_Position_right;  //1 = right. 0 = left

  unsigned int Dip_Switch_Value = 0;
  double Gear_Belt_Value = 0;


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

  unsigned int errorcode = 0;
  double Max_AngDev_Value = 0;
  double Min_AngDev_Value = 0;

  Closed_Loop_Step_Motor(Adafruit_MCP23X17 *GPIO_instance) {

    GPIO_I2C_Exp = GPIO_instance;
  }




  //Methods
  //Method to configure the Motor Object. - Hardware Pin Mapping and DipSwitch Position
  void setupMotor(bool MotorPos, unsigned int EnPin, unsigned int PullPin, unsigned int DirPin, unsigned int AlmPin, unsigned int PedPin, unsigned int RPM, unsigned int DipSwitch, double GearBelt) {  //Motor Pin Configuration
    Motor_Enable_Pin = EnPin;
    Motor_Pull_Pin = PullPin;
    Motor_Dir_Pin = DirPin;
    Motor_Alarm_Pin = AlmPin;
    Motor_Ped_Pin = PedPin;
    Dip_Switch_Value = DipSwitch;
    Gear_Belt_Value = GearBelt;
    Angle_Per_step = Dip_Switch_Array[DipSwitch][1]/Gear_Belt_Value;  // Read Value Angle per Step from DipSwitch Array
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
    Motor_Alarm_Value = GPIO_I2C_Exp->digitalRead(Motor_Alarm_Pin);
    /*
    if (Motor_Alarm_Value == true) {
      Serial.println("ALARM");
      M5.lcd.println("ALARM");
      errorcode = 1;
      return (1);
    } else {
      return (1);
    }*/
    return (1);
  }


  bool setOldAngelValue(double Angle) {

    while (Angle >= 360) { Angle = Angle - 360; }
    //keeps Angle below 360°
    if ((Motor_Position_Value == 1) and ((Angle <= 100) or (Angle >= 270))) {  //if motorposition is right / 90°=>x<=270°
      Old_Angle_Value = Angle;
      /*   Serial.print("setOldAngleValue r ");
      Serial.println(Old_Angle_Value);
      M5.lcd.print("setOldAngleValue r ");
      M5.lcd.println(Old_Angle_Value);*/
      return (1);
    } else if ((Motor_Position_Value == 0) and (Angle >= 80) and (Angle <= 270))  //if motorposition is left / 90°=<x<=270°
    {
      Old_Angle_Value = Angle;
      /*   Serial.print("setOldAngleValue l ");
      Serial.println(Old_Angle_Value);
      M5.lcd.print("setOldAngleValue l ");
      M5.lcd.println(Old_Angle_Value);*/
      return (1);
    } else {
      return (0);  //returns error Message (Angle Value Wrong)
      errorcode = 2;
    }
  }

  double getOldAngelValue() {
    return (Old_Angle_Value);
    Serial.println("getOldAngleValue");
  }


  bool setNewAngelValue(double Angle) {

    while (Angle >= 360) { Angle = Angle - 360; }                                                  //keeps Angle below 360°
    if ((Motor_Position_Value and (Angle <= 120)) or (Motor_Position_Value and (Angle >= 270))) {  //if motorposition is right / 90°=>x>=-90°
      Angle = Angle + 120;
      if (Angle >= 360) Angle = Angle - 360;  //if Angle between 270 and 360, it will be changed to a value between 0 and 90
      New_Angle_Value = Angle - 120;          //Angle between 0 and 180, will be changed to a value between -90 and 90
      Serial.print(" sNA: setNewAngleValue° R: ");
      Serial.println(New_Angle_Value);

      return (1);
    } else if (!Motor_Position_Value and (Angle >= 80) and (Angle <= 270))  //if motorposition is left / 90°=<x<=270°
    {
      New_Angle_Value = Angle;  //
      Serial.print(" sNA: setNewAngleValue° L: ");
      Serial.println(New_Angle_Value);

      return (1);
    } else {
      return (0);  //returns error Message (Angle Value Wrong)
      errorcode = 3;
    }
  }

  double getNewAngelValue() {
    return (New_Angle_Value);
    Serial.println("getNewAngleValue");
  }

/*
void StartTimer(){
      //start timer
    us_timer_state = 1;
    us_timer_value = Time_PerStep_Value * 1000000;
    us_timer_start = micros();

}

double TimeControll(){
    unsigned long Step_Time_Value =0;
    bool survilance = 1;


  
      while (us_timer_state and survilance) {  //wait if time per step is to short
        survilance = startSurvilance();
        Step_Time_Value = micros() - us_timer_start;

        if ((us_timer_value) < Step_Time_Value) us_timer_state = 0;
      }
      return (Step_Time_Value);
      }*/

  //MotorMovement Methodes
  bool moveOneStep(bool direction) {
    long long counter = 0;

    if (startSurvilance()) {
      GPIO_I2C_Exp->digitalWrite(Motor_Dir_Pin, direction);  //set direction 1= right 0 =left
      delayMicroseconds(5);
      GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 1);  //send one Pulse
      delayMicroseconds(5);
      GPIO_I2C_Exp->digitalWrite(Motor_Pull_Pin, 0);

      // maybe needs to be an Interupt
      /*  do {
        Motor_Ped_Value = GPIO_I2C_Exp->digitalRead(Motor_Ped_Pin);
        counter++;
        //               Serial.print(" Motor_Ped_Pin: ");
        //    Serial.print(GPIO_I2C_Exp->digitalRead(Motor_Ped_Pin));
        //     Serial.print(" counter: ");
        //       Serial.print(counter);
      } while (((Motor_Ped_Value == false)) and (counter < 100000));  //wait until position has been reached

      if (counter > 10000) {
        Serial.print(" Warning: Slow PED Response: ");  //Warning it took more than 10000 tries
        M5.lcd.print(" Warning: Slow PED Response ");
        Serial.print(counter);
        Serial.print("Motor_Ped_Pin: ");
        Serial.print(GPIO_I2C_Exp->digitalRead(Motor_Ped_Pin));
      }*/
      return (1);  //it was successfull
    } else {
      return (0);
    }



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
    bool survilance = 1;
    Max_AngDev_Value = 0;
    Min_AngDev_Value = 0;

    //Angle deviation depending on min. resolution
    Max_AngDev_Value = New_Angle_Value + Angle_Per_step * 0.5;
    Min_AngDev_Value = New_Angle_Value - Angle_Per_step * 0.5;/*
Serial.println("");
      Serial.print("OSD: Max_AngleDev_value : ");
      Serial.print(Max_AngDev_Value);
        Serial.print("OSD: Min_AngleDev_value : ");
      Serial.println(Min_AngDev_Value);*/

    //enableMotor();
    // survilance = startSurvilance();

    if (survilance) {
      /*Serial.println("");
      Serial.print("OSD: New_Angle_Value: ");
      Serial.print(New_Angle_Value);
      Serial.print(" Old_Angle_Value: ");
      Serial.println(Old_Angle_Value);*/

      //aslong as the old angle is out of new angle tollerance
      if (Old_Angle_Value < Min_AngDev_Value) {
        Angle_Tollerance_Value = 0;
        while ((I_did_one_Step == 0) and (counter < 100000) and startSurvilance()) {  // try as until you did one Stepbut try not more than 100000 times.
          I_did_one_Step = moveOneStep(Motor_Turn_left);
          counter++;
        };
        counter = 0;
        I_did_one_Step = 0;
        Old_Angle_Value = Old_Angle_Value + Angle_Per_step;

      } else if (Old_Angle_Value > Max_AngDev_Value)  //if the new Angle is smaller than the old turn the motor left to make the difference smaller.
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
        
      }

    } 

    return(1);


  }
};

class Motor_Movement_Calc {
private:
  double Steps_Per_Rev = 0;
  double Angle_Per_Step = 0;
  double Angle_Per_StepB = 0;

public:
  unsigned long TotalNof_Steps_Value = 0;
  unsigned long Done_Steps_Value = 0;
  unsigned long Missing_Steps_Value = 0;

  double Min_Resolution_Value = 0;
  double Time_PerStep_Value = 0;
  double Max_Work_Time = 0;
  unsigned long X1_Origin_Value = 0;
  unsigned long Y1_Origin_Value = 0;

  unsigned long X2_Target_Value = 0;
  unsigned long Y2_Target_Value = 0;

  unsigned long Xz_NextStep_Value = 0;
  unsigned long Yz_NextStep_Value = 0;

  double Length_ofWay_Value = 0;      // The distance between Point A and B
  double Length_BaseToTip_Value = 0;  // The distance from the axis of the arm to the tip of the pen

  void setupInterpolate(double StPerRot, double AngPerStp, double LArm) {  //Set all nessesary Variables
    double MinR_Value = 0;

    Steps_Per_Rev = StPerRot;
    Angle_Per_Step = AngPerStp*1.05; // Angle prer step times a 5% Tollerance Margin
    Angle_Per_StepB = DEG_TO_RAD * Angle_Per_Step;  //Angle per Step in Radiants
    Length_BaseToTip_Value = LArm;

    Serial.print(" sI: Steps_Per_Rev: ");
    Serial.println(Steps_Per_Rev);
    Serial.print("sI:  Angle_Per_Step: ");
    Serial.println(Angle_Per_Step);
    Serial.print("sI:  Length_BaseToTip_Value ");
    Serial.println(Length_BaseToTip_Value);

    //calculate the minimum resolution per step depending on the minimum Angle and the length of the Robot Arm
    MinR_Value = (Length_BaseToTip_Value * Length_BaseToTip_Value + Length_BaseToTip_Value * Length_BaseToTip_Value - 2 * Length_BaseToTip_Value * Length_BaseToTip_Value * cos(Angle_Per_StepB));
    if (MinR_Value <= 0) MinR_Value *(-1);  //Make sure value is not <0
    Min_Resolution_Value = sqrt(MinR_Value);

    Serial.print("sI:  Min_Resolution_Value: ");
    Serial.println(Min_Resolution_Value);
  }

  //set the origin coordinates og the robot arm
  void setOriginCoordinates(unsigned long X1, unsigned long Y1) {
    X1_Origin_Value = X1;
    Y1_Origin_Value = Y1;
    Serial.print(" sOC: X1_Origin_Value ");
    Serial.print(X1_Origin_Value);
    Serial.print(" Y1_Origin_Value ");
    Serial.println(Y1_Origin_Value);
  }

  void setTargetCoordinates(unsigned long X2, unsigned long Y2) {
    X2_Target_Value = X2;
    Y2_Target_Value = Y2;

    Serial.print(" sTC: X2_Target_Value ");
    Serial.print(X2_Target_Value);
    Serial.print(" Y2_Target_Value ");
    Serial.println(Y2_Target_Value);
  }

  void setMaxTime(double Tmax) {
    Max_Work_Time = Tmax;
    Time_PerStep_Value = Max_Work_Time / Steps_Per_Rev;

    Serial.print(" sMT: Max_Work_Time ");
    Serial.println(Max_Work_Time);
    Serial.print(" sMT: Time_PerStep_Value ");
    Serial.println(Time_PerStep_Value);
  }

  void setDoneSteps(unsigned long Dsteps) {
    Done_Steps_Value = Dsteps;

    Serial.print(" sDS: Done_Steps_Value ");
    Serial.println(Done_Steps_Value);
  }

  void Interpolate() {
    double LoW_Value = 0;
    double Q_Value = 0;

    if (Done_Steps_Value == 0) {  // Calculations once per Interpolate session
      LoW_Value = (Y2_Target_Value - Y1_Origin_Value) * (Y2_Target_Value - Y1_Origin_Value);
      if (LoW_Value <= 0) LoW_Value *(-1);                    //Make sure value is not <0
      Length_ofWay_Value = sqrt(LoW_Value);                   //if it is 0 then there is nothing to do
      if (Length_ofWay_Value == 0) TotalNof_Steps_Value = 0;  //Number of Steps is also gona be 0
      else {
        TotalNof_Steps_Value = Length_ofWay_Value / Min_Resolution_Value;
        /*
       Serial.print(" Ib: TotalNof_Steps_Value ");
        Serial.print(TotalNof_Steps_Value);
        Serial.print(" = Min_Res:  ");
        Serial.print(Min_Resolution_Value);
        Serial.print(" / ");
        Serial.print(Length_ofWay_Value);
        Serial.println("mm ");*/
      }
    }
    //interpolation
    if ((Done_Steps_Value <= TotalNof_Steps_Value) and (TotalNof_Steps_Value > 0)) {
      //calculate Q Value


      Q_Value = Done_Steps_Value / (TotalNof_Steps_Value * 1.00);
      /*Serial.print(" Ic: Q_Value = ");
        Serial.print( Done_Steps_Value);
        Serial.print(" / ");
        Serial.println(TotalNof_Steps_Value);*/

      Xz_NextStep_Value = ((1 - Q_Value) * X1_Origin_Value) + (Q_Value * X2_Target_Value);
      Yz_NextStep_Value = ((1 - Q_Value) * Y1_Origin_Value) + (Q_Value * Y2_Target_Value);
      /*
        Serial.println(" Ic: Xz_NextStep_Value = ((1 - Q_Value) * X1_Origin_Value) + (Q_Value * X2_Target_Value);");
        Serial.print(" ((");
        Serial.print(1);
        Serial.print(" - ");
        Serial.print(Q_Value);
        Serial.print(" )) *  ");
        Serial.print(X1_Origin_Value);
        Serial.print(" ) + (");
        Serial.print(Q_Value);
        Serial.print(" * ");
        Serial.print(X2_Target_Value);
        Serial.println(" ) ");

      Serial.print(" Ic: Xz_NextStep_Value ");
      Serial.print(Xz_NextStep_Value);
      Serial.print(" Yz_NextStep_Value ");
      Serial.print(Yz_NextStep_Value);
      Serial.print(" Done_Steps_Value ");*/
      Serial.print(Done_Steps_Value);
      Serial.print(" <= ");
      Serial.println(TotalNof_Steps_Value);

      Done_Steps_Value++;

      //interpolation finished
    } else if ((Done_Steps_Value >= TotalNof_Steps_Value) and (TotalNof_Steps_Value > 0)) {

      /*  Serial.print(" Id: Xz_NextStep_Value: ");
      Serial.print(Xz_NextStep_Value);
      Serial.print(" Yz_NextStep_Value ");
      Serial.println(Yz_NextStep_Value);
      Serial.print(" Id: X2_Target_Value  : ");
      Serial.print(X2_Target_Value);
      Serial.print(" Y2_Target_Value  : ");
      Serial.println(Y2_Target_Value);*/

      Y1_Origin_Value = Yz_NextStep_Value;
      X1_Origin_Value = Xz_NextStep_Value;


      Serial.print(" Id: X1_Origin_Value  : ");
      Serial.print(X1_Origin_Value);
      Serial.print(" Y1_Origin_Value  : ");
      Serial.println(Y1_Origin_Value);

      Done_Steps_Value = 0;
      Length_ofWay_Value = 0;

      Serial.println(" Finished Interpolate ");

    } else {
      Serial.println(" long: no Change ");
    }
  }
};

class Delta2D_Kinematic {
private:
  //the linear slope between the intersection of the two circles
  //G=K1-K2
  //G: y=mx+b -> Intersection_I1_Y  = Line_S1S2_m * Intersection_I1_X + Line_S1S2_b
  // insert y into K1
  //solve the quadratic equation for x
  double Line_S1S2_b = 0;
  double Line_S1S2_m = 0;

  //Static Roboter Parameters in mm
  unsigned long Lenght_UpArm_Value = 0;   //length of the Left Upper Arm in mm
  unsigned long Lenght_LowArm_Value = 0;  //length of the Left Lower Arm in mm

  unsigned long MeetingPoint_TipPoint_Value = 0;  //length between tip of the tool and meeting point of arms
  double MeetingPoint_TipPoint_AngleValue = 0;    //Angle of the line between Meetingpoint to tip of the tool
  unsigned long DeltaX_LeftArm_Value = 0;         //Delta X from center to rotation point of the Left Arm
  unsigned long DeltaX_RightArm_Value = 0;        //Delta X from center to rotation point of the Right Arm
  double Transmission_LeftArm_Value = 0;
  double Transmission_RightArm_Value = 0;

public:
  double Circle1_Middle_X = 0;  // Middle Point coordianetes X of Circle 1
  double Circle1_Middle_Y = 0;  // Middle Point coordianetes Y of Circle 1
  double Circle2_Middle_X = 0;  // Middle Point coordianetes X of Circle 2
  double Circle2_Middle_Y = 0;  // Middle Point coordianetes Y of Circle 2

  double Intersection_I1_X = 0;  //Intersection S1
  double Intersection_I1_Y = 0;  //Intersection S1
  double Intersection_I2_X = 0;  //Intersection S2
  double Intersection_I2_Y = 0;  //Intersection S2

  double angle_LeftArm_Value1 = 0;
  double angle_LeftArm_Value2 = 0;
  double angle_RightArm_Value1 = 0;
  double angle_RightArm_Value2 = 0;

  //Methodes:
  void setupDelta2D_Kinematic(
    unsigned long LUArm,
    unsigned long LLArm,
    unsigned long MPTipP,
    double MPTipPAngle,
    unsigned long DXLArm,
    unsigned long DXRArm,
    double TransLArm,
    double TransRArm) {
    Lenght_UpArm_Value = LUArm;   //length of the Left Upper Arm in mm
    Lenght_LowArm_Value = LLArm;  //length of the Left Lower Arm in mm

    MeetingPoint_TipPoint_Value = MPTipP;            //length between tip of the tool and meeting point of arms
    MeetingPoint_TipPoint_AngleValue = MPTipPAngle;  //Angle of the line between Meetingpoint to tip of the tool
    DeltaX_LeftArm_Value = DXLArm;                   //Delta X from center to rotation point of the Left Arm
    DeltaX_RightArm_Value = DXRArm;                  //Delta X from center to rotation point of the Right Arm
    Transmission_LeftArm_Value = TransLArm;
    Transmission_RightArm_Value = TransRArm;

    Serial.print("SD2D: L_UpArm_Value: ");
    Serial.println(Lenght_UpArm_Value);
    Serial.print("SD2D: L_LowArm_Value: ");
    Serial.println(Lenght_LowArm_Value);
    Serial.print("SD2D: R_UpArm_Value: ");
    Serial.println(MeetingPoint_TipPoint_Value);
    Serial.print("SD2D: MeetingPoint_TipPoint_AngleValue: ");
    Serial.println(MeetingPoint_TipPoint_AngleValue);
    Serial.print("SD2D: DeltaX_LeftArm_Value: ");
    Serial.println(DeltaX_LeftArm_Value);
    Serial.print("SD2D: DeltaX_RightArm_Value:  ");
    Serial.println(DeltaX_RightArm_Value);
    Serial.print("SD2D: Transmission_LeftArm_Value: ");
    Serial.println(Transmission_LeftArm_Value);
    Serial.print("SD2D: Transmission_RightArm_Value:  ");
    Serial.println(Transmission_RightArm_Value);
  }

  unsigned long IntersectionOfTwoCircles(unsigned long C1MX, unsigned long C1MY, unsigned long C2MX, unsigned long C2MY) {  //Calculate the intersection of two circles
    //1.Step: G = K1-K2
    // Gerade zw. den Kreisschnittpunkten
    // G=K1-K2
    // G: y=mx+b
    // y in K1 einsetzen
    // Quadratische Gleichung nach X auflösen
    double p = 0;
    double q = 0;
    double discriminant_2Circle_Value = 0;
    double pStack = 0;
    double UpArm2;
    double LowArm2, X2, X1, Y1, Y2, Y2x2;
    Circle1_Middle_X = C1MX * 1.00;  // Middle Point coordianetes X of Circle 1 (Drehgelenk Schulterachse)
    Circle1_Middle_Y = C1MY * 1.00;  // Middle Point coordianetes Y of Circle 1 (Drehgelenk Schulterachse)
    Circle2_Middle_X = C2MX * 1.00;  // Middle Point coordianetes X of Circle 2 (Koordinaten von Stift)
    Circle2_Middle_Y = C2MY * 1.00;  // Middle Point coordianetes Y of Circle 2 (Koordinaten von Stift)

    Serial.print("Io2C: 1 Circle Middle: X  ");
    Serial.print(Circle1_Middle_X);
    Serial.print(" Y: ");
    Serial.print(Circle1_Middle_Y);
    Serial.print(" 2 Circle Middle: X  ");
    Serial.print(Circle2_Middle_X);
    Serial.print(" Y: ");
    Serial.println(Circle2_Middle_Y);

    // y=mx+b
    Line_S1S2_m = (Circle1_Middle_X * 1.00 - Circle2_Middle_X * 1.00) / ((Circle2_Middle_Y * 1.00 - Circle1_Middle_Y * 1.00));

    UpArm2 = Lenght_UpArm_Value * Lenght_UpArm_Value * 1.00;
    LowArm2 = Lenght_LowArm_Value * Lenght_LowArm_Value;
    Y2 = Circle2_Middle_Y * Circle2_Middle_Y;
    Y1 = Circle1_Middle_Y * Circle1_Middle_Y;
    X2 = Circle2_Middle_X * Circle2_Middle_X;
    X1 = Circle1_Middle_X * Circle1_Middle_X;
    Y2x2 = 2.00 * (Circle2_Middle_Y - Circle1_Middle_Y);

    Line_S1S2_b = (UpArm2 - LowArm2 + Y2 - Y1 + X2 - X1) / (2.00 * (Circle2_Middle_Y - Circle1_Middle_Y));

    /*
    Serial.print(" UpArm2 ");
    Serial.print(UpArm2);
    Serial.print(" LowArm2 ");
    Serial.print(LowArm2);
    Serial.print(" Y2 ");
    Serial.print(Y2);
    Serial.print(" Y1 ");
    Serial.print(Y1);
    Serial.print(" X2 ");
    Serial.print(X2);
    Serial.print(" X1: ");
    Serial.print(X1);
    Serial.print("( Circle1_Middle_X: ");
    Serial.print(Circle1_Middle_X);

    Serial.print(")/ Y2x2 ");
    Serial.println(Y2x2);
    Serial.print("(UpArm2 - LowArm2 + Y2 - Y1 + X2 - X1): ");
    Serial.println((UpArm2 - LowArm2 + Y2 - Y1 + X2 - X1));
*/

    /* Serial.print("Io2C: y=mx+b:  ");
    Serial.print(" y= ");
    Serial.print(Line_S1S2_m);
    Serial.print(" * x +");
    Serial.println(Line_S1S2_b);
    Serial.print(" = ");
    Serial.print(Lenght_LowArm_Value * Lenght_LowArm_Value);
    Serial.print("- ");
    Serial.print(Lenght_UpArm_Value * Lenght_UpArm_Value);
    Serial.print(" + ");
    Serial.print(Circle2_Middle_Y * Circle2_Middle_Y);
    Serial.print(" - ");
    Serial.print(Circle1_Middle_Y * Circle1_Middle_Y);
    Serial.print(" + ");
    Serial.print(Circle2_Middle_Y * Circle2_Middle_Y);
    Serial.print(" - ");
    Serial.print(Circle1_Middle_X * Circle1_Middle_X);
    Serial.print(") / (");
    Serial.println((2.00 * (Circle2_Middle_Y - Circle1_Middle_Y)));*/

    // Quadratische Gleichung der Form x^2+px+q=0

    pStack = (-Circle1_Middle_X + Line_S1S2_m * (Line_S1S2_b - Circle1_Middle_Y));

    p = 2 * pStack / (1 + Line_S1S2_m * Line_S1S2_m);
    q = (Circle1_Middle_X * Circle1_Middle_X + (Line_S1S2_b - Circle1_Middle_Y) * (Line_S1S2_b - Circle1_Middle_Y) - Lenght_UpArm_Value * Lenght_UpArm_Value) / (1 + Line_S1S2_m * Line_S1S2_m);
    discriminant_2Circle_Value = p * p / 4 - q;
    /*
    Serial.print("Io2C: p: ");
    Serial.print(p);
    Serial.print(" = 2 *");
    Serial.print(pStack);
    Serial.print(" / ");
    Serial.println((1 + Line_S1S2_m * Line_S1S2_m));
    
    Serial.print("Io2C: q = ");
    Serial.print(q);
    Serial.print(" = ");
    Serial.print(Circle1_Middle_X * Circle1_Middle_X);
    Serial.print(" + ");
    Serial.print(Line_S1S2_b - Circle1_Middle_Y);
    Serial.print(" - ");
    Serial.print(Lenght_LowArm_Value * Lenght_LowArm_Value);
    Serial.print(" / ");
    Serial.println(1 + Line_S1S2_m * Line_S1S2_m);

    Serial.print("Io2C: discriminant: p*p/4-q = ");
    Serial.println(discriminant_2Circle_Value);
    */

    if (discriminant_2Circle_Value >= 0) {
      Intersection_I1_X = (-p / 2) + sqrt(discriminant_2Circle_Value);
      Intersection_I2_X = (-p / 2) - sqrt(discriminant_2Circle_Value);

      Serial.print("Io2C: Intersection_I1_X: ");
      Serial.print(Intersection_I1_X);
      Serial.print(" Intersection_I2_X: ");
      Serial.println(Intersection_I2_X);

      // Y=mx+b, Res_X1 und Res_X2 einsetzen
      Intersection_I1_Y = Line_S1S2_m * Intersection_I1_X + Line_S1S2_b;
      Intersection_I2_Y = Line_S1S2_m * Intersection_I2_X + Line_S1S2_b;

      Serial.print("Io2C: Intersection_I1_Y: ");
      Serial.print(Intersection_I1_Y);
      Serial.print(" Intersection_I2_Y: ");
      Serial.println(Intersection_I2_Y);
    }
    if (discriminant_2Circle_Value > 0) return 2;
    if (discriminant_2Circle_Value == 0) return 1;
    if (discriminant_2Circle_Value < 0) return 0;

    //delay(1000);
  }

  void InverseKinematic(bool WhichArm)  //Left Arm = 0, Right Arm = 1
  {

    if (WhichArm == right_Arm) {
      //  Serial.println("IK: Inverse Kinematic right Arm Angle  ");

      angle_RightArm_Value1 = RAD_TO_DEG * atan2((Intersection_I1_Y - 0.00) * 1.00, (Intersection_I1_X - DeltaX_RightArm_Value * 1.00));
      if (angle_RightArm_Value1 < 0) angle_RightArm_Value1 = 360 + angle_RightArm_Value1;  //calculating the positiv angle

      /* Serial.print("IK: 1RightArm°: ");
      Serial.print(angle_RightArm_Value1);
      Serial.print(" =  atan2(");
      Serial.print((Intersection_I1_Y - 0.00) * 1.00);
      Serial.print("/");
      Serial.print((Intersection_I1_X - DeltaX_RightArm_Value) * 1.00);
      Serial.println(") ");*/

      angle_RightArm_Value2 = RAD_TO_DEG * atan2((Intersection_I2_Y - 0.00) * 1.00, (Intersection_I2_X - DeltaX_RightArm_Value * 1.00));
      if (angle_RightArm_Value2 < 0) angle_RightArm_Value2 = 360 + angle_RightArm_Value2;  //calculating the positiv angle

      /* Serial.print(", 2RightArm°: ");
      Serial.println(angle_RightArm_Value2);
    Serial.print(" =  atan2(");
      Serial.print((Intersection_I2_Y - 0.00) * 1.00);
      Serial.print("/");
      Serial.print((Intersection_I2_X - DeltaX_RightArm_Value) * 1.00);
      Serial.println(")");*/
    }

    if (WhichArm == left_Arm) {
      //Serial.println("IK: Inverse Kinematic left Arm Angle  ");

      angle_LeftArm_Value1 = RAD_TO_DEG * atan2((Intersection_I1_Y - 0.00) * 1.00, (Intersection_I1_X - DeltaX_LeftArm_Value * 1.00));
      if (angle_LeftArm_Value1 < 0) angle_LeftArm_Value1 = 360 + angle_LeftArm_Value1;  //calculating the positiv angle

      /* Serial.print("IK: 1LeftArm°: ");
      Serial.print(angle_LeftArm_Value1);
      Serial.print(" =  atan2(");
      Serial.print((Intersection_I1_Y - 0.00) * 1.00);
      Serial.print("/");
      Serial.print((Intersection_I1_X - DeltaX_LeftArm_Value) * 1.00);
      Serial.println(") ");*/


      angle_LeftArm_Value2 = RAD_TO_DEG * atan2((Intersection_I2_Y - 0.00) * 1.00, (Intersection_I2_X - DeltaX_LeftArm_Value * 1.00));
      if (angle_LeftArm_Value2 < 0) angle_LeftArm_Value2 = 360 + angle_LeftArm_Value2;  //calculating the positiv angle
                                                                                        /*
      Serial.print(", 2LeftArm°: ");
      Serial.println(angle_LeftArm_Value2);
      Serial.print(" =  atan2(");
      Serial.print((Intersection_I2_Y - 0.00) * 1.00);
      Serial.print("/");
      Serial.print((Intersection_I2_X - DeltaX_LeftArm_Value) * 1.00);
      Serial.println(")");*/
    }
  }
};

#endif