#include <Arduino.h>
#include "MHZ19.h"     
#include <ESP8266WiFi.h>                                   
#include <SoftwareSerial.h>

// CO2 Sensor
#define BAUDRATE 9600                                      
#define RX_PIN D3       //D3_Wemos an TX_Sensor                                   
#define TX_PIN D4       //D4_Wemos an RX_Sensor                                   

// LED Output
int led = D5;           //D5_Wemos an langes Bein LED

// CO2 Schwellwerte 
int co2gut = 500;
int co2mittel = 700;
int co2schlecht = 1100;

//Konstruktoren
MHZ19 myMHZ19;                                             
SoftwareSerial mySerial(RX_PIN, TX_PIN);      
//WiFi     
const char* ssid     = ".....";  //Für die Pünktchen bitte W-LAN Namen einsetzen
const char* password = "....";   // Für die Pünktchen bitte W-LAN Passwort einsetzen

const char* host = "api.thingspeak.com";  //Die Adresse, an die wir unsere Daten senden.
String apiKey = "........"; // thingspeak.com api key findet man auf der Thinksspeak Seite
WiFiClient client;        

// Alle Systeme hochfahren. Dieser Programmteil wird einmal ausgeführt
void setup()
{
    /* 
     * Start der seriellen Kommunikation
     */
   pinMode(led, OUTPUT);  
    Serial.begin(9600);                                     // Device to serial monitor feedback
    mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start   
    myMHZ19.begin(mySerial);                                // *Serial(Stream) refence must be passed to library begin(). 
    connectToWiFi();
    /*
     * Autokalibrierung des Sensors beim Start (abschalten autoCalibration(false))
     */
     myMHZ19.autoCalibration();  //der niedrigste vom Sensor gesehene Wert, wird als 400ppm angenommen
    
}

// Dieser Programmteil wiederholt sich, bis man den Stecker zieht
void loop()
{
int8_t temp;
temp = myMHZ19.getTemperature();  //Temperatur messen
int CO2;
CO2 = myMHZ19.getCO2();           //CO2 in ppm messen
//WiFi- verbindung wird genutzt, um Daten an die Webadresse zu senden, die wir unter Host gespeichert haben
if (client.connect(host,80)) {
    String postStr = apiKey;   //in das Datenpaket mit dem Namen postStr kommt alles, was man senden möchte
    postStr +="&field1=";
    postStr += String(CO2);
    postStr +="&field2=";
    postStr += String(temp);
    postStr += "\r\n\r\n";
  
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
//hier wird die LED eingeschaltet, wenn der Schwellwert erreicht wird
    if (CO2 >= co2mittel) {
  digitalWrite(led,HIGH);
}
if (CO2 < co2mittel){
  digitalWrite(led,LOW);
}
}
  client.stop(); 
  Serial.println("Uploaded"); 
  delay(20000); // die lange Pause schützt vor Durcheinander beim Senden der Daten
  


 //Ausgabe der Werte auf dem seriellen Monitor
        Serial.print("CO2 (in ppm): ");                      
        Serial.println(CO2);                                      
        Serial.print("Temperatur (C): ");                  
        Serial.println(temp);
    
}

//Funktion, die den WiFi Verbindungsstatus auf dem seriellen Monitor ausgibt
void connectToWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
