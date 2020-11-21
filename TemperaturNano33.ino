/*
  HTS221 - Read Sensors
  This example reads data from the on-board HTS221 sensor of the
  Nano 33 BLE Sense and prints the temperature and humidity sensor
  values to the Serial Monitor once a second.
  The circuit:
  - Arduino Nano 33 BLE Sense
  This example code is in the public domain.
*/

#include <Arduino_HTS221.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);  //die Zeile auskommentieren, wenn kein Mitlesen am Seriellen Monitor gewünscht ist

Serial1.begin(115200);

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
}

void loop() {
  
  float temperature = HTS.readTemperature();
  Serial1.print(temperature); //am port txrx
  Serial.print(temperature);  //am port txrx
  delay(6000);
}
