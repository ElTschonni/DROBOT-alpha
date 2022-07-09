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
  |Software:      Arduino IDE 1.8.19 (www.arduino.cc)                               |
  |Beschreibung:  DROBOT Library
  |---------------------------------------------------------------------------------|
  |Beschreibung:  WetterLibrary                                                      |
  |---------------------------------------------------------------------------------|

  | Classes:

  1: R_Trig Class: Raising Trigger
  The value of a Pin can be given tho the method as bool.
  Value Output_positiv will be high if a positive flanc has been detected.
  -Attributes:  Output_positiv
  -Methodes:   void read (bool newInput)

  2: F-Trigg Class:  Falling Trigger
  The value of a Pin can be given tho the method as bool.
  Value Output_positiv will be high if a negative flanc has been detected.
  -Attributes:  Output_positiv
  -Methodes:   void read (bool newInput)

  3: T_FF Class: Toggle FlipFlop
   A triggering clock signal can be given tho the method as bool.
  Inverts the value of Q if the input changes its state from low to high.
  -Attributes:  Q
  -Methodes:   void toggle(bool Clk)

  4: TON Class:  Timer On
  The value of a Pin can be given tho the method as bool.
  The value Q will change from low to high if the Input has been high for a defined duration of time.
  -Attributes:  Q
  -Methodes:     void Run(bool Input01)

  5: TOFF Class: Timer OFF
   The value of a Pin can be given tho the method as bool.
  The value Q will change from high to low if the Input has been low for a defined duration of time.
  -Attributes:  Q
  -Methodes:     void Run(bool Input01)

  6: SignalG Class:  Signalgenerator
    The value of a Pin can be given tho the method as bool.
  The value Q will change from high to low if the Input has been low for a defined duration of time.
  -Attributes:  square
  -Methodes:     void run(float f)

  |--------------------------------------------------------------------------------|*/

#ifndef LibraryDROBOT_h
#define LibraryDROBOT_h
#include "Arduino.h" 
#include "M5Core2.h" 
#include "Adafruit_MCP23X17.h"
#endif


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
//OOP Trigger Class
class R_TRIG {
private:
  bool oldInput;
  // Interface
public:
  // Eigenschaften
  bool Output_positiv;
  // Methoden
  void read(bool newInput) {
    if (oldInput < newInput) {
      Output_positiv = true;
    } else {
      Output_positiv = false;
    }
    oldInput = newInput;
  }
};

//=========| 2: F-Trigg Class:  Falling Trigger  |=============
//OOP Trigger Class

class F_TRIG {
private:
  bool oldInput;
  // Interface
public:
  // Attributes
  bool Output_positiv;
  // Methods
  void read(bool newInput) {
    if (oldInput > newInput) {
      Output_positiv = true;
    } else {
      Output_positiv = false;
    }
    oldInput = newInput;
  }
};
//=========| 3: T_FF Class: Toggle FlipFlop |=============
class T_FF {
private:
  F_TRIG RT_Clk;  //Checking the raising Trigger of the clock signal
public:
  //Attributes
  bool Q;  //Output Value
  //Methods
  void toggle(bool Clk) {
    RT_Clk.read(Clk);  //Check if there is a raising trigger

    if (RT_Clk.Output_positiv) {  //If there is a raising trigger, toggle Q
      Q = not Q;
    }
  }
};

//=========| 4: TON Class:  Timer On  |=============

class TON {
private:
  unsigned long StartTime;

public:
  //Attributes
  bool Q;
  //Methods
  void Run(bool Input01) {
    if (not Input01) {       // Timer gesperrt
      StartTime = millis();  // Timer resetten, d.h. immer auf aktueller Zeit halten
      Q = false;
    }
    if (StartTime + bouncingTime < millis()) {
      Q = Input01;
    }
  }
};


//=========| 5: TOFF Class:  Timer OFF  |=============

class TOFF {
private:
  unsigned long StartTime;

public:
  //Attributes
  bool Q;
  //Methods
  void Run(bool Input01) {
    if (Input01) {           // Timer gesperrt
      StartTime = millis();  // Timer resetten, d.h. immer auf aktueller Zeit halten
      Q = true;
    }
    if (StartTime + bouncingTime < millis()) {
      Q = Input01;
    }
  }
};

//=========| 6: SignalG Class:  Signalgenerator  |=============

