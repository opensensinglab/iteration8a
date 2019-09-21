// SPS30Interface.h

#if !defined(SPS30_INTERFACE_h) 
#define SPS30_INTERFACE_h

#include <sps30.h>
#include "Arduino.h"
#include <stdint.h>

// ERROR CODES
#define ERROR_SPS30_NOERROR             0   // No error
#define ERROR_SPS30_NOTFOUND            1   // Error finding sensor/probe failed
#define ERROR_SPS30_AUTOCLEAN           2   // Error setting auto-clean interval
#define ERROR_SPS30_STARTMEASUREMENTS   3   // Error starting measurements
#define ERROR_SPS30_DATAREADYFLAG       4   // Error reading data ready flag
#define ERROR_SPS30_DATANOTREADY        5   // Error: data is not ready to read
#define ERROR_SPS30_READMEASUREMENT     6   // Error reading measurement




class SPS30Interface {
  // Variables  
  public:
  //SensorBuffer* sbRadCounts;
  int8_t errorCode;
  struct sps30_measurement m;
  
  private:  
  int test;


  public:
  // Constructor/Destructor
  SPS30Interface();
  ~SPS30Interface();
  
  // Setup methods
  boolean setup();
  boolean inBlockingErrorState();
  
  // Data methods
  void readMeasurements();
          
  // Debug 
  void exportJSON();
};


#endif
