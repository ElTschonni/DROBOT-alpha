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
2022-07
  |---------------------------------------------------------------------------------|
  //=========| include librarys |==================================================*/
#include "LibraryDROBOT.h"  // Robot controll
#include "Wire.h"           // I2C
#include "M5Core2.h"
#include "WiFi.h"
#include "ModbusIP_ESP8266.h"
#include "WiFiMulti.h"
#include <Adafruit_MCP23X17.h>



//=========| definiere Werte |==================================================*/
#define ON HIGH
#define OFF LOW
#define baudRate 115200  //set baud rate for serial communiction

#define WLAN_SSID "Wildbachhuis 2.4"  // define: suchen und ersetzen im Code "L-TEKO"
#define WLAN_PASSWD "Milkyway29!"     //"teko2016"
#define MCP23017_ADDR 0x20            //I2C Addrese
#define I2C_Clock_Speed 1700000       // 1.7 MHz


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
const int BEFEHL_HREG = 0;  // Hreg 0 f??r den Befehl, z.B. G01
const int X_HREG = 1;       // Hreg 1 f??r die X-Koordinate
const int Y_HREG = 2;       // Hreg 2 f??r die Y-Koordinate
const int Z_HREG = 3;       // Hreg 3 f??r die Z-Koordinate
const int F_HREG = 4;       // Hreg 4 f??r die Geschwindigkeit
const int STATUS_HREG = 5;  // Hreg 5 f??r den Zustand der StateMachine


//=========| initializing Classes |==================================================*/
//ModbusIP object
ModbusIP mb;
WiFiMulti wiFiMulti;
Adafruit_MCP23X17 I2C_Exp_Class;  //creating the MCP23017 Class
Closed_Loop_Step_Motor Motor_Left_Class;//creating the Stepper Motor Class

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

//==============================================================================*/
//=========| Funktionen ModBus |=========================================*/

//Return new GCode_Command ReadModbusRegister CBneuerBefehl


