#include <MAX31855.h> // Include MAX31855 Sensor library
const int baudRate = 9600;
const int SSR = 8; // solid state relay
const int csH = 14; // heating plate cs
const int csSub = 16; // substrate cs
const uint8_t  SPI_MISO         =   MISO; ///< Master-In, Slave-Out PIN for SPI
const uint8_t  SPI_SYSTEM_CLOCK =    SCK; ///< System Clock PIN for SPI
float temprature = 0.0;
float Htemprature = 0.0;
MAX31855_Class hMAX31855; ///< Create an instance of MAX31855 for heating plate
MAX31855_Class subMAX31855; ///< Create an instance of MAX31855 for substrate
int steadyHeat = 44; // analog value for the pwm for keeping the temprature at a steady rate
int hErr = 1;
int subErr = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  while (!hMAX31855.begin(csH))
  {
    Serial.println(F("Unable to start csH MAX31855. Waiting 3 seconds."));
    delay(3000);
  } // of loop until device is located
  while (!subMAX31855.begin(csSub))
  {
    Serial.println(F("Unable to start csSub MAX31855. Waiting 3 seconds."));
    delay(3000);
  } // of loop until device is located
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int32_t hAmbientTemperature = hMAX31855.readAmbient(); // retrieve hMAX31855 die ambient temperature
  int32_t hProbeTemperature   = hMAX31855.readProbe();   // retrieve thermocouple probe temp
  uint8_t hFaultCode          = hMAX31855.fault();       // retrieve any error codes
  
  int32_t subAmbientTemperature = subMAX31855.readAmbient(); // retrieve subMAX31855 die ambient temperature
  int32_t subProbeTemperature   = subMAX31855.readProbe();   // retrieve thermocouple probe temp
  uint8_t subFaultCode          = subMAX31855.fault();       // retrieve any error codes
  
  temprature = (float)subProbeTemperature/1000; 
  Htemprature = (float)hProbeTemperature/1000; 

  if ( subFaultCode )                                     // Display error code if present
  {
    Serial.print("substrate Fault code ");
    Serial.print(subFaultCode);
    Serial.println(" returned.");
    subErr++;
    if (subErr >= 4)
    {
      //protocol
    }
  }
  else
  {
    subErr = 1;
    Serial.print("substrate Probe Temperature is   ");
    Serial.println((float)subProbeTemperature/1000,3);
  }
  
  if ( hFaultCode )                                     // Display error code if present
  {
    Serial.print("heating plate Fault code ");
    Serial.print(hFaultCode);
    Serial.println(" returned.");
    hErr++;
    if (hErr >= 4)
    {
      //protocol
    }
  }
  else
  {
    hErr = 1;
    Serial.print("heating plate Probe Temperature is   ");
    Serial.println((float)hProbeTemperature/1000,3);    
  }
  delay(1000);
}
