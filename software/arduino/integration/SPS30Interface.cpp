#include "SPS30Interface.h"
#include <sps30.h>
#include "Arduino.h"
#include <stdint.h>

// Constructor/Destructor
SPS30Interface::SPS30Interface() {  

}

SPS30Interface::~SPS30Interface() {  

}


// Setup
boolean SPS30Interface::setup() {
  s16 ret;
  u8 auto_clean_days = 4;
  u32 auto_clean;

  // Reset error code
  errorCode = ERROR_SPS30_NOERROR;

  int count = 0;
  while (sps30_probe() != 0) {
    // Serial.print("SPS sensor probing failed\n");
    errorCode = ERROR_SPS30_NOTFOUND;
    
    // Retry up to 5 times
    count += 1;
    if (count > 5) return false;
    
    delay(500);
  }

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    // Serial.print("error setting the auto-clean interval: ");
    // Serial.println(ret);
    errorCode = ERROR_SPS30_AUTOCLEAN;
  }
    
  ret = sps30_start_measurement();
  if (ret < 0) {
    // Serial.print("error starting measurement\n");
    errorCode = ERROR_SPS30_STARTMEASUREMENTS;
  }  
}

boolean SPS30Interface::inBlockingErrorState() {
  if ((errorCode == ERROR_SPS30_NOTFOUND) || 
      (errorCode == ERROR_SPS30_STARTMEASUREMENTS)) {
        return true;
  }

  // Default return
  return false;
}

// Take measurements
void SPS30Interface::readMeasurements() {
  u16 data_ready;
  s16 ret;

  // If the sensor is in a blocking error state, do not attempt to take a measurement
  if (inBlockingErrorState() == true) {
    return;
  }

  // Reset error code
  errorCode = ERROR_SPS30_NOERROR;

  ret = sps30_read_data_ready(&data_ready);
  if (ret < 0) {
    // Serial.print("error reading data-ready flag: ");
    // Serial.println(ret);
    errorCode = ERROR_SPS30_DATAREADYFLAG;
    return;    
  } 
  
  if (!data_ready) {
    // Serial.print("data not ready, no new measurement available\n");
    errorCode = ERROR_SPS30_DATANOTREADY;
    return;    
  } 

  // Take measurement
  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    // Serial.print("error reading measurement\n");
    errorCode = ERROR_SPS30_READMEASUREMENT;
  }
  
}


// Export
void SPS30Interface::exportJSON() {
  Serial.print("{\"airparticles\": {");

  // Sensor
  Serial.print("\"sensor\": \"SPS30\"");

  // Error code
  Serial.print(", \"errorCode\": ");
  Serial.print(errorCode);

  // PM1.0 to PM10.0
  Serial.print(", \"pm1p0\": ");
  Serial.print(m.mc_1p0);
  Serial.print(", \"pm2p5\": ");
  Serial.print(m.mc_4p0);
  Serial.print(", \"pm4p0\": ");
  Serial.print(m.mc_4p0);
  Serial.print(", \"pm10p0\": ");
  Serial.print(m.mc_10p0);

  Serial.print(", \"nc0p5\": ");
  Serial.print(m.nc_0p5);
  Serial.print(", \"nc1p0\": ");
  Serial.print(m.nc_1p0);
  Serial.print(", \"nc2p5\": ");
  Serial.print(m.nc_2p5);
  Serial.print(", \"nc4p0\": ");
  Serial.print(m.nc_4p0);
  Serial.print(", \"nc10p0\": ");
  Serial.print(m.nc_10p0);

  // Typical partical size
  Serial.print(", \"tps\": ");
  Serial.print(m.typical_particle_size);

  Serial.print("} }");
  Serial.println("");
  
}
