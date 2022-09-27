/*******************************************************
   Roboter-Modbus-Client
   =====================
   G-Code via Modbus
   andreas.holzer@edu.teko.ch
   20220613    v01_01: Initiale Version
   20220613    v01_02: Modbus-Verbindung erstellt (setup)
   20220620    v01_03: Modbus-write
   20220620    v01_04: Random-Werte generieren zu Testzwecken
   20220620    v01_05: Zustandsmaschine
   20220620    v01_06: Reconnect & Fehlerbehandlung
   20220705    v01_07: Parser integriert
   20220705    v01_08: M5.update()
*/

/////////////////////////////////////////////////////////
// Bibliotheken einfügen
#include <M5Stack.h>
#include <WiFi.h>
#include <ModbusIP_ESP8266.h>
#include <SPI.h>
#include <SD.h>

#include "time.h"

/////////////////////////////////////////////////////////
// Datei auf SD-Karte
File myFile;
String Zeilen;
int G, X, Y, Z, F;
unsigned int ROW = 0;

// Time
struct tm timeinfo;
const char* ntpServer = "0.ch.pool.ntp.org";  //0.ch.pool.ntp.org
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

/////////////////////////////////////////////////////////
// Modbus-Register
// G01 X30.00 Y100.00 Z10.00 F100
const int BEFEHL_HREG = 0;  // Hreg 0 für den Befehl, z.B. G01
const int X_HREG = 1;       // Hreg 1 für die X-Koordinate
const int Y_HREG = 2;       // Hreg 2 für die Y-Koordinate
const int Z_HREG = 3;       // Hreg 3 für die Z-Koordinate
const int F_HREG = 4;       // Hreg 4 für die Geschwindigkeit
const int STATUS_HREG = 5;  // Hreg 5 für den Zustand der StateMachine
unsigned int oldState = 0;

/////////////////////////////////////////////////////////
// Modbus-Verbindungsdaten
IPAddress RoboterIP(192, 168, 40, 69);  //IP-Adresse des Roboters
ModbusIP mb;                           //ModbusIP object
#define WLAN_SSID "L-TEKO"   // define: suchen und ersetzen im Code
#define WLAN_PASSWD "teko2016"      //teko2016

/////////////////////////////////////////////////////////
// Setup-Routine
void setup() {
  M5.begin(true, true, true);
  M5.Power.begin();
  Serial.println("Connecting WiFi...");
  WiFi.begin(WLAN_SSID, WLAN_PASSWD);      // Verbinden mit Access Point
  while (WiFi.status() != WL_CONNECTED) {  // Warte, bis WiFi verbunden ist
    delay(500);
    Serial.print(".");
  }

  // WiFi ist verbunden, meine IP anzeigen
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Startet Modbus-Client");
  mb.client();  // Modbus-Client starten

  Serial.println("Verbinden mit Roboter");
  mb.connect(RoboterIP);  // Try to connect if no connection
  while (!mb.isConnected(RoboterIP)) {
    mb.connect(RoboterIP);  // Try to connect if no connection
    delay(500);
    Serial.print(".");
  }

  mb.autoConnect(true);

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("Note: press reset button on the board and reopen this serial monitor after fixing your issue!");
    while (1)
      ;
  }
  Serial.println("initialization done.");

  if (SD.exists("TestJe.txt")) {
    Serial.println("TestJe.txt exists.");
  } else {
    Serial.println("Test.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists:
  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  } else {
    Serial.println("example.txt doesn't exist.");
  }

  // delete the file:
  Serial.println("Removing example.txt...");
  SD.remove("example.txt");

  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  } else {
    Serial.println("example.txt doesn't exist.");
  }

  //Get Time

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  //init and get the time
  getLocalTime(&timeinfo);
  Serial.print("-Time: ");
  Serial.print(&timeinfo);
  M5.lcd.print("-Time: ");
  M5.lcd.print(&timeinfo);

  Serial.println("!StartUp Complete!");

  delay(1000);
  M5.Lcd.fillScreen(DARKCYAN);
  M5.Lcd.setCursor(0, 0);
}


//////////////////////////////////////////////////////////
// Variablen
uint16_t Roboter_Status = 0;    // TEL
uint16_t Steuerung_Status = 0;  // TMA
unsigned long Startzeit;
#define SENDETIMEOUT 10000    // in Millisekunden
#define BEFEHLSTIMEOUT 180000  // in Millisekunden

uint16_t Xrob = 0;
uint16_t Yrob = 0;
uint16_t Zrob = 0;
uint16_t Frob = 0;




