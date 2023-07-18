![ref1]![P1#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.002.png)

|<p></p><p></p><p>**RR Deltaroboter**</p>||
| :- | :- |
|**Autoren:**|Reto Bissig<br>Jonathan Ernst<br>Alain Michel<br>Marcel Bernet<br>Lorenz Pfyl|
|**Referent:**|A.H|
|**Datum:**|26.09.2022|


![ref2]	

Inhaltsverzeichnis
######## <a name="_ref496453709"></a>**Inhaltsverzeichnis**
[1	Ausgangslage	1](#_toc115030248)

[1.1	Aufgabenstellung	1](#_toc115030249)

[1.2	Meilensteine	1](#_toc115030250)

[2	Planen und Klären	2](#_toc115030251)

[2.1	Projektplan	2](#_toc115030252)

[2.2	Aufgabenabgrenzung	3](#_toc115030253)

[3	Infosammlung	3](#_toc115030254)

[3.1	Infos von Auftraggeber	3](#_toc115030255)

[3.2	Käufliche Lösungen	3](#_toc115030256)

[4	Anforderungsliste	4](#_toc115030257)

[5	Konzipieren	6](#_toc115030258)

[5.1	Black Box	6](#_toc115030259)

[5.2	Prinzipskizze / Grobkonzept	7](#_toc115030260)

[5.3	Funktionsstruktur	8](#_toc115030261)

[5.4	Beschreibung der Gesamtfunktion	9](#_toc115030262)

[5.4.1	Input	9](#_toc115030263)

[5.4.2	Output	9](#_toc115030264)

[5.4.3	Wirkungen von aussen	9](#_toc115030265)

[5.4.4	Wirkungen nach aussen	9](#_toc115030266)

[5.5	Auflistung der Teilfunktionen	10](#_toc115030267)

[5.6	Morphologischer Kasten	11](#_toc115030268)

[	13](#_toc115030269)

[5.7	Bewertung der Teilfunktionen	14](#_toc115030270)

[5.8	Bewertung der Lösungsvarianten	16](#_toc115030271)

[6	Ausarbeitung	17](#_toc115030272)

[6.1	Mechanische Bearbeitung	17](#_toc115030273)

[6.1.1	Frästeile	17](#_toc115030274)

[6.1.2	Drehteile	18](#_toc115030275)

[6.1.3	Bosch-Profile	18](#_toc115030276)

[6.1.4	Normteile	18](#_toc115030277)

[6.2	Arbeitsbereich	20](#_toc115030278)

[6.3	Zusammenstell Zeichnung	21](#_toc115030279)

[7	Hardware	22](#_toc115030280)

[7.1	Hardwarekomponenten	23](#_toc115030281)

[7.1.1	Elektrokomponenten Antrieb	23](#_toc115030282)

[7.1.2	Elektrokomponenten Steuerung	24](#_toc115030283)

[7.2	Verkabelung	25](#_toc115030284)

[8	Software	26](#_toc115030285)

[8.1	Entwicklungsumgebung	26](#_toc115030286)

[8.1.1	GitHub	26](#_toc115030287)

[8.2	Struktur	27](#_toc115030288)

[8.2.1	State Diagramm	29](#_toc115030289)

[8.2.2	Klassen	31](#_toc115030290)

[8.2.3	Berechnungen	32](#_toc115030291)

[8.2.4	Interpolation	32](#_toc115030292)

[8.2.5	Kinematik	33](#_toc115030293)

[8.3	Programmcode	36](#_toc115030294)

[8.3.1	Klassen	36](#_toc115030295)

[8.3.2	State machine	41](#_toc115030296)

[8.4	Funktion	42](#_toc115030297)

[8.4.1	Client	42](#_toc115030298)

[8.4.2	Server	42](#_toc115030299)

[8.4.3	Clamp	43](#_toc115030300)

[8.4.4	Home	43](#_toc115030301)

[9	Anhang	44](#_toc115030302)

[9.1	Abbildungsverzeichnis	44](#_toc115030303)

[9.2	Tabellenverzeichnis	44](#_toc115030304)

[10	Selbständigkeitserklärung	45](#_toc115030305)


© Jonathan Ernst, Reto Bissig, Alain Michel, Marcel Bernet, Lorenz Pfyl:	September 2022	II

![ref2]Selbständigkeitserklärung
1. # <a name="_toc492839097"></a><a name="_toc492842008"></a><a name="_toc492918454"></a><a name="_toc494457144"></a><a name="_toc494470708"></a><a name="_toc115030248"></a><a name="_toc382579068"></a><a name="_toc484451678"></a><a name="_hlk492839002"></a>**Ausgangslage**
   1. ## <a name="_toc115030249"></a>**Aufgabenstellung**
Der RR-Deltaroboter ist ein vergleichbar mit einem speziellen Plotter. Dieser wird elektronisch mithilfe einer selbsterstellten Software angesteuert. 

1. # **<a name="_toc115030257"></a>Anforderungsliste**


|Revision|Anforderung|F|M|W|Verantwortlich|
| :- | :- | :- | :- | :- | :- |
||Geometrie|||||
||Anhand einer CAD-Simulation das A3-Blatt abdecken|X|||Maschinenbau|
||2-Achsen Positionierung|X|||Maschinenbau|
||Z-Achse mit einem Halter für einen Stift |X|||Maschinenbau|
||Sollte auf einem Teko-Schreibtisch Platz haben||X||Maschinenbau|
||Der Stift sollte wechselbar sein|||X|Maschinenbau/ Elektrotechnik|
||Kräfte|||||
||Das Drehmoment der Motoren aushalten||X||Maschinenbau|
||Energie|||||
||Elektrische Versorgung|X|||Elektrotechnik|
||Material|||||
||Stabilität muss gewährleistet sein||X||Maschinenbau|
||Rostfreie Konstruktion||X||Maschinenbau|
||Die Arme sollten leicht sein||X||Maschinenbau|
||Sicherheit|||||
||Der Roboter darf sich nicht selbst zerstören|X|||Maschinenbau/ Elektrotechnik|
||Quetschgefahr vermeiden||X||Maschinenbau/ Elektrotechnik|
||Genauigkeit|||||
||Linearität:  je linearer, desto besser|||X|Elektrotechnik|
||Wiederholgenauigkeit: je genauer, desto besser|||X|Elektrotechnik|


1. # <a name="_toc115030280"></a>**Hardware**
Die Hauptbestandteile der Hardware können in drei Gruppen aufgeteilt werden. 

1. Antrieb
1. Stromversorgung 
1. Steuerung

In diesem Kapitel werden die verwendeten Komponenten beschrieben sowie weshalb diese gewählt wurden.

<a name="_toc115029001"></a>Abbildung 3: Hardwarekomponenten

![P1533#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.078.png)![P1533TB63#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.079.png)



1. ## <a name="_toc115030281"></a>**Hardwarekomponenten**
Folgende Tabelle zeigt eine Auflistung der Verwendeten Komponenten und ihr Einkaufspreis. Die wichtigsten Elektrokomponenten werden anschliessend kurz beschrieben. 

<a name="_toc115095541"></a>Tabelle 10: Preisliste der Komponenten

|**Bezeichnung**|**Anzahl**|**Preis**|**Preis Total**||
| :- | :- | :- | :- | :- |
|JMC Closed Loop Schrittmotor mit integrierter Endstufe|2\.00|108\.47|216\.94||
|M5Stack Development Kit|1\.00|58\.91|58\.91||
|Micro switch|3\.00|0\.98|2\.94||
|RGB LED Strip 1m|1\.00|6\.84|6\.84||
|Zahnriemen und Zahnräder|1\.00|85\.00|85\.00||
|Diverse Kugellager und Schrauben|1\.00|10\.00|10\.00||
|Greifer|1\.00|5\.00|5\.00||
|Getriebemotor|1\.00|18\.00|18\.00||
|Catch Unit|1\.00|4\.95|4\.95||
|Reed sensor|4\.00|3\.76|15\.04||
|Permanent magnet|4\.00|3\.43|13\.72||
|Rohmaterial pauschal|1\.00|40\.00|40\.00||
|Bosch-Profile und Nutensteine pauschal|1\.00|20\.00|20\.00||
|120W Netzteil Meanwell|3\.00|60\.00|180\.00||
|6A LS|1\.00|10\.00|10\.00||
|Relais Modul|2\.00|12\.00|24\.00||
|PWM Expansionboard|1\.00|21\.00|21\.00||
|MCP23017 I2C Port Expander|1\.00|2\.00|2\.00||
|Kabelkanal|3\.00|5\.00|15\.00||
|Installationsbox|1\.00|65\.00|65\.00||
|Total:|||**814.34**||

||Mechanik Komponente|
| :- | :- |
||Elektrokomponente|

1. ### <a name="_toc115030282"></a>**Elektrokomponenten Antrieb**
### **JMC Closed Loop Schrittmotor mit integrierter Endstufe** 
<a name="_toc115029002"></a>Abbildung 4: DC-Schrittmotoren
![P1681#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.080.png)Als Antrieb für die Delta-Arme haben wir uns für einen Schrittmotor mit einem integrierten Treiber sowie einem geschlossenen Regelkreis entschieden. Der Motor korrigiert automatisch, wenn die gesetzte Schrittposition nicht stimmt. Mit bis zu 50'000 Schritten pro Umdrehung kann eine hohe Winkelgenauigkeit erreicht werden. 

Mit einem «Direction-Bit» kann die Drehrichtung bestimmt werden. Mit jeder Taktflanke am Puls Eingang bewegt sich der Motor um einen Schritt. 

<a name="_toc115095542"></a>Tabelle 11: Winkelgenauigkeit

|**StepsPerRotation:**|**Gear**|**Angle Per Step**|
| :- | :- | :- |
|51200|2\.5|0\.003 °|


1. ### <a name="_toc115030283"></a>**Elektrokomponenten Steuerung**
### **M5Stack Development Kit![P1694#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.081.jpeg)**
<a name="_toc115029003"></a>Abbildung 5: M5Stack Core 2

Als Controller für die Robotersteuerung wurde ein M5Stack Core 2 Mikrocontroller verwendet. Der RISC-5 Controller unterstützt mit Wifi und I2C wichtige Schnittstellen für die Kommunikation zwischen Client und Roboter bzw. MCP23017. Der M5Stack Core verfügt mit einem 320x220 pixel LCD-Touchscreen über die nötigen Bedienungs- und Anzeigemöglichkeiten. Da der M5Stack nicht über einfach zugängliche GPIO-Ports verfügt wurde mit der I2C Schnittstelle ein GPIO und ein PWM Port Expander verwendet. 

Um die Geschwindigkeit und die Ansteuerung des Roboters zu vereinfachen, wäre wohl eine direkte Ansteuerung des Roboters mit einem ESP32 Dev 3 Controllers auch möglich gewesen, allerdings mit dem Verlust des LCD-Screens.
### **MCP23017 I2C Port Expander**
Zur GPIO Port Erweiterung wurde mit einem MCP23017 gearbeitet. Dieser Chip stellte sich als verlässlicher Partner bei der Ansteuerung der DC-Motoren heraus. Leider verfügt der MCP23017 über keine Hardware PWM- oder DAC-Schnittstellen. 
### **PWM Expansionboard**
Um dennoch einen Servomotor anschliessen zu können, wurde eine PWM-Erweiterungsplatine mit einem PCA9685 16 Kanal 12-Bit Controller verwendet.

<a name="_toc115029004"></a>Abbildung 6: PCA9685 PWM Module

<a name="_toc115029005"></a>Abbildung 7: MCP23017

![P1702TB67#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.082.png)![P1702TB66#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.083.png)![P1702#y2](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.084.png)![P1702#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.085.png) 
1. ## <a name="_toc115030284"></a>**Verkabelung**
Für die Verkabelung des Roboters wurde kein Schema erstellt. Mit einer Definition der Ein- und Ausgänge sowie der Bestimmung der Betriebsspannung von 36VDC, konnte die Steuerung, der Motor und die Stromversorgung miteinander verbunden werden. 

<a name="_toc115095543"></a>Tabelle 12: GPIO Port Zuweisung

|**GPIO Port** |**HW Pin**|**IO Type**|**Interface**|**Info**|
| :- | :- | :- | :- | :- |
|GPB0|1|Output|i2C - MCP23017-E|Pull+ Blue Motor Left|
|GPB1|2|Output|i2C - MCP23017-E|Dir+ White Motor Left|
|GPB2|3|Output|i2C - MCP23017-E|En + BrownBlack Motor Left&Right|
|GPB3|4|Input|i2C - MCP23017-E|Alarm+ Whiteblack Motor Left|
|GPB4|5|Input|i2C - MCP23017-E|Ped+ GreenBlack Motor Left|
|GPB5|6|Output|i2C - MCP23017-E|Datapin RGBLED|
|GPB6|7|Output|i2C - MCP23017-E|Signal LED / Optional|
|PWM1|1|Output|i2C – PCA9685|Clamp Orange Servo|
|GPA0|21|Output|i2C - MCP23017-E|Pull+ Blue Motor Left|
|GPA1|22|Output|i2C - MCP23017-E|Dir+ White Motor Left|
|GPA2|23|Output|i2C - MCP23017-E|En + BrownBlack Motor Left&Right|
|GPA3|24|Input|i2C - MCP23017-E|Alarm+ Whiteblack Motor Left|
|GPA4|25|Input|i2C - MCP23017-E|Ped+ GreenBlack Motor Left|
|GPA5|26|Output|i2C - MCP23017-E|Reed / Push Sensor|
|GPA6|27|Output|i2C - MCP23017-E|Vertical Down|
|GPA7|28|Output|i2C - MCP23017-E|Vertical Up|

Die restliche Verkabelungsarbeit beinhaltet lediglich die Verbindung dieser Komponenten gemäss obenstehender Tabelle sowie die Versorgung dieser Komponenten mit der richtigen Spannung. Gelegentlich musste noch ein Widerstand als Pull-Up oder zur Strombegrenzung eingebaut werden. 

![P1809#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.086.png)

<a name="_toc115029006"></a>Abbildung 8: Verkabelung
1. # <a name="_toc115030285"></a>**Software**
Als wichtiger Teil dieses Projektes bestimmt die Software die Funktionsweise des Roboters. 
1. ## <a name="_toc115030286"></a>**Entwicklungsumgebung**
Die Mikrocontroller (M5Stack Core2 und M5Stack) können mit der Entwicklungsumgebung von Arduino programmiert werden. 

![P1815#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.087.png)

<a name="_toc115029007"></a>Abbildung 9: VS Code mit der Platformio Erweiterung

Bei der Suche nach einer alternative, bietet die Entwicklungsumgebung VS-Code mit der Erweiterung Platformio eine professionelle Lösung.

<a name="_toc115095544"></a>Tabelle 13: Verwendete Programme

|**#**|**Name**|**Typ**|**Link**|**Info**|
| :- | :- | :- | :- | :- |
|1|VS-Code|Entwicklungsumgebung|[**Ê**](https://code.visualstudio.com/)|Entwicklungsumgebung zum Schreiben von Software.|
|2|Platformio|Add-On|[**Ê**](https://platformio.org/)|Erweiterung für VS-Code für die Arduino Umgebung.|
|3|GitHub|Software-Versionierung |[**Ê**](https://github.com/ElTschonni)|Online Software Repository|
|4|Git|Software-Versionierung |[**Ê**](https://git-scm.com/)|Zur Synchronisation von GitHub und VS-Code|
|5|Arduino IDE|Entwicklungsumgebung|[**Ê**](https://www.arduino.cc/en/software)|Entwicklungsumgebung für Mikrokontroller mit Arduino bootloader.|
|6|ModbusWindowsClient|Modbus Client||Zum Testen von Modbus|
|7|Notepad++|Texteditor|[**Ê**](https://notepad-plus-plus.org/downloads/)|Anzeigen von Programcode|
|9|Excel|Tabellenkalkulation|[**Ê**](https://www.microsoft.com/en-us/microsoft-365/excel)|Erstellen von Rechentabellen|
|10|GeoGebra|Rechner|[**Ê**](https://www.geogebra.org/)|Berechnung von Geometrien|
1. ### <a name="_toc115030287"></a>**GitHub**
Der Programcode ist online auf GitHub unter folgendem Link einsehbar. 

<table><tr><th colspan="1" rowspan="2" valign="top"><p>
<a name="_toc115029008"></a>Abbildung 10: GitHub Profil</p><p>![P1881C1T21#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.088.png)</p></th><th colspan="1" valign="top"><https://github.com/ElTschonni/DROBOT-alpha></th></tr>
<tr><td colspan="1" valign="top">[Source Code](https://github.com/ElTschonni/DROBOT-alpha/blob/main/DROBOT-code/src/DR_v02_00.cpp)   -> [DROBOT-Library](https://github.com/ElTschonni/DROBOT-alpha/blob/main/DROBOT-code/include/DROBOT.h)</td></tr>
</table>

1. ## <a name="_toc115030288"></a>**Struktur**
Die Programmstruktur wird aufgeteilt in zwei grobe Strukturformen. 

In der Hauprogrammdatei wird der Ablauf geregelt. Mit verschiedenen Klassen, welche sich in einer Bibliothek befinden werden die Probleme gelöst.

Der Ablauf des Hauptprogrammes orientiert sich an dem im Kapitel 8.2.1 definierten State Diagramm.

Es gibt insgesamt 3 Klassen, welche für die Steuerung des Roboters und die Berechnung der Bewegungen erstellt wurden. Diese werden im Kapitel  0 
### **State 3 Draw**
Beschreibung des States 3 Draw anhand verschiedener Diagramme:
### ![P1906#yIS1]
Abbildung 12: State 3 Draw Flowchart
### ![P1908#yIS1]
Abbildung 13: State 3 Draw UML
### **
Klassen beschrieben.
1. ### <a name="_ref115004502"></a><a name="_toc115030289"></a>**State Diagramm**
![P1894#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.091.jpeg)

<a name="_toc115029009"></a>Abbildung 11: State Diagramm DRobot Code

1. **Idle**: 	In diesem State wird sich der Roboter befinden, wenn er auf einen neuen Befehl vom Modbus Client wartet.
1. **Receive**:	In diesem State wird sich der Roboter befinden, wenn er einen neuen Befehl vom Modbus Client erhalten hat. Nun werden nie Neuen Daten eingelesen.
1. **Draw**:	In diesem State wird sich der Roboter befinden, wenn er einen Befehl ausführen soll. Er ist so lange in diesem State, bis er die Zielkoordinaten erreicht hat.
1. **Wrong Command:**	Wenn der Befehl, welcher dem Roboter gesendet wurde, nicht programmiert ist, wechselt er in diesen State.
1. **ERROR:**	Wenn eine Berechnung zur Ansteuerung von Koordinaten einen Fehler zurückgibt, wechselt der Roboter in den Fehlermodus.
1. **Initialize:**	Immer, wenn der Controller eingeschaltet wird, durchläuft der Roboter diesen State. Hier werden die Klassen initialisiert.
1. **Calibrate:**	Um den 0-Koordinatenpunkt zu finden, wird eine Programablauf zur Kalibrierung der Roboterarme durchgeführt.

<a name="_ref115004861"></a>
### **State 3 Draw**
Beschreibung des States 3 Draw anhand verschiedener Diagramme:
### ![P1906#yIS1]
Abbildung 12: State 3 Draw Flowchart
### ![P1908#yIS1]
Abbildung 13: State 3 Draw UML
### **
1. ### <a name="_toc115030290"></a>**Klassen**
Abbildung 14: Diagramm der Klassen
![P1912#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.092.png)Die drei Klassen lassen sich anhand folgender Grafik gut beschreiben.





1. ### <a name="_toc115030291"></a>**Berechnungen**
Dem Roboter werden jeweils Koordinatenpunkte geschickt. Vom aktuellen Standort soll nun der neue Punkt P angefahren werden. 

Nun bedarf es zwei grundlegenden Berechnungen, um auf den Weg zu diesen Koordinaten auf einer gerade Linie abfahren zu können. In einem ersten Schritt wird der Weg in kleine Abschnitte aufgeteilt und es werden für jeden Abschnitt zwischenpunkte berechnet. Zudem muss der Einstellwinkel für den Schrittmotor berechnet werden.
1. ### <a name="_toc115030292"></a>**Interpolation**
Mit der Interpolation zwischen zwei Koordinatenpunkten können wir viele kleine Abschnitte auf dieser Linie berechnen. 

<a name="_toc115095545"></a>Tabelle 14: Parameter für die Berechnung der Interpolation

|**r**|10|mm|**Number of Stepps: (NoS)**|10000||
| :- | -: | :- | :- | -: | :- |
|**tmax**|16|s|**angle per step**|0\.036|0\.00062832|
|**x1**|10|mm|**min res:** |0\.4021|mm|
|**y1**|1|mm|**tperstep**|0\.007229029|s|
|**x2**|900|mm|**l**|890\.0202245|mm|
|**y2**|7|mm|**Länge Arm**|640|mm|
|**yw**|0|mm|**NofSteps**|2213\.298758||

![](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.093.png)

<a name="_toc115029010"></a>Abbildung 15: Grafische Darstellung Interpolation

Umso grösser der Abstand zwischen den zwei Koordinatenpunkten ist, umso mehr schritte werden berechnet. 

Xz=1-StepNoS×X1+StepNoS×X2
Yz=1-StepNoS×Y1+StepNoS×Y2

1. ### <a name="_toc115030293"></a>**Kinematik**
Zur Berechnung des Einstellwinkels des Schrittmotores wird mit der Inversen Kinematik, vom Koordinatenschnittpunkt aus dem Winkel des Armes berechnet. Dies kann mit der Vektorrechnung oder mit dem Schnittpunkt zweier Kreise berechnet werden.
### **Schnittpunkt zweier Kreise**

![P1980#yIS1]

<a name="_toc115029011"></a>Abbildung 16: Geometrische Zeichnung der Roboter Arme

Schnittpunkt zweier Kreise:*  K1-K2

K1=x1-xm12+y1-ym12
K2=x2-xm22+y2-ym22
Formel der Schnittpunkts Linie zwischen S1 und S2:

fx=mx+b→ m= S1x-S2xS2y-S1y→b=r12-r22+ym22-ym12+xm22-xm122\*(ym1-ym2)

Schnittpunktkoordinaten mit der p-q Formel berechnen:

p=2(-xm1+m(b-ym1)1+m2
q=xm12+b-ym1×b-ym1-r121+m2

Determinante p22-q>0

X1,2=-p2±p24-q
Y1,Y2=m×X1,2+b

<a name="_toc115029012"></a>Abbildung 17: Vektorzeichnung der Roboterarme

ut[19]=
### ![P1989TB70#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.095.png)![P1989#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.096.png)**Vektorberechnung**
Die Arme des Deltaroboters können auch als Vektoren aufgezeichnet und berechnet werden. 
Ein Versuch mit Mathlab ergab folgende Lösung: 
![P1990#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.097.png)
Leider war der Versuch diese Gleichung nach dem gesuchten Winkel V1 aufzulösen, erfolglos. 


![P1993#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.098.png)

<a name="_toc115029013"></a>Abbildung 18: Berechnungsversuch mit Mathlab
### **Tooloffset**
Da der Haltegriff für den Schreiber nicht auf dem Schnittpunkt der beiden Arme liegt, sondern etwas versetzt, musste diese Abweichung berechnet werden. 

![P1997#yIS1]

<a name="_toc115029014"></a>Abbildung 19: Geometrische Zeichnung Roboterarm

Zur Berechnung des Winkels α, wurde bei der Berechnung des Schnittpunktes zweier Kreise, der Radius vr verwendet und nicht wie bisher der Radis vR2. Bevor nun der Winkel des linken Armes berechnet werden kann, wird der Vektor w berechnet und die Koordinaten PA. 

1. Berechne den Vektor vr:	vr=P0-SR1
1. Winkel vi3 berechnen:	vi3=sin-1vryvR2
1. Länge von vr berechnen:	P0SR1=vry2+vrx2
1. Winkel vi4  berechnen:	vi4=cos-1w2+vr2-vR222×vr×w 
1. Winkel vi5 berechnen:	vi5=180-vi0-vi4
1. Winkel δvi berechnen:	δvi=360-vi3-vi4
1. Neue X Koordinaten:		Xz=w×cosδvi+P0x	
1. Neue Y Koordinaten:		Yz=w×sin(δvi)+P0y

Nun kann mit der üblichen Berechnungsmethode vom Koordinatenpunkt PA aus, der Winkel des zweiten Armes berechnet werden.


1. ## <a name="_toc115030294"></a>**Programmcode**
Der Programmcode für den Deltaroboter wurde in der Programmiersprache C++ geschrieben. In diesem Kapitel werden anhand von einzelnen Ausschnitten die wichtigsten Bereiche des Programmes erklärt. 

Der Programcode ist online auf GitHub unter folgendem Link einsehbar. 

<table><tr><th colspan="1" rowspan="2" valign="top"><p>
<a name="_toc115029015"></a>Abbildung 20: GitHub Profil</p><p>![P2014C1T23#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.100.png)</p></th><th colspan="1" valign="top"><https://github.com/ElTschonni/DROBOT-alpha></th></tr>
<tr><td colspan="1" valign="top">[Source Code](https://github.com/ElTschonni/DROBOT-alpha/blob/main/DROBOT-code/src/DR_v02_00.cpp)   -> [DROBOT-Library](https://github.com/ElTschonni/DROBOT-alpha/blob/main/DROBOT-code/include/DROBOT.h)</td></tr>
</table>


1. ### <a name="_toc115030295"></a>**Klassen**
Es wurden insgesamt 3 Klassen zur Steuerung des Roboters erstellt. 
### **Closed\_Loop\_Step\_Motor**
//=========| 1: CLSM Class: Closed\_Loop\_Step\_Motor  |=============

class Closed\_Loop\_Step\_Motor  //CLSM

Die Klasse Closed Loop Step Motor wird dafür benötigt, um die Schrittmotoren mit der Angabe eines Winkels zu steuern. So kann dem Motor ein Befehl einen Schritt zu machen erteilt werden. Der Motor rechnet dann den Schrittwinkel zur aktuellen Winkelposition des Motors hinzu. In einer entsprechenden Schlaufe im Hauptprogram werden dann, so viele Schritte gefahren, bis der Zielwinkel erreicht wird. 

|**Closed\_Loop\_Step\_Motor Class Diagram**||
| :- | :- |
|Class Attributes:||
|<p>private:</p><p>`  `long us\_timer\_state;</p><p>`  `unsigned long us\_timer\_value;</p><p>`  `unsigned long us\_timer\_start;</p><p>`  `//Motor Pin Configuration</p><p>`  `unsigned int Motor\_Enable\_Pin;  //Hardware configuration of the Enable pin</p><p>`  `unsigned int Motor\_Pull\_Pin;    //Pull Pin Puls to move by one step</p><p>`  `unsigned int Motor\_Dir\_Pin;     //Direction pin - set to high to move right, low for left</p><p>`  `unsigned int Motor\_Alarm\_Pin;   //Alarm Pin</p><p>`  `unsigned int Motor\_Ped\_Pin;     // Confirmation bit for reaching the new stepp</p><p>`  `//  Adafruit\_MCP23X17 GPIO\_I2C\_Exp;</p><p>`  `Adafruit\_MCP23X17 \*GPIO\_I2C\_Exp;</p><p>`  `//Motor Pin Values</p><p>`  `bool Motor\_Enable\_Value = false;</p><p>`  `bool Motor\_Pull\_Value = false;</p><p>`  `bool Motor\_Dir\_Value = Motor\_Turn\_right;</p><p>`  `bool Motor\_Alarm\_Value = false;</p><p>`  `bool Motor\_Ped\_Value = false;</p><p>`  `bool Motor\_Position\_Value = Motor\_Position\_right;  //1 = right. 0 = left</p><p>`  `unsigned int Dip\_Switch\_Value = 0;</p><p>`  `double Gear\_Belt\_Value = 0;</p><p>`    `//according to the Datasheet iHSS57</p><p>`  `double Dip\_Switch\_Array[17][3] </p><p>`  `double Old\_Angle\_Value = 0;</p><p>`  `double New\_Angle\_Value = 0;</p>||
|<p>public:</p><p>`  `//Attributes</p><p>`  `double Motor\_Angle\_Value = 0;</p><p>`  `double Angle\_Per\_step = 0;  //  360 / StepsPerRotation</p><p>`  `double Steps\_Per\_rev = 0;</p><p>`  `double RPM\_SetPoint\_Value = 0;         // setpoint value</p><p>`  `double RPM\_ProcessVariable\_Value = 0;  // calculating the actual value</p><p>`  `double RPM\_Error\_Value = 0;            // calculate the difference between setpoint and actual value</p><p>`  `double Time\_PerRotation\_Value = 0;     // RPM\_SetPoint\_Value - RPM\_ProcessVariable\_Value</p><p>`  `double Time\_PerStep\_Value = 0;         // (60 / RPM\_ProcessVariable\_Value) / StepsPerRotation:</p><p>`  `double Frequency\_Pulse\_Value = 0;      // 1/Time\_PerStep\_Value</p><p>`  `double Time\_PulseHigh\_Value = 0;       // Time\_PerStep\_Value \* 0.5</p><p>`  `double Time\_PulseLow\_Value = 0;        // Time\_PerStep\_Value \* 0.5</p><p>`  `double Frequency\_I2CBus\_Value = 0;     // Frequency\_Pulse\_Value \* 2</p><p></p><p>`  `bool Angle\_Tollerance\_Value = 0;  //1 = Angle in Tollerance; 0 = Angle outside Tollerance</p><p></p><p>`  `unsigned int errorcode = 0;</p><p>`  `double Max\_AngDev\_Value = 0;</p><p>`  `double Min\_AngDev\_Value = 0;</p>||
|**Constructor:** Closed\_Loop\_Step\_Motor(Adafruit\_MCP23X17 \*GPIO\_instance) ||
|**Class Methodes:**||
|void setupMotor(bool MotorPos, unsigned int EnPin, unsigned int PullPin, unsigned int DirPin, unsigned int AlmPin, unsigned int PedPin, unsigned int RPM, unsigned int DipSwitch, double GearBelt) {  //Motor Pin Configuration||
|void test() ||
|void enableMotor()||
|void disableMotor()||
|bool startSurvilance()||
|bool setOldAngelValue(double Angle)||
|double getOldAngelValue()||
|bool setNewAngelValue(double Angle)||
|double getNewAngelValue()||
|bool moveOneStep(bool direction)||
|bool OneStepDir(/\*bool direction\*/)||

**

**Motor\_Movement\_Calc**

//=========| 2: MMC Class: Motor\_Movement\_Calc  |=============

class Motor\_Movement\_Calc {

Mit der Motor Movement Calc Klasse wird die Interpolation zur Bahnberechnung gemacht. Mit  4 Methoden: 

- setOriginCoordinates 
- setTargetCoordinates 
- setMaxTime 
- setDoneSteps

können gewisse Attribute gesetzt werden. Mit der Methode Interpolate() kann anschliessend eine Interpolation zwischen diesen Punkten durchgeführt werden. 

|**Motor\_Movement\_Calc Class Diagram**||
| :- | :- |
|Class Attributes:||
|<p>private:</p><p>`  `double Steps\_Per\_Rev = 0;</p><p>`  `double Angle\_Per\_Step = 0;</p><p>`  `double Angle\_Per\_StepB = 0;</p>||
|<p>public:</p><p>`  `unsigned int TotalNof\_Steps\_Value = 0;</p><p>`  `unsigned int Done\_Steps\_Value = 0;</p><p>`  `unsigned int Missing\_Steps\_Value = 0;</p><p>`  `double Min\_Resolution\_Value = 0;</p><p>`  `double Time\_PerStep\_Value = 0;</p><p>`  `double Max\_Work\_Time = 0;</p><p>`  `unsigned long X1\_Origin\_Value = 0;</p><p>`  `unsigned long Y1\_Origin\_Value = 0;</p><p>`  `unsigned long X2\_Target\_Value = 0;</p><p>`  `unsigned long Y2\_Target\_Value = 0</p><p>`  `unsigned long Xz\_NextStep\_Value = 0;</p><p>`  `unsigned long Yz\_NextStep\_Value = 0;</p><p>`  `double Length\_ofWay\_Value = 0;      // The distance between Point A and B</p><p>`  `double Length\_BaseToTip\_Value = 0;  // The distance from the axis of the arm to the tip of the pen</p>||
|**Class Methodes:**||
|void setupInterpolate(double StPerRot, double AngPerStp, double LArm)   //Set all nessesary Variables||
|void setOriginCoordinates(unsigned long X1, unsigned long Y1)||
|void setTargetCoordinates(unsigned long X2, unsigned long Y2)||
|void setMaxTime(double Tmax)||
|void setDoneSteps(unsigned long Dsteps) ||
|void Interpolate()||





### **Delta2D\_Kinematic**
//=========| 3: D2D Class: Delta2D\_Kinematic   |=============

class Delta2D\_Kinematic

Die Delta 2D Kinematik Klasse ist dazu da um die Kinematik des Deltaroboters zu berrechnen. Insbesondere sind hier die neuen Winkel interessant, welche eingestellt werden müssen um einen Koordinatenpunkt zu erreichen.

Mit der Methode RecalcCoordinates kann die Abweichung vom Stift zum Schnittpunkt der Arme berechnet werden.

|**Delta2D\_Kinematic Class Diagram**|
| :- |
|Class Attributes:|
|<p>private:</p><p>`  `double Line\_S1S2\_b = 0;</p><p>`  `double Line\_S1S2\_m = 0;</p><p>`  `unsigned long Lenght\_UpArm\_Value = 0</p><p>`  `unsigned long Lenght\_LowArm\_Value = 0</p><p>`  `unsigned long rK1\_LowArm\_Value = 0;  </p><p>`  `double l\_PAP0\_Value = 0; </p><p>`  `double vi0\_PAP0\_angle = 0</p><p>`  `double vi0\_ToolTip\_angle = 0;</p><p>`  `unsigned long DeltaX\_LeftArm\_Value = 0</p><p>`  `unsigned long DeltaX\_RightArm\_Value = 0</p><p>`  `double Transmission\_LeftArm\_Value = 0;</p><p>`  `double Transmission\_RightArm\_Value = 0;</p><p>`  `bool TolOffSet\_true = 0;</p><p></p>|
|<p>public:</p><p>`  `double Circle1\_Middle\_X = 0;  // Middle Point coordianetes X of Circle 1</p><p>`  `double Circle1\_Middle\_Y = 0;  // Middle Point coordianetes Y of Circle 1</p><p>`  `double Circle2\_Middle\_X = 0;  // Middle Point coordianetes X of Circle 2</p><p>`  `double Circle2\_Middle\_Y = 0;  // Middle Point coordianetes Y of Circle 2</p><p>`  `double Intersection\_I1\_X = 0;  //Intersection S1</p><p>`  `double Intersection\_I1\_Y = 0;  //Intersection S1</p><p>`  `double Intersection\_I2\_X = 0;  //Intersection S2</p><p>`  `double Intersection\_I2\_Y = 0;  //Intersection S2</p><p>`  `double angle\_LeftArm\_Value1 = 0;</p><p>`  `double angle\_LeftArm\_Value2 = 0;</p><p>`  `double angle\_RightArm\_Value1 = 0;</p><p>`  `double angle\_RightArm\_Value2 = 0;</p><p>`  `double Xz\_newRightArm\_Value = 0;</p><p>`  `double Yz\_newRightArm\_Value = 0;</p>|



|**Class Methodes:**||
| :- | :- |
|<p>void setupDelta2D\_Kinematic(</p><p>`    `unsigned long LUArm,</p><p>`    `unsigned long LLArm,</p><p>`    `bool SetTool,</p><p>`    `unsigned long MPTipP,</p><p>`    `double MPTipPAngle,</p><p>`    `unsigned long DXLArm,</p><p>`    `unsigned long DXRArm,</p><p>`    `double TransLArm,</p><p>`    `double TransRArm)</p>||
|<p>`  `unsigned long IntersectionOfTwoCircles(unsigned long C1MX, unsigned long C1MY, unsigned long C2MX, unsigned long C2MY) {  </p><p>//Calculate the intersection of two circles</p>||
|void InverseKinematic(bool WhichArm)  //Left Arm = 0, Right Arm = 1||
|<p>void RecalcCoordinates(unsigned long P0X,unsigned long P0Y) </p><p>//Calculate Coordinates after ToolOffset</p>||


1. ### <a name="_toc115030296"></a>**State machine**
Es gibt 7 Zustände welche in der Endlosschleife im Hauptprogramm aufgerufen werden können. 

`    `switch (state) {

`      `case 1: state = Standby(); break;  //Standby

`      `case 2: state = Receive(); break;  //receive new Coordinates

`      `case 3: state = Draw(); break;  //Draw according to the new command

`      `case 4: state = WrongCommand(); break;  //Received a Wrong Command

`      `case 5: state = Error(); break;  

`      `case 6: state = Init(); break;  

`      `case 7: state = Calibrate(); break;  

`    `}

`    `programSelector = state;

<a name="_toc115095546"></a>Tabelle 15: Anzeigebeispiel

|![P2241C1T28#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.101.jpeg)|![P2242C2T28#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.102.jpeg)|![P2243C3T28#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.103.jpeg)|![P2244C4T28#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.104.jpeg)|
| :-: | :-: | :-: | :-: |
|state = Init();|state=Calibrate();|state=Standby();|state=Draw();|


1. ## <a name="_toc115030297"></a>**Funktion**
Über die Touchscreens der M5Stack können gewisse Befehle an den Roboter gesendet werden.
1. ### <a name="_toc115030298"></a>**Client**
![P2255#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.105.jpeg)

<a name="_toc115029016"></a>Abbildung 21: Anzeige Client

Der Client zeigt den aktuellen Status der Roboter Steuerung sowie des Roboters an. Von hier können neue Koordinaten, welche in einer Datei auf der SD Karte gespeichert sind an den Roboter geschickt werden. 

||Mit dem Betätigen des linken Tasters kann die Datei geöffnet und die Koordinaten an den Roboter geschickt werden. |
| :- | :- |
1. ### <a name="_toc115030299"></a>**Server**
![P2262#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.106.jpeg)

<a name="_toc115029017"></a>Abbildung 22: Anzeige Server

Für den Server wurde ein M5Core2 verwendet. Auf dem Display werden jeweils Informationen zum Aktuellen Status des Roboters angezeigt. Mit 3 Tastern können auch die Stifthalterung oder das Homing gesteuert werden. 
1. ### <a name="_toc115030300"></a>**Clamp**

|<p>![P2266C1T30#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.107.jpeg)</p><p><a name="_toc115029018"></a>Abbildung 23: Standby Anzeige</p>|<p>- Mit dem Betätigen des linken Tasters kann die Stifthalterung geschlossen werden.</p><p>- Mit dem Betätigen des mittleren Tasters kann die Stifthalterung geöffnet werden. </p><p></p>|
| :- | :- |

1. ### <a name="_toc115030301"></a>**Home**

|<p>![P2274C1T31#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.108.jpeg)</p><p><a name="_toc115029019"></a>Abbildung 24: Calibrate Anzeige</p>|<p>- Mit dem Betätigen des rechten Tasters kann eine Kalibrierung des Stifthalters durchgeführt werden.</p><p>Dann wechselt der Roboter in den State Calibrate.</p>|
| :- | :- |



1. # <a name="_toc115030302"></a>**Anhang**
   1. ## <a name="_toc115030303"></a>**Abbildungsverzeichnis**
[Abbildung 1: Black Box	6](#_toc115028999)

[Abbildung 2: Grobkonzept Skizze	7](#_toc115029000)

[Abbildung 3: Hardwarekomponenten	22](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115029001)

[Abbildung 4: DC-Schrittmotoren	23](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115029002)

[Abbildung 5: M5Stack Core 2	24](#_toc115029003)

[Abbildung 6: PCA9685 PWM Module	24](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115029004)

[Abbildung 7: MCP23017	24](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115029005)

[Abbildung 8: Verkabelung	25](#_toc115029006)

[Abbildung 9: VS Code mit der Platformio Erweiterung	26](#_toc115029007)

[Abbildung 10: GitHub Profil	26](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115029008)

[Abbildung 11: State Diagramm DRobot Code	29](#_toc115029009)

[Abbildung 12: Grafische Darstellung Interpolation	32](#_toc115029010)

[Abbildung 13: Geometrische Zeichnung der Roboter Arme	33](#_toc115029011)

[Abbildung 14: Vektorzeichnung der Roboterarme	34](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115029012)

[Abbildung 15: Berechnungsversuch mit Mathlab	34](#_toc115029013)

[Abbildung 16: Geometrische Zeichnung Roboterarm	35](#_toc115029014)

[Abbildung 17: GitHub Profil	36](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115029015)

[Abbildung 18: Anzeige Client	42](#_toc115029016)

[Abbildung 19: Anzeige Server	42](#_toc115029017)

[Abbildung 20: Standby Anzeige	43](#_toc115029018)

[Abbildung 21: Calibrate Anzeige	43](#_toc115029019)
1. ## <a name="_toc115030304"></a>**Tabellenverzeichnis**
[Tabelle 1: Projektplan	2](https://tekoedu.sharepoint.com/sites/a_Gruppenarbeit_Robotik/Freigegebene%20Dokumente/General/01_Dokumentation/01_Dokumentation/Projektdokumentation%20Roboter.docx#_Toc115095532)

[Tabelle 2: Anforderungsliste	5](#_toc115095533)

[Tabelle 3: Funktionsstruktur	8](#_toc115095534)

[Tabelle 4: Auflistung der Teilfunktionen	10](#_toc115095535)

[Tabelle 5: Morphologischer Kasten	12](#_toc115095536)

[Tabelle 6: Bewertung der Teilfunktionen 1-2	14](#_toc115095537)

[Tabelle 7: Bewertung der Teilfunktionen 3-4	15](#_toc115095538)

[Tabelle 8: Bewertung der Teilfunktionen 5-6	16](#_toc115095539)

[Tabelle 9: Bewertung der Lösungsvarianten	16](#_toc115095540)

[Tabelle 10: Preisliste der Komponenten	23](#_toc115095541)

[Tabelle 11: Winkelgenauigkeit	23](#_toc115095542)

[Tabelle 12: GPIO Port Zuweisung	25](#_toc115095543)

[Tabelle 13: Verwendete Programme	26](#_toc115095544)

[Tabelle 14: Parameter für die Berechnung der Interpolation	32](#_toc115095545)

[Tabelle 15: Anzeigebeispiel	41](#_toc115095546)




1. # <a name="_toc494457266"></a><a name="_toc115030305"></a>**Selbständigkeitserklärung**
Ich erkläre hiermit, dass ich die vorliegende Arbeit selbstständig verfasst und keine anderen als die angegebenen Quellen benutzt habe. Alle Stellen, die wörtlich oder sinngemäss aus Quellen entnommen wurden, habe ich als solche kenntlich gemacht. Mir ist bewusst, dass Verstösse gegen die Grundsätze der Selbstständigkeit als Täuschung betrachtet und entsprechend der Prüfungsordnung geahndet werden.

Diese Arbeit darf nur mit Genehmigung des Verfassers an Dritte weitergegeben werden.

![P2329#yIS1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.109.png)![](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.110.png)![P2328#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.111.jpeg)Joanthan Ernst    Reto Bissig	Alain Michel		Marcel Bernet		Lorenz Pfyl		

![P2329#y2](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.112.png)![P2329#y1](Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.113.png)



Schwyz, 22.09.20![ref1]22

[ref1]: Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.001.png
[ref2]: Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.004.png
[P1516#y1]: Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.075.jpeg
[P1906#yIS1]: Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.089.jpeg
[P1908#yIS1]: Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.090.jpeg
[P1980#yIS1]: Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.094.png
[P1997#yIS1]: Aspose.Words.70a3802e-d390-4a62-9d0c-541cbcb047f0.099.png
