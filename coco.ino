
/* Angelehnt an
 *  
 *  CO2 Ampel Projekt des Verstehbahnhof
 *  https://www.verstehbahnhof.de
 *  kontakt@verstehbahnhof.de
 *  Twitter @verstehbahnhof
 *  
 *  
 *  
 *  Der Sketch verwendet
 *    - die MHZ19 Library von Jonathan Dempsey
 *    
 *  Features v0.2
 *    - Setzen von 3 Schwellwerten: gut, mittel, schlecht
 *    - Auslesen des CO2 Sensors
 *  
 *  
 *  Hardware
 *    - MH-Z19B CO2 Sensor
 *    - Wemos D1 Mini Microcontroller
 *    - LED
 *    
 *  Messwerte, Genauigkeit et al
 *    Der MH-Z19B liefert Messwerte die auf +/- 50ppm genau sind.
 *    Ser Sensor misst Werte gegen angenommene 400ppm CO2 in der Luft.
 *    Beim Start dieses Sketches wird eine Kalibrierung des Sensors
 *    durchgeführt und angenommen das ein 400ppm Zustand herrscht.
 *    Räume sollten somit zum Beginn der Messung gut gelüftet sein.
 *     
 */

#include <Arduino.h>
#include "MHZ19.h"                                        
#include <SoftwareSerial.h>

// CO2 Sensor
#define BAUDRATE 9600                        // Der MH-Z19B verwendet 9600 Baud
#define RX_PIN D3                            //D3_Wemos an TX_Sensor 
#define TX_PIN D4                            //D4_Wemos an RX_Sensor 

// LED Output
int led = D5;                               //D5_Wemos an langes Bein LED

// CO2 Schwellwerte
int co2gut = 500;
int co2mittel = 700;
int co2schlecht = 1100;
//Konstruktoren
MHZ19 myMHZ19;                                             // Constructor for library
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   
void setup()
{
    /* 
     * Start der seriellen Kommunikation
     */
   pinMode(led, OUTPUT);  
    Serial.begin(9600);                                     // Device to serial monitor feedback
    mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start   
    myMHZ19.begin(mySerial);                                // *Serial(Stream) refence must be passed to library begin(). 

    /*
     * Autokalibrierung des Sensors beim Start (abschalten autoCalibration(false))
     */
     myMHZ19.autoCalibration();  //Der niedrigste vom Sensor gesehene Wert, wird als 400ppm angenommen
    
}
// Dieser Programmteil wiederholt sich, bis man den Stecker zieht
void loop()
{

int CO2;
CO2 = myMHZ19.getCO2();    //CO2 in ppm messen
//hier wird die LED eingeschaltet, wenn der Schwellwert erreicht wird
if (CO2 >= co2mittel) {
  digitalWrite(led,HIGH);
}
if (CO2 < co2mittel){
  digitalWrite(led,LOW);
}

        delay(3000);
        Serial.print("CO2 (in ppm): ");                      
        Serial.println(CO2);                                
        int8_t Temp;
        Temp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)
        Serial.print("Temperatur (C): ");                  
        Serial.println(Temp);
    
}