//Return new Client_Command ReadModbusRegister CBneuerBefehl
uint16_t CBneuerBefehl(TRegister* reg, uint16_t val) {
  if (Start_StateMachine == false) {
    Serial.println("Neuer Befehl!");
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


//=========| Funktionen StateMachine |=========================================*/



int Standby()  //State1
{
  Serial.println("------ Standby -------");

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
  I2C_Exp_Class.digitalWrite(Clamp_Servo_Pin, HIGH);



  return (2);
}

int Receive()  //Verarbeitung
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
      Xneu = X;  // Neues X von Modbus
      Yneu = Y;  // Neues Y von Modbus
      Alpha = 0.0;
      Status = 2;  // gehe zu busy
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
  delay(500);
  Serial.print("Robot is Calculationg Coordinates ");
  Motor_Left_Class.test();//Motor Class Test
  delay(500);
  Serial.print("Robot is Calculationg Route ");
  delay(500);
  Serial.print("Robot is drawing ");
  delay(500);

  Start_StateMachine = false;
  Status = 1;

  return (1);
}

int WrongCommand()  //Programm 4, steuert die Motoren
{
  Serial.println("------ Wrong Command -------");
  Status = 1;
  return (1);
}

int Error() {
  Serial.println("------ Error -------");

  return (1);
}


int Init() {
  Serial.println("------ Initialize -------");


  return (7);
}

int Calibrate() {
  Serial.println("------ Calibration -------");


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

  if (!I2C_Exp_Class.begin_I2C()) {
    Serial.println("Error.I2C");
    M5.lcd.println("Error.I2C");
    while (1)
      ;
  }
  //Pinconfiguration :
  Serial.println("-I2C_ Set Input/Output Pins");
  M5.lcd.println("-I2C_ Set Input/Output Pins ");

  I2C_Exp_Class.pinMode(Mright_Pull_Pin, OUTPUT);           //	Pull+ Blue Motor Left
  I2C_Exp_Class.pinMode(Mright_Dir_Pin, OUTPUT);            //	Dir+ White Motor Left
  I2C_Exp_Class.pinMode(Mright_En_Pin, OUTPUT);             //	En + BrownBlack Motor Left&Right
  I2C_Exp_Class.pinMode(Mright_Alarm_Pin, INPUT_PULLUP);    //	Alarm+ Whiteblack Motor Left
  I2C_Exp_Class.pinMode(Mright_Ped_Pin, INPUT_PULLUP);      //	Ped+ GreenBlack Motor Left
  I2C_Exp_Class.pinMode(Reed_Sensor_Pin, INPUT_PULLUP);     //	Reed / Push Sensor
  I2C_Exp_Class.pinMode(Mvertical_Down_Pin, INPUT_PULLUP);  //	Vertical Down
  I2C_Exp_Class.pinMode(Mvertical_Up_Pin, OUTPUT);          //	Vertical Up
  I2C_Exp_Class.pinMode(Clamp_Servo_Pin, OUTPUT);           //	Clamp Orange Servo
  I2C_Exp_Class.pinMode(End_Button2_Pin, OUTPUT);           //	End_Button 2 left
  I2C_Exp_Class.pinMode(End_Button1_Pin, OUTPUT);           //	End_Button 1 right
  I2C_Exp_Class.pinMode(Mleft_Ped_Pin, INPUT_PULLUP);       //	Ped+ GreenBlack Motor Left
  I2C_Exp_Class.pinMode(Mleft_Alarm_Pin, INPUT_PULLUP);     //	Alarm+ Whiteblack Motor Left
  I2C_Exp_Class.pinMode(Mleft_En_Pin, INPUT_PULLUP);        //	En + BrownBlack Motor Left&Right
  I2C_Exp_Class.pinMode(Mleft_Dir_Pin, OUTPUT);             //	Dir+ White Motor Left
  I2C_Exp_Class.pinMode(Mleft_Pull_Pin, OUTPUT);            //	Pull+ Blue Motor Left


  Serial.println("-I2C: Turn on LED 15");
  M5.lcd.println("-I2C: Turn on LED 15");
  I2C_Exp_Class.digitalWrite(Clamp_Servo_Pin, LOW);

  Serial.println("-I2C: wait 10s");
  M5.lcd.println("-I2C: wait 10s");
  delay(10000);
  Serial.println("-I2C: Turn off LED 15");
  M5.lcd.println("-I2C: Turn off LED 15");
  I2C_Exp_Class.digitalWrite(Clamp_Servo_Pin, HIGH);




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

  delay(5000);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
}

//=========| Hauptprogramm    |=================================================*/
void loop() {
int cycleCounter =0;
  int state = 1;

  while (1) {
    state = programSelector;
    M5.update();
    if(cycleCounter>15)  {   
       M5.Lcd.fillScreen(BLACK); 
       M5.Lcd.setCursor(0, 0);
       cycleCounter=0;
       
       }
cycleCounter++;

    switch (state) {
      case 1: state = Standby(); break;  //Standby

      case 2: state = Receive(); break;  //receive new Coordinates

      case 3: state = Draw(); break;  //Draw according to the new command

      case 4: state = WrongCommand(); break;  //Received a Wrong Command

      case 5: state = Error(); break;  //Test Modus, Teil 3 (Leistungsmessung mit Last)

      case 6: state = Init(); break;  //Test Modus, Teil 4 (??berpr??fung und Anpassung der Daten)

      case 7: state = Calibrate(); break;  //Test Modus, Teil 4 (??berpr??fung und Anpassung der Daten)
    }
    programSelector = state;
  }
}




//==============================================================================

//interupt service routinen:
/*---| starte Interrupt Routinen |-----
  attachInterrupt( 3, check_S3DModul1, CHANGE);        //wenn sich Pin  3, Schalter S3D auf dem Modul 1 ??ndert, wird die ISR ausgef??hrt
  attachInterrupt( 6, check_S4DModul2, CHANGE);        //wenn sich Pin  6, Schalter S4D auf dem Modul 2 ??ndert, wird die ISR ausgef??hrt
  attachInterrupt( 9, check_S5DModul3, CHANGE);        //wenn sich Pin  9, Schalter S5D auf dem Modul 3 ??ndert, wird die ISR ausgef??hrt
  attachInterrupt(12, check_S6DModul4, CHANGE);        //wenn sich Pin 12, Schalter S6D auf dem Modul 4 ??ndert, wird die ISR ausgef??hrt
  attachInterrupt(24, check_AttachedModul1, CHANGE);   //24,
  attachInterrupt(27, check_AttachedModul2, CHANGE);   //27,
  attachInterrupt(30, check_AttachedModul3, CHANGE);   //30,
  attachInterrupt(33, check_AttachedModul4, CHANGE);   //33
*/