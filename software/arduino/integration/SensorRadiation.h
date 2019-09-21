// SensorRadiation.h

#if !defined(SENSOR_RADIATION_h) 
#define SENSOR_RADIATION_h

//#include <wprogram.h>
#include "SensorBuffer.h"

// Pin mappings
//#define RAD_SIG        2
//#define RAD_INT        0      // pin 7 is interrupt 2 on the Chipkit MAX32

// Normal output
//#define RAD_SIG        3
//#define RAD_INT        1      // pin 7 is interrupt 2 on the Chipkit MAX32

// Digipot output
#define RAD_SIG        2
#define RAD_INT        0      // pin 7 is interrupt 2 on the Chipkit MAX32


//#define RAD_NOISE      8

// Number of spectral bins for energy level spectra
#define RAD_SPEC_BINS    40

// Digital potentiometer pins
#define DIGIPOT_UD     5
#define DIGIPOT_CS    4


class SensorRadiation {
  // Variables  
  public:
  SensorBuffer* sbRadCounts;
  uint16_t intTime; 
  uint16_t spectralBins[RAD_SPEC_BINS];
  int digipotSetting;

  private:  
  unsigned long countStart;
  uint16_t curCount;

  public:
  // Constructor/Destructor
  SensorRadiation(SensorBuffer* sb);
  ~SensorRadiation();
  
  // Setup methods
  boolean begin();
  void enableInterrupt();
  void suspendInterrupt();
  void setIntegrationTime(uint16_t time);
  void clearSpectrum();
  
  // Data methods
  void addCount(uint16_t pulseWidth);
  float calculateCPM();
  void populateSensorBufferSpectrum(SensorBuffer* sb);

  // Digipot          
  void initDigipot();
  void incrementDigipot(int val);
  void decrementDigipot(int val);
  void setDigipot(int num);
  void setDigipotAbsolute(int num);  
  
  // Debug 
  void debugPrint();
  void exportJSON();

  
};


// Prototypes for interruipt service routine
void setupRadiationISR(SensorRadiation* sr);
void detectStartISR();


#endif
