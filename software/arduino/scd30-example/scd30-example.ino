/*
  Reading CO2, humidity and temperature from the SCD30
  By: Nathan Seidle
  SparkFun Electronics
  Date: May 22nd, 2018
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/14751

  This example prints the current CO2 level, relative humidity, and temperature in C.

  Hardware Connections:
  If needed, attach a Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the device into an available Qwiic port
  Open the serial monitor at 9600 baud to see the output
*/

#include <Wire.h>

//Click here to get the library: http://librarymanager/All#SparkFun_SCD30
#include "SparkFun_SCD30_Arduino_Library.h" 

SCD30 airSensor;

void exportSCD30JSON(SCD30 scd30) {  
  // Ensure that data is available to send
  if (!airSensor.dataAvailable()) {
    return;
  }

  Serial.print("{\"co2\": {");

  // Sensor
  Serial.print("\"sensor\": \"SCD30\", ");

  // CO2 (PPM)
  Serial.print("\"co2ppm\": ");  
  Serial.print(airSensor.getCO2());
  Serial.print(", ");

  // Air temperature
  Serial.print("\"temp\": ");
  Serial.print(airSensor.getTemperature(), 1);
  Serial.print(", ");
  
  // Humidity  
  Serial.print("\"humidity\": ");   
  Serial.print(airSensor.getHumidity(), 1);
  
  // End
  Serial.print("} }");
  Serial.println("");  
}


void setup() {
  Wire.begin();

  Serial.begin(115200);  

  airSensor.begin(); //This will cause readings to occur every two seconds
}

void loop() {

  exportSCD30JSON(airSensor);

  delay(1000);
}
