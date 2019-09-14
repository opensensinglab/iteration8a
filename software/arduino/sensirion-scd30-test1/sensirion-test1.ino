// On Leonardo/Micro or others with hardware serial, use those!
// uncomment this line:
// #define pmsSerial Serial1
 
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (TX pin on sensor), leave pin #3 disconnected
// comment these two lines if using hardware serial

// Plantower A003
#include <SoftwareSerial.h>
#include <sps30.h>

SoftwareSerial pmsSerial(10, 11);

// SCD30
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h" 
SCD30 airSensor;


// CCS811
#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;
#define CCS811_ADDR 0x5B 

//CCS811 mySensor(CCS811_ADDR);

// BME280
#include "SparkFunBME280.h"
#include "Wire.h"
unsigned int sampleNumber = 0; //For counting number of CSV rows
BME280 myBME280Sensor;

// Plantower data structures
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;



/*
 * Setup
 */


void setup() {
  // our debugging output
  Serial.begin(115200);
  Serial.println("Initializing...");
 
  // Plantower A003 sensor baud rate is 9600
  //## pmsSerial.begin(9600);

/*
  // CCS811 
  if(!ccs.begin(CCS811_ADDR)){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }  

  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);
  

  // BME280
  setupBME280();
*/

  // Setup Sensirion particle sensor
  setupSensirion();


  // SCD30 
  airSensor.begin(); //This will cause readings to occur every two seconds
  
}

void setupSensirion() {
    s16 ret;
  u8 auto_clean_days = 4;
  u32 auto_clean;
  
  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }

#ifndef PLOTTER_FORMAT
  Serial.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }
    
  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("error starting measurement\n");
  }

#ifndef PLOTTER_FORMAT
  Serial.print("measurements started\n");
#endif /* PLOTTER_FORMAT */

#ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
  Serial.print("Your Arduino hardware has a limitation that only\n");
  Serial.print("  allows reading the mass concentrations. For more\n");
  Serial.print("  information, please check\n");
  Serial.print("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
  Serial.print("\n");
  delay(2000);
#endif

  delay(1000);

}

void readSensirion() {
  struct sps30_measurement m;
  char serial[SPS_MAX_SERIAL_LEN];
  u16 data_ready;
  s16 ret;

  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  } else {

#ifndef PLOTTER_FORMAT
    Serial.print("PM  1.0: ");
    Serial.println(m.mc_1p0);
    Serial.print("PM  2.5: ");
    Serial.println(m.mc_2p5);
    Serial.print("PM  4.0: ");
    Serial.println(m.mc_4p0);
    Serial.print("PM 10.0: ");
    Serial.println(m.mc_10p0);

#ifndef SPS30_LIMITED_I2C_BUFFER_SIZE
    Serial.print("NC  0.5: ");
    Serial.println(m.nc_0p5);
    Serial.print("NC  1.0: ");
    Serial.println(m.nc_1p0);
    Serial.print("NC  2.5: ");
    Serial.println(m.nc_2p5);
    Serial.print("NC  4.0: ");
    Serial.println(m.nc_4p0);
    Serial.print("NC 10.0: ");
    Serial.println(m.nc_10p0);

    Serial.print("Typical partical size: ");
    Serial.println(m.typical_particle_size);
#endif

    Serial.println();
    
#else
    // since all values include particles smaller than X, if we want to create buckets we 
    // need to subtract the smaller particle count. 
    // This will create buckets (all values in micro meters):
    // - particles        <= 0,5
    // - particles > 0.5, <= 1
    // - particles > 1,   <= 2.5
    // - particles > 2.5, <= 4
    // - particles > 4,   <= 10
    
    Serial.print(m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_1p0  - m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_2p5  - m.nc_1p0);
    Serial.print(" ");
    Serial.print(m.nc_4p0  - m.nc_2p5);
    Serial.print(" ");
    Serial.print(m.nc_10p0 - m.nc_4p0);
    Serial.println();


#endif /* PLOTTER_FORMAT */
    
  }

  delay(1000);  
}

void setupBME280() {
  //***Driver settings********************************//
  //commInterface can be I2C_MODE or SPI_MODE
  //specify chipSelectPin using arduino pin names
  //specify I2C address.  Can be 0x77(default) or 0x76
  
  //For I2C, enable the following and disable the SPI section
  myBME280Sensor.settings.commInterface = I2C_MODE;
  myBME280Sensor.settings.I2CAddress = 0x77;
  
  //For SPI enable the following and dissable the I2C section
  //myBME280Sensor.settings.commInterface = SPI_MODE;
  //myBME280Sensor.settings.chipSelectPin = 10;


  //***Operation settings*****************************//
  myBME280Sensor.settings.runMode = 3; //  3, Normal mode
  myBME280Sensor.settings.tStandby = 0; //  0, 0.5ms
  myBME280Sensor.settings.filter = 0; //  0, filter off
  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  myBME280Sensor.settings.tempOverSample = 1;
  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    myBME280Sensor.settings.pressOverSample = 1;
  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  myBME280Sensor.settings.humidOverSample = 1;
  
  Serial.print("Program Started\n");
  Serial.print("Starting BME280... result of .begin(): 0x");
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  //Calling .begin() causes the settings to be loaded
  Serial.println(myBME280Sensor.begin(), HEX);

  //Build a first-row of column headers
  Serial.print("\n\n");
  Serial.print("Sample,");
  Serial.print("T(deg C),");
  Serial.print("T(deg F),");
  Serial.print("P(Pa),");
  Serial.print("Alt(m),");
  Serial.print("Alt(ft),");
  Serial.print("%RH");
  Serial.println("");

}
 

/*
 * Loop
 */
 
void loop() {
  //while (!readPMSdata(&pmsSerial)) { }

  /*
  if (readPMSdata(&pmsSerial)) {
    // reading data was successful!
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    Serial.print("Particles > 50 um / 0.1L air:"); Serial.println(data.particles_100um);
    Serial.println("---------------------------------------");
  }
  */

/*
  // CCS811
  if(ccs.available()){
    float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.print(ccs.getTVOC());
      Serial.print("ppb   Temp:");
      Serial.println(temp);
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
*/

/*
  // BME280
  //Print each row in the loop
  //Start with temperature, as that data is needed for accurate compensation.
  //Reading the temperature updates the compensators of the other functions
  //in the background.
  Serial.print(sampleNumber);
  Serial.print(", ");
  Serial.print(myBME280Sensor.readTempC(), 2);
  Serial.print("C ,");
  Serial.print(myBME280Sensor.readTempF(), 3);
  Serial.print("F ,");
  Serial.print(myBME280Sensor.readFloatPressure(), 0);
  Serial.print("Pa ,");
  Serial.print(myBME280Sensor.readFloatAltitudeMeters(), 3);
  Serial.print("Alt(m) ,");
  Serial.print(myBME280Sensor.readFloatAltitudeFeet(), 3);
  Serial.print("Alt(ft) ,");
  Serial.print(myBME280Sensor.readFloatHumidity(), 0);
  Serial.println("%RH");
  
  sampleNumber++;
*/

  // Read SC30 CO2 sensor
  if (airSensor.dataAvailable()) {
    Serial.print("co2(ppm):");
    Serial.print(airSensor.getCO2());

    Serial.print(" temp(C):");
    Serial.print(airSensor.getTemperature(), 1);

    Serial.print(" humidity(%):");
    Serial.print(airSensor.getHumidity(), 1);

    Serial.println();
  } else {
    //Serial.println("No data");
  }



  // Read sensirion particle sensor
  readSensirion();


  
}
 
boolean readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}
