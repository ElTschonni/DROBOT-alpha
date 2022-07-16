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

#ifndef Generator_h
#define Generator_h
#include "Arduino.h"
#include "M5Core2.h"

#define bouncingTime 50



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

#endif
