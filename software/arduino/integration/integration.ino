#include <Wire.h>


#include <sps30.h>
#include "SPS30Interface.h"
#include "SparkFun_SCD30_Arduino_Library.h" 
#include "SensorBuffer.h"
#include "SensorRadiation.h"

SCD30 airSensor;          // SCD30 CO2 Sensor
SPS30Interface sps30;     // SPS30 Air Particle Sensor

// Radiation Watch Type 5 radiation sensor
SensorBuffer sbRad(100);
SensorRadiation sensorRadiation(&sbRad);


/*
 * Sensirion SCD30 CO2 sensor
 */
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

/*
 * Radiation sensor with digipot backpack
 */
void initializeRadiationSensor() {
  // Initialize the digipot
  sensorRadiation.initDigipot();
  delay(10);
  //setDigipot(10);
  sensorRadiation.setDigipotAbsolute(55);
  delay(10);

  // Initialize radiation sensor
  setupRadiationISR(&sensorRadiation);     // MUST be called before begin()
  sensorRadiation.begin();  
}

 

/*
 * Main Program
 */
 
void setup() {
  Wire.begin();

  Serial.begin(115200);  

  // Initialize SCD30 CO2 sensor
  airSensor.begin(); //This will cause readings to occur every two seconds

  // Initialize SPS30 air particle sensor
  sps30.setup();  

  // Initialize radiation sensor
  initializeRadiationSensor();
  
}

void loop() {

  // SCD30 CO2 sensor
  exportSCD30JSON(airSensor);

  // SPS30 air particle sensor
  sps30.readMeasurements();
  sps30.exportJSON();

  // Radiation Watch Type 5 radiation sensor w/digipot backpack
  sensorRadiation.exportJSON();  
  
  delay(1000);
}