class SignalG {
private:
  unsigned long lastMeasurment;
  float Ts;  //sample Time
  float T;   //Time
  float Tp;  //Time of a Periode


public:
  //Attributes
  bool square;
  //Methods
  void run(float f) {  //f = frequency
    Tp = 1 / f;

    Ts = (millis() - lastMeasurment) / 1000.0;
    lastMeasurment = millis();
    T = T + Ts;
    if (T >= Tp) T = 0;

    //Square Generator
    if (T / Tp < 0.5) square = false;
    else {
      square = true;
    }

    //Sine Generator
  }
};

//=========| 7: Meteo Class:  Meteo  |=============

class Meteo {
private:
  unsigned long TMotor1_Position_now = TMotor_Position_max;  //Position of Motor1 between step  step 0 20ml - step 122880 50ml
  unsigned long TMotor2_Position_now = TMotor_Position_max;  //(T) Position of Motor2  between step 0  20ml - step 122880 50ml
  unsigned long TMotor1_Position_old;                        //Position of Motor1 between step  step 0 20ml - step 122880 50ml
  unsigned long TMotor2_Position_old;                        //(T) Position of Motor2  between step 0  20ml - step 122880 50ml
  unsigned long Motor_Position_change;                       //(T) Position of Motor1  between step 0  20ml - step 122880 50ml


  //BME280 Values
  float TSensor1_Value_old = 0;  //new Measured Temperature Value from Sensor 1 BME280
  float TSensor1_Value_new = 0;  //new Measured Temperature Value from Sensor 1 BME280

  float HSensor1_Value_old = 0;  //new Measured Humitidy Value from Sensor 1 BME280
  float HSensor1_Value_new = 0;  //new Measured Humitidy Value from Sensor 1 BME280

  float PSensor1_Value_old = 0;  //new Measured Prssure Value from Sensor 1 BME280
  float PSensor1_Value_new = 0;  //new Measured Prssure Value from Sensor 1 BME280

  float ASensor1_Value_old = 0;  //new Measured Altitude Value from Sensor 1 BME280
  float ASensor1_Value_new = 0;  //new Measured Altitude Value from Sensor 1 BME280

  //BMP280 Values
  float TSensor2_Value_old = 0;  //new Measured Temperature Value from Sensor 2 BMP280
  float TSensor2_Value_new = 0;  //new Measured Temperature Value from Sensor 2 BMP280

  float PSensor2_Value_old = 0;  //new Measured Pressure Value from Sensor 2 BMP280
  float PSensor2_Value_new = 0;  //new Measured Pressure Value from Sensor 2 BMP280


  //DB130 Values
  float LSensor3_Value_old = 0;  //new Measured Lux Value from Sensor 3 DB130
  float LSensor3_Value_new = 0;  //new Measured Lux Value from Sensor 3 DB130



public:
  //Attributes


  //Methods

  //-----Sensor Value Methodes -----
  // void setNewVal(int which_Sensor, float temperature_value)
  // float getNewVal(int which_Sensor)
  // float getOldVal(int which_Sensor)
  // bool lookupValChange (int which_Sensor)

  //Sensors:
  //1 = BME280_Temp_Sensor;
  //2 = BME280_Pres_Sensor;
  //3 = BME280_Humi_Sensor;
  //4 = BME280_Alti_Sensor;
  //5 = BMP280_Temp_Sensor;
  //6 = BMP280_Pres_Sensor;
  //7 = DB130_Light_Sensor;


  //-----Motor Methodes -----

  //Temperature Methodes
  void setNewVal(int which_Sensor, float sensor_value)  //1 = sensor BMP280 / 2 = sensor SHT10
  {
    switch (which_Sensor) {
      case 1:
        TSensor1_Value_old = TSensor1_Value_new;
        TSensor1_Value_new = sensor_value;
        break;

      case 2:
        PSensor1_Value_old = PSensor1_Value_new;
        PSensor1_Value_new = sensor_value;
        break;

      case 3:
        HSensor1_Value_old = HSensor1_Value_new;
        HSensor1_Value_new = sensor_value;
        break;

      case 4:
        ASensor1_Value_old = ASensor1_Value_new;
        ASensor1_Value_new = sensor_value;
        break;

      case 5:
        TSensor2_Value_old = TSensor2_Value_new;
        TSensor2_Value_new = sensor_value;
        break;

      case 6:
        PSensor2_Value_old = PSensor2_Value_new;
        PSensor2_Value_new = sensor_value;
        break;

      case 7:
        LSensor3_Value_old = LSensor3_Value_new;
        LSensor3_Value_new = sensor_value;
        break;
    }
  }

