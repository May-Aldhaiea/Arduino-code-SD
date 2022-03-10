#include <MAX31855.h> // Include MAX31855 Sensor library

// variables
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
int stage = 1; // stage value should go from 1 to 5
const int PES = 28; // photoelectric sensor pin
bool recieved = false; // checks if the substrate has been recieved 

// clamps pins
const int clampSLpush = 5; // clamp push
const int clampSLpull = 6; // clamp pull
const int NPN1 = 1; // left side sensor
const int NPN2 = 2; // right side sensor
bool clamps = true; // a state diagram to know if the clamps are on or off

// suction cup pins
const int pressureInput = 11; //select the analog input pin for the pressure transducer
const int solonoid1 = 12; //suction cup push
const int solonoid2 = 13; // suction cup pull
const int NPNsuc = 7; // npn sensor for the suction cup
const int Vsolenoid = 26; // vacuum solenoid 
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
float pressureValue = 0; //variable to store the value coming from the pressure transducer
bool CheckSolonoid = false; // a state diagram to know if the suction cup are on or off
bool vaccuum = false; // vaccuum state variable
bool psi = false; // checks if the target psi is reached
int pressure = 0;

// lead screw pins
const int motorCW = 3; // clockwise motor
const int motorCCW = 4; // counter clockwise motor
const int SMS1 = 22; // position 1 proximity sensor
const int SMS2 = 24; // position 2 proximity sensor
int pos1 = 0; // check if position one is on
int pos2 = 0; // check if position two is on
const int RPM = 15; // 6% power
bool CW = false; // state diagram for stepper motor

// heater pins
const int SSR = 8; // solid state relay
const int csH = 14; // heating plate cs
const int csSub = 16; // substrate cs
const int csAir = 18; // vantage air temp cs 
const uint8_t  SPI_MISO         =   MISO; ///< Master-In, Slave-Out PIN for SPI
const uint8_t  SPI_SYSTEM_CLOCK =    SCK; ///< System Clock PIN for SPI
float temprature = 0.0;
MAX31855_Class hMAX31855; ///< Create an instance of MAX31855 for heating plate
MAX31855_Class subMAX31855; ///< Create an instance of MAX31855 for substrate
MAX31855_Class airMAX31855; ///< Create an instance of MAX31855 for the air temprature

void setup() { // input for sensors, output for motors and control units
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
  #ifdef  __AVR_ATmega32U4__  // If this is a 32U4 processor, then wait 3 seconds for the interface to initialize
    delay(3000);
  #endif
  pinMode(PES,INPUT); 
  // suction cup
  pinMode(solonoid1, OUTPUT); // suction cups
  pinMode(solonoid2, OUTPUT);// put your setup code here, to run once:
  pinMode(NPNsuc, INPUT);
  pinMode(Vsolenoid, OUTPUT);
  
  // lead screw
  pinMode(motorCW, OUTPUT);
  pinMode(motorCCW, OUTPUT);
  pinMode(SMS1, INPUT);
  pinMode(SMS2, INPUT);
   
  //clamps
  pinMode(clampSLpush, OUTPUT);
  pinMode(clampSLpull, OUTPUT);
  pinMode(NPN1,INPUT);
  pinMode(NPN2,INPUT);

  // heater
  pinMode(SSR, OUTPUT);
  //pinMode(csH, INPUT);
  //pinMode(csSub, INPUT);
  //pinMode(csAir, INPUT);
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
  while (!airMAX31855.begin(csAir))
  {
    Serial.println(F("Unable to start csAir MAX31855. Waiting 3 seconds."));
    delay(3000);
  } // of loop until device is located

  // setup phase
  // turn on solonoid for suction cup
  digitalWrite(solonoid1, HIGH);
  //turn on solonoid for clamps
  digitalWrite(clampSLpush, HIGH);
  // heater on

  // lead screw setup
  pos1 = digitalRead(SMS1); // read sensor 1
  pos2 = digitalRead(SMS2); // read sensor 2
  
}

void loop() {
  int32_t hAmbientTemperature = hMAX31855.readAmbient(); // retrieve hMAX31855 die ambient temperature
  int32_t hProbeTemperature   = hMAX31855.readProbe();   // retrieve thermocouple probe temp
  uint8_t hFaultCode          = hMAX31855.fault();       // retrieve any error codes
  
  int32_t subAmbientTemperature = subMAX31855.readAmbient(); // retrieve subMAX31855 die ambient temperature
  int32_t subProbeTemperature   = subMAX31855.readProbe();   // retrieve thermocouple probe temp
  uint8_t subFaultCode          = subMAX31855.fault();       // retrieve any error codes
  
  int32_t airAmbientTemperature = airMAX31855.readAmbient(); // retrieve airMAX31855 die ambient temperature
  int32_t airProbeTemperature   = airMAX31855.readProbe();   // retrieve thermocouple probe temp
  uint8_t airFaultCode          = airMAX31855.fault();       // retrieve any error codes
  temprature = (float)subProbeTemperature/1000; 
switch (stage){
  case 1:
  while (recieved == false) // wait for the substrate to come from the EFEM
  {
    recieved = digitalRead(PES); // check if substrate been recieved
    delay(1000);
  }
  while (psi == false) // get the vacuum to the proper PSI
  {
    psi = digitalRead(NPNsuc);
    pressure = pressure + 1;
    analogWrite(Vsolenoid, pressure);
    delay(5);
  }
  digitalWrite(solonoid1, LOW);
  delay(100);
  digitalWrite(solonoid2, HIGH);
  stage = 2;
  
  break;
  case 2:
  digitalWrite(clampSLpush, LOW);
  delay(100);
  digitalWrite(clampSLpull, HIGH);
  while (temprature < 120.5)
  {
    // implement heating mechanism
    temprature = (float)subProbeTemperature/1000; 
  }
  // add temprature protocol here
  // vantage protocol
  stage = 3;
  break;
  case 3:
  analogWrite(motorCW, RPM);
  while (pos2 == 0)
  {
    pos2 = digitalRead(SMS2);
  }
  digitalWrite(motorCW, LOW);
  while (temprature > 60)
  {
    // implement temprature control here
    temprature = (float)subProbeTemperature/1000; 
  }
  if (pos2 == true && (temprature > 59.5 && temprature < 60.5)
  {
    //communicate with UI
    stage = 4;
  }
  
  break;
  case 4:
  analogWrite(motorCCW, RPM);
  while (pos1 == 0)
  {
    pos1 = digitalRead(SMS1);
  }
  digitalWrite(motorCCW, LOW);
  if (pos1 == true && (temprature > 59.5 && temprature < 60.5)
  {
    // communicate with UI
    stage = 5;
  }
  
  break;
  case 5:
  digitalWrite(solonoid2, LOW);
  delay(100);
  digitalWrite(solonoid1, HIGH);
  digitalWrite(clampSLpull, LOW);
  delay(100);
  digitalWrite(clampSLpush, HIGH);
  
  break;
}
delay(1000);

}
