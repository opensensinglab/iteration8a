// Example code for the Interrupt-driven Radiation Watch Type 5 library
// This code is interrupt driven, so everything happens in the background. 
//#include <wprogram.h>
#include "SensorBuffer.h"
#include "SensorRadiation.h"

SensorBuffer sbRad(100);
SensorRadiation sensorRadiation(&sbRad);

int DIGIPOT_UD  = 5;
int DIGIPOT_CS  = 4;


void setup() {
  Serial.begin(115200);
  Serial.println("Initializing... "); 

  // Initialize the digipot
  initDigipot();
  delay(10);
  //setDigipot(10);
  setDigipotAbsolute(55);
  delay(10);

  // Initialize radiation sensor
  setupRadiationISR(&sensorRadiation);     // MUST be called before begin()
  sensorRadiation.begin();

}

int numLoop = 0;
void loop() {
  // Display counts per minute (cpm)
  float cpm = sensorRadiation.calculateCPM();
  Serial.print ("CPM: ");  
  Serial.println(cpm, DEC);
  
  // Display pulse width histogram
  sensorRadiation.debugPrint();

  numLoop += 1;
  Serial.print ("Loop: ");  
  Serial.println(numLoop, DEC);
  
  delay(2000);

  
}


/*
 * Digipot
 */
int digipotSetting = 32;

void initDigipot() {
  pinMode(DIGIPOT_UD, OUTPUT);
  pinMode(DIGIPOT_CS, OUTPUT);

  digitalWrite(DIGIPOT_CS, 1);    // Inactive
  digitalWrite(DIGIPOT_UD, 0);    // N/A
}

void incrementDigipot(int val) {
  digitalWrite(DIGIPOT_CS, 1);    // Inactive
  delayMicroseconds(10);
  digitalWrite(DIGIPOT_UD, 1);    // Set to increment
  delayMicroseconds(10);

  digitalWrite(DIGIPOT_CS, 0);    // Active
  delayMicroseconds(10);

  for (int i=0; i<val; i++) {
    // Pulse to increment
    digitalWrite(DIGIPOT_UD, 0);    
    delayMicroseconds(10);
    digitalWrite(DIGIPOT_UD, 1);    
    delayMicroseconds(10);
  }

  digitalWrite(DIGIPOT_CS, 1);    // Inactive  
  delayMicroseconds(10);
}

void decrementDigipot(int val) {
  digitalWrite(DIGIPOT_CS, 1);    // Inactive
  delayMicroseconds(10);
  digitalWrite(DIGIPOT_UD, 0);    // Set to decrement
  delayMicroseconds(10);

  digitalWrite(DIGIPOT_CS, 0);    // Active
  delayMicroseconds(10);

  for (int i=0; i<val; i++) {
    // Pulse to decrement
    digitalWrite(DIGIPOT_UD, 1);    
    delayMicroseconds(10);
    digitalWrite(DIGIPOT_UD, 0);    
    delayMicroseconds(10);
  }

  digitalWrite(DIGIPOT_CS, 1);    // Inactive  
  delayMicroseconds(10);
}


void setDigipot(int num) {
  int delta = digipotSetting - num;
  if (delta > 0) {
    incrementDigipot(delta);
  } 
  if (delta < 0) {
    decrementDigipot(-delta);
  }
  digipotSetting += delta;
}

void setDigipotAbsolute(int num) {
  incrementDigipot(64);
  decrementDigipot(64 - num);
  //digipotSetting = 32;

  //setDigipot(num);
}