  float getNewVal(int which_Sensor) {
    switch (which_Sensor) {
      case 1: return (TSensor1_Value_new); break;
      case 2: return (PSensor1_Value_new); break;
      case 3: return (HSensor1_Value_new); break;
      case 4: return (ASensor1_Value_new); break;
      case 5: return (TSensor2_Value_new); break;
      case 6: return (PSensor2_Value_new); break;
      case 7: return (LSensor3_Value_new); break;
    }
  }

  float getOldVal(int which_Sensor) {
    switch (which_Sensor) {
      case 1: return (TSensor1_Value_old); break;
      case 2: return (PSensor1_Value_old); break;
      case 3: return (HSensor1_Value_old); break;
      case 4: return (ASensor1_Value_old); break;
      case 5: return (TSensor2_Value_old); break;
      case 6: return (PSensor2_Value_old); break;
      case 7: return (LSensor3_Value_old); break;
    }
  }

  bool lookupValChange(int which_Sensor) {
    switch (which_Sensor) {
      case 1:
        if (TSensor1_Value_new == TSensor1_Value_old) return (false);
        else {
          return (true);
        }
        break;

      case 2:
        if (PSensor1_Value_new == PSensor1_Value_old) return (false);
        else {
          return (true);
        }
        break;

      case 3:
        if (HSensor1_Value_new == HSensor1_Value_old) return (false);
        else {
          return (true);
        }
        break;

      case 4:
        if (ASensor1_Value_new == ASensor1_Value_old) return (false);
        else {
          return (true);
        }
        break;

      case 5:
        if (TSensor2_Value_new == TSensor2_Value_old) return (false);
        else {
          return (true);
        }
        break;

      case 6:
        if (PSensor2_Value_new == PSensor2_Value_old) return (false);
        else {
          return (true);
        }
        break;

      case 7:
        if (LSensor3_Value_new == LSensor3_Value_old) return (false);
        else {
          return (true);
        }
        break;
    }
  }



  //Add steps to motor position
  void setTMotorPos(int which_Motor, long toWhichPos)  //1 = TMotor1 / 2 = TMotor2
  {
    switch (which_Motor) {
      case 1: TMotor1_Position_now = toWhichPos; break;
      case 2: TMotor2_Position_now = toWhichPos; break;
    }
  }

  void changeTMotorStep(int which_Motor, long howManySteps)  //1 = TMotor1 / 2 = TMotor2
  {
    switch (which_Motor) {
      case 1:
        TMotor1_Position_old = TMotor1_Position_now;
        TMotor1_Position_now = TMotor1_Position_now + howManySteps;
        break;
      case 2:
        TMotor2_Position_old = TMotor2_Position_now;
        TMotor2_Position_now = TMotor2_Position_now + howManySteps;
        break;
    }
  }
  //get steps value of motor position
  unsigned long getTMotorPosNow(int which_Motor)  //1 = TMotor1 / 2 = TMotor2
  {
    switch (which_Motor) {
      case 1: return (TMotor1_Position_now); break;
      case 2: return (TMotor2_Position_now); break;
    }
  }

  unsigned long getTMotorPos_old(int which_Motor)  //1 = TMotor1 / 2 = TMotor2
  {
    switch (which_Motor) {
      case 1: return (TMotor1_Position_old); break;
      case 2: return (TMotor2_Position_old); break;
    }
  }

  //change request information

  long changeRequestTMotor(int which_Sensor)  //returns a value of steps which need to be changed, to sync with temperature Value
  {
    switch (which_Sensor) {  //temperature
      case 1:
        Motor_Position_change = (TSensor1_Value_new - TSensor1_Value_old) * (TMotor_Position_max / 50);  //1°C = 0.4ml = 3.6mm = 1667 steps
        return (Motor_Position_change);
        break;
      case 2:
        Motor_Position_change = (PSensor1_Value_new - PSensor1_Value_old) * (TMotor_Position_max / 150);  //1hPa =
        return (Motor_Position_change);
        break;
      case 3:
        Motor_Position_change = (HSensor1_Value_new - HSensor1_Value_old) * (TMotor_Position_max / 100);  //1°C = 0.4ml = 3.6mm = 1667 steps
        return (Motor_Position_change);
        break;
      case 4:
        Motor_Position_change = (ASensor1_Value_new - ASensor1_Value_old) * (TMotor_Position_max / 4800);  //1°C = 0.4ml = 3.6mm = 1667 steps
        return (Motor_Position_change);
        break;
      case 5:
        Motor_Position_change = (TSensor2_Value_new - TSensor2_Value_old) * (TMotor_Position_max / 50);  //1°C = 0.4ml = 3.6mm = 1667 steps
        return (Motor_Position_change);
        break;
      case 6:
        Motor_Position_change = (PSensor2_Value_new - PSensor2_Value_old) * (TMotor_Position_max / 150);  //1°C = 0.4ml = 3.6mm = 1667 steps
        return (Motor_Position_change);
        break;
      case 7:
        Motor_Position_change = (LSensor3_Value_new - LSensor3_Value_old) * (TMotor_Position_max / 10000);  //1°C = 0.4ml = 3.6mm = 1667 steps
        return (Motor_Position_change);
        break;
    }
  }
};

//=========| 8: CLSM Class: Closed_Loop_Step_Motor  |=============
class Closed_Loop_Step_Motor  //CLSM
{
private:
  int us_timer_state;
  long double us_timer_value;
  long double us_timer_start;
  //Motor Pin Configuration
  int Motor_Enable_Pin;  //Hardware configuration of the Enable pin
  int Motor_Pull_Pin;    //Pull Pin Puls to move by one step
  int Motor_Dir_Pin;     //Direction pin - set to high to move right, low for left
  int Motor_Alarm_Pin;   //Alarm Pin
  int Motor_Ped_Pin;     // Confirmation bit for reaching the new stepp

Adafruit_MCP23X17 I2C_Exp_Class2;
 




  //Motor Pin Values
  bool Motor_Enable_Value = false;
  bool Motor_Pull_Value = false;
  bool Motor_Dir_Value = Motor_Turn_right;
  bool Motor_Alarm_Value = false;
  bool Motor_Ped_Value = false;
  bool Motor_Position_Value = Motor_Position_right;  //1 = right. 0 = left

  int Dip_Switch_Value = 0;
  /* 1 = Alarm Pin Motor
    2 = Old Angle Value Wrong
    3=  New Angle Value Wrong
    4= Step failed
    5= 
    6= 
    7= 
    8= 
    9= 
    10= 
  */

  //according to the Datasheet iHSS57
  double Dip_Switch_Array[16][2] = {
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
  double Angle_Per_step = 0;
  double Steps_Per_rev = 0;

  int errorcode = 0;

  //Methods
  //Method to configure the Motor Object. - Hardware Pin Mapping and DipSwitch Position
  void setupMotor(bool MotorPos, int EnPin, int PullPin, int DirPin, int AlmPin, int PedPin, int DipSwitch) {  //Motor Pin Configuration
    Motor_Enable_Pin = EnPin;
    Motor_Pull_Pin = PullPin;
    Motor_Dir_Pin = DirPin;
    Motor_Alarm_Pin = AlmPin;
    Motor_Ped_Pin = PedPin;
    Dip_Switch_Value = DipSwitch;
    Angle_Per_step = Dip_Switch_Array[DipSwitch][2];  // Read Value Angle per Step from DipSwitch Array
    Steps_Per_rev = Dip_Switch_Array[DipSwitch][1];   //Read Value Steps per Revolution from Dip Switch Array
    Motor_Position_Value = MotorPos;



  }

  void test(){
    Serial.println("MotorClassTest: Turn on LED 15");
  M5.lcd.println("MotorClassTest: Turn on LED 15");
  //I2C_Exp_Class2.begin_I2C();
  I2C_Exp_Class2.digitalWrite(0, LOW);
}

  void enableMotor() {
    Motor_Enable_Value = true;
    Serial.println("MotorEnabled");
    M5.lcd.println("MotorEnabled");
  }
  void disableMotor() {
    Motor_Enable_Value = false;
    Serial.println("MotorDisabled");
    M5.lcd.println("MotorDisabled");
  }

  bool startSurvilance() {
  I2C_Exp_Class2.begin_I2C();
    if (!digitalRead(Motor_Alarm_Pin)) {
      Motor_Enable_Value = false;
      Serial.println("ALARM");
      M5.lcd.println("ALARM");
      errorcode = 1;
      return (0);
    } else {
      return (1);
    };
    I2C_Exp_Class2.digitalWrite(Motor_Enable_Pin, Motor_Enable_Value);
  }


  bool setOldAngelValue(double Angle) {

    while (Angle >= 360) { Angle = Angle - 360; }                            //keeps Angle below 360°
    if ((Motor_Position_Value == 1) and (Angle <= 90) and (Angle >= 270)) {  //if motorposition is right / 90°=>x<=270°
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
    if (Motor_Position_Value and (Angle <= 90) and (Angle >= 270)) {  //if motorposition is right / 90°=>x<=270°
      New_Angle_Value = Angle;
      Serial.print("setNewAngleValue r ");
      Serial.println(New_Angle_Value);
      M5.lcd.print("setNewAngleValue r ");
      M5.lcd.println(New_Angle_Value);
      return (1);
    } else if (!Motor_Position_Value and (Angle >= 90) and (Angle <= 270))  //if motorposition is left / 90°=<x<=270°
    {
      New_Angle_Value = Angle+90;    //Add 90° to make it positiv.
      Serial.print("setNewAngleValue l ");
      Serial.println(New_Angle_Value);
      M5.lcd.print("setNewAngleValue l ");
      M5.lcd.println(New_Angle_Value);
       while (Angle >= 360) { Angle = Angle - 360; }  //if Angle between 270 and 360, it will be changed to a value between 0 and 90
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
    long int counter;
    I2C_Exp_Class2.begin_I2C();
    I2C_Exp_Class2.digitalWrite(Motor_Dir_Pin, direction);  //set direction 1= right 0 =left
    delayMicroseconds(5);
    I2C_Exp_Class2.digitalWrite(Motor_Pull_Pin, 1);  //send one Pulse
    delayMicroseconds(5);
    I2C_Exp_Class2.digitalWrite(Motor_Pull_Pin, 0);                                            // maybe needs to be an Interupt
    while (digitalRead(!Motor_Ped_Pin) and (counter < 100000)) { counter++; };  //wait until position has been reached
    if (counter > 10000) {
      Serial.println("Warning: Step took too much time");  //Warning it took more than 10000 tries
      M5.lcd.println("Warning: Step took too much time");
    }
    return (1);  //it was successfull
    if (counter > 90000) {
      return (0);  //error - try again
      Serial.println("Error: Step failed");
      M5.lcd.println("Error: Step failed");
      errorcode = 4;
    }
  }

  bool OneStepDir(/*bool direction*/) {
    bool I_did_one_Step = false;
    long int counter = 0;
    enableMotor();

    if (startSurvilance()) {

      if (Old_Angle_Value != New_Angle_Value) {
        Serial.print("Old_Angle_Value is not equal to New_Angle_Value");
        Serial.print(New_Angle_Value);
        Serial.print(" ");
        Serial.println(Old_Angle_Value);

        M5.lcd.println("Old_Angle_Value is not equal to New_Angle_Value");
        M5.lcd.print(New_Angle_Value);
        M5.lcd.print(" ");
        M5.lcd.println(Old_Angle_Value);
        Serial.println("Tryng to fix it");
        M5.lcd.println("Tryng to fix it");

        if (New_Angle_Value > Old_Angle_Value) {
          while ((I_did_one_Step == 0) and (counter < 100000)) {  // try as until you did one Stepbut try not more than 100000 times.
            I_did_one_Step = moveOneStep(Motor_Turn_right);
            counter++;
          };
          counter = 0;
          I_did_one_Step = 0;
        } else if (New_Angle_Value < Old_Angle_Value) //if the new Angle is smaller than the old turn the motor left to make the difference smaller.
        {
          while ((I_did_one_Step == 0) and (counter < 100000)) {  // try as until you did one Stepbut try not more than 100000 times.
            I_did_one_Step = moveOneStep(Motor_Turn_left);
            counter++;
          };
          counter = 0;
          I_did_one_Step = 0; //normaly 0
        }
      }
    }
  }
};
