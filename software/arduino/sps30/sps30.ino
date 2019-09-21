#include <sps30.h>
#include "SPS30Interface.h"

// Example arduino sketch, based on 
// https://github.com/Sensirion/embedded-sps/blob/master/sps30-i2c/sps30_example_usage.c

SPS30Interface sps30;

void setup() {
  Serial.begin(115200);
  delay(2000);

  sps30.setup();  

  delay(1000);
}

void loop() {
  sps30.readMeasurements();

  sps30.exportJSON();

  delay(1000);
}