/////////////////////////////////////////////////////////
// Loop (Hauptschleife)
void loop() {
  M5.update();
  if (!mb.isConnected(RoboterIP)) {
    mb.connect(RoboterIP);  // Try to connect if no connection
    M5.Lcd.fillRect(10, 80, 320, 20, DARKCYAN);
    M5.Lcd.setCursor(10, 80);
    M5.Lcd.setTextColor(RED);

    M5.lcd.print("Con. to Rob: ");
    M5.lcd.print(RoboterIP);

    delay(500);
  } else {
    M5.Lcd.fillRect(10, 80, 320, 20, DARKCYAN);
    M5.Lcd.setCursor(10, 80);
    M5.Lcd.setTextColor(BLACK);
    M5.lcd.print("Robot_IP:  ");
    M5.lcd.print(RoboterIP);
  }

  mb.readHreg(RoboterIP, STATUS_HREG, &Roboter_Status,1);  // Liest Status vom Modbus und speichert Resultat in STATUS
    Serial.print("mb: read HReg -> ");



  ///////////////////////////////////////////////// UI Display
  M5.Lcd.setTextColor(NAVY);
  M5.Lcd.fillRect(6, 215, 100, 30, DARKGREEN);
  M5.Lcd.setCursor(16, 220);
  M5.Lcd.print("SEND");

  M5.Lcd.fillRect(109, 215, 100, 30, DARKGREEN);
  M5.Lcd.setCursor(119, 220);
  M5.Lcd.print("");
  M5.Lcd.fillRect(212, 215, 100, 30, DARKGREEN);
  M5.Lcd.setCursor(225, 220);
  M5.Lcd.print("");

  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(BLACK);
  M5.lcd.println("Client is Running");
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.print("Client_IP: ");
  M5.Lcd.println(WiFi.localIP());

  delay(100);
  M5.Lcd.fillRect(10, 40, 240, 20, DARKCYAN);
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.setTextColor(BLACK);
  getLocalTime(&timeinfo);
  M5.lcd.print(&timeinfo);


  if (oldState != 1) {

    M5.Lcd.fillRect(10, 100, 240, 20, DARKCYAN);
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.setTextColor(BLACK);

    M5.lcd.print("Steuerung_Status: ");
    M5.lcd.print(Steuerung_Status);
    Serial.print("loop: Steuerung_Status=");
    Serial.print(Steuerung_Status);

    M5.Lcd.fillRect(10, 120, 320, 20, DARKCYAN);
    M5.Lcd.setCursor(10, 120);
    M5.Lcd.setTextColor(BLACK);

    M5.lcd.print("Roboter_Status  : ");
    M5.lcd.print(Roboter_Status);
    Serial.print(" Roboter_Status=");
    Serial.print(Roboter_Status);
    oldState = 1;
  }



  // Steuerung ist IDLE
  if (Steuerung_Status == 0) {
    if (M5.BtnA.isPressed()) {
      Serial.println("loop: Datei /TestJe.txt öffnen");
      myFile = SD.open("/TestJe.txt");
      if (myFile) {
        Serial.println("Datei /TestJe.txt konnte geöffnet werden!");

        M5.Lcd.fillRect(10, 140, 240, 20, DARKCYAN);
        M5.Lcd.setCursor(10, 140);
        M5.Lcd.setTextColor(BLACK);
        M5.lcd.print("G-Code File open!");


        Steuerung_Status = 1;
      } else {
        Serial.println("loop: Datei /TestJe.txt konnte nicht geöffnet werden!");
        M5.Lcd.fillRect(10, 140, 240, 20, DARKCYAN);
        M5.Lcd.setCursor(10, 140);
        M5.Lcd.setTextColor(BLACK);
        M5.lcd.print("G-Code File not open!");
      }
    }
    oldState = 0;
  }
  /////////////////////////////////////////////////
  // Steuerung ist im Zustand "File is open"
  else if (Steuerung_Status == 1) {
    if (Roboter_Status == 1) {
      // Roboter ist bereit für neuen Befehl
      Steuerung_Status = 2;  // Transition zum Zustand 2
      // Zeitpunkt der Transition festhalten
      Startzeit = millis();
      // neuen Befehl senden
      Serial.println("loop: Neuer Befehl");  // Gibt Status auf Serial Monitor aus

      ////////////////myFile//////////////////////////////////////////
      // Read, Parse und Send
      if (myFile.available()) {


        // es hat Zeilen zu lesen
        Zeilen = myFile.readStringUntil('\n');
        Serial.print("St1: Zeilen=");
        Serial.println(Zeilen);



        M5.Lcd.fillRect(220, 140, 80, 20, DARKCYAN);
        M5.Lcd.setCursor(220, 140);
        M5.Lcd.setTextColor(BLACK);
        M5.lcd.print("Row:");
        M5.lcd.print(ROW);

        // Parsen

        Serial.println("St1: indexOf");
        
        unsigned int Pos_G = Zeilen.indexOf('G');
        unsigned int Pos_X = Zeilen.indexOf('X');
        unsigned int Pos_Y = Zeilen.indexOf('Y');
        unsigned int Pos_Z = Zeilen.indexOf('Z');
        unsigned int Pos_F = Zeilen.indexOf('F');
        Serial.print("Pos1: G: ");
        Serial.print(Pos_G);
        Serial.print(" X: ");
        Serial.print(Pos_X);
        Serial.print(" Y: ");
        Serial.print(Pos_Y);
        Serial.print(" Z: ");
        Serial.print(Pos_Z);
        Serial.print(" F: ");
        Serial.println(Pos_F);



        Serial.println("substring");
        String string_G = Zeilen.substring(Pos_G+1);
        String string_X = Zeilen.substring(Pos_X+1);
        String string_Y = Zeilen.substring(Pos_Y+1);
        String string_Z = Zeilen.substring(Pos_Z+1);
        String string_F = Zeilen.substring(Pos_F+1);
        Serial.println(string_G);
        Serial.println(string_X);
        Serial.println(string_Y);
        Serial.println(string_Z);
        Serial.println(string_F);

        Serial.println("toInt und toFloat");
        G = string_G.toInt();
        X = int(string_X.toFloat());
        Y = int(string_Y.toFloat());
        Z = int(string_Z.toFloat());
        F = string_F.toInt();

   
        //Go to next new ROW
        Serial.print("Pos2: G: ");
        Serial.print(Pos_G);
        Serial.print(" X: ");
        Serial.print(Pos_X);
        Serial.print(" Y: ");
        Serial.print(Pos_Y);
        Serial.print(" Z: ");
        Serial.print(Pos_Z);
        Serial.print(" F: ");
        Serial.println(Pos_F);




        if (Pos_G >= 0) {
          Serial.println(G);
        } else {
          Serial.println("kein G vorhanden!");
        }
        if (Pos_X >= 0) {
          Serial.println(X);
          // Modbus-Write
        } else {
          Serial.println("kein X vorhanden!");
        }
        if (Pos_Y >= 0) {
          Serial.println(Y);
        } else {
          Serial.println("kein Y vorhanden!");
        }
        if (Pos_Z >= 0) {
          Serial.println(Z);
        } else {
          Serial.println("kein Z vorhanden!");
        }
        if (Pos_F >= 0) {
          Serial.println(F);
        } else {
          Serial.println("kein F vorhanden!");
        }

      } else {
        // Ende der Datei
        myFile.close();
        M5.Lcd.fillRect(10, 140, 320, 20, DARKCYAN);
        M5.Lcd.setCursor(10, 140);
        M5.Lcd.setTextColor(BLACK);
        M5.lcd.print("G-Code File closed!");
        Steuerung_Status = 0;  // Transition zum Zustand 0 (IDLE)
      }
      //////////////////////////////////////////////////////////

      while (X != Xrob) {
        mb.readHreg(RoboterIP, X_HREG, &Xrob);
        mb.writeHreg(RoboterIP, X_HREG, X);
        Serial.println();
        Serial.print("Writing X...");
        mb.task();
        delay(500);
      }
      while (Y != Yrob) {
        mb.readHreg(RoboterIP, Y_HREG, &Yrob);
        mb.writeHreg(RoboterIP, Y_HREG, Y);
        Serial.println();
        Serial.print("Writing Y...");
        mb.task();
        delay(500);
      }
      while (F != Frob) {
        mb.readHreg(RoboterIP, F_HREG, &Frob);
        mb.writeHreg(RoboterIP, F_HREG, F);
        Serial.println();
        Serial.print("Writing F...");
        mb.task();
        delay(500);
      }
      while (Z != Zrob) {
        mb.readHreg(RoboterIP, Z_HREG, &Zrob);
        mb.writeHreg(RoboterIP, Z_HREG, Z);
        Serial.println();
        Serial.print("Writing Z...");
        mb.task();
        delay(500);
      }
      while (Roboter_Status == 1) {
        mb.readHreg(RoboterIP, STATUS_HREG, &Roboter_Status);
        mb.writeHreg(RoboterIP, BEFEHL_HREG, G);
        Serial.println();
        Serial.print("Writing G...");
        mb.task();
        delay(500);
      }



      Serial.println("MOD: Write Data to Modbus");
      Serial.print("MOD: X: ");
      Serial.print(X);
      Serial.print(" Y: ");
      Serial.print(Y);
      Serial.print(" Z: ");
      Serial.print(Z);
      Serial.print(" F: ");
      Serial.println(F);


      M5.Lcd.fillRect(10, 160, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 160);
      M5.Lcd.setTextColor(BLACK);
      M5.lcd.print("Neue Koordinaten X: ");
      M5.lcd.print(X);
      M5.Lcd.fillRect(10, 180, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 180);
      M5.Lcd.setTextColor(BLACK);
      M5.lcd.print("Neue Koordinaten Y: ");
      M5.lcd.print(Y);
      M5.Lcd.fillRect(10, 200, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 200);
      M5.Lcd.setTextColor(BLACK);
      M5.lcd.print("Neue Koordinaten Z: ");
      M5.lcd.print(Z);

      //mb.task();                                        // Starte die Übermittlung

      //mb.task();                                        // Starte die Übermittlung
      //delay(1000);
      oldState = 1;
       Startzeit = millis();
    }
  }
  /////////////////////////////////////////////////
  // Steuerung hat Befehl gesendet
  else if (Steuerung_Status == 2) {
    oldState = 2;

    M5.Lcd.fillRect(10, 100, 240, 20, DARKCYAN);
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.setTextColor(BLACK);

    M5.lcd.print("Steuerung_Status: ");
    M5.lcd.print(Steuerung_Status);


    M5.Lcd.fillRect(10, 120, 320, 20, DARKCYAN);
    M5.Lcd.setCursor(10, 120);
    M5.Lcd.setTextColor(BLACK);

    M5.lcd.print("Roboter_Status  : ");
    M5.lcd.print(Roboter_Status);
    Roboter_Status=2;

    if (Roboter_Status == 2) {
      // Roboter hat Befehl empfangen
      Steuerung_Status = 3;  // Transition zum Zustand 2
      // Zeitpunkt der Transition festhalten
      Startzeit = millis();
      M5.Lcd.fillRect(10, 100, 240, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 100);
      M5.Lcd.setTextColor(BLACK);

      M5.lcd.print("Steuerung_Status: ");
      M5.lcd.print(Steuerung_Status);

      M5.Lcd.fillRect(10, 120, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 120);
      M5.Lcd.setTextColor(BLACK);

      M5.lcd.print("Roboter_Status  : ");
      M5.lcd.print(Roboter_Status);


    } else if (Roboter_Status != 2 and ((Startzeit + SENDETIMEOUT) < millis())) {
      // Roboter hat Befehl nicht empfangen
      Steuerung_Status = 0;  // Transition zum Zustand 0
      Serial.println("Sendetimeout!");
      Roboter_Status = 20;
      myFile.close();
      M5.Lcd.fillRect(10, 140, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 140);
      M5.Lcd.setTextColor(BLACK);
      M5.lcd.print("G-Code File closed!");

      M5.Lcd.fillRect(10, 120, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 120);
      M5.Lcd.setTextColor(RED);

      M5.lcd.print("Roboter_Status  : ");
      M5.lcd.print("s-error");
      Serial.print(" Roboter_Status=");
    }
  }
  /////////////////////////////////////////////////
  // Roboter ist beschäftigt
  else if (Steuerung_Status == 3) {
    oldState = 3;
    if (Roboter_Status == 1) {
      // Roboter ist fertig
      Steuerung_Status = 1;  // Transition zum Zustand 1
      // Zeitpunkt der Transition festhalten
      Startzeit = millis();
    } else if (Roboter_Status != 1 and ((Startzeit + BEFEHLSTIMEOUT) < millis())) {
      // Roboter sollte fertig sein
      Steuerung_Status = 0;  // Transition zum Zustand 0
      Serial.println("Befehlstimeout!");
      Roboter_Status = 30;
      myFile.close();
      M5.Lcd.fillRect(10, 140, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 140);
      M5.Lcd.setTextColor(BLACK);
      M5.lcd.print("G-Code File closed!");

      M5.Lcd.fillRect(10, 120, 320, 20, DARKCYAN);
      M5.Lcd.setCursor(10, 120);
      M5.Lcd.setTextColor(RED);

      M5.lcd.print("Roboter_Status  : ");
      M5.lcd.print("B-error");
    }
  }
  mb.task();  // Starte die Übermittlung
  Serial.println("  MOD:  mb.task(); ");
}