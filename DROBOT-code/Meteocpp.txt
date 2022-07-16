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

    |--------------------------------------------------------------------------------|*/

#ifndef LibraryDROBOT_h
#define LibraryDROBOT_h
#include "Arduino.h"
#include "M5Core2.h"

#define bouncingTime 50
#define TMotor_Position_min 0
#define TMotor_Position_max 90686



//=========| Instancing Classes: |==================================================
//=========| Generating Object |==================================================
//=========| Classes |==================================================

//=========| Meteo Class:  Meteo  |=============

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

#endif
