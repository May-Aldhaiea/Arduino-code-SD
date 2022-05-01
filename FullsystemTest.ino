#include <MAX31855.h> // Include MAX31855 Sensor library
#include <Stepper.h>
#include <Keyboard.h>

// variables
const int baudRate = 115200; //constant integer to set the baud rate for serial monitor
int stage = 0; // stage value should go from 1 to 5
const int PES = 8; // photoelectric sensor pin
bool recieved = false; // checks if the substrate has been recieved 
int substrateReady = 0; // checks if the substrate
int Readinput; // read info into the GUI

// clamps pins
const int clampSLpush = 25; // clamp push
const int clampSLpull = 27; // clamp pull
const int NPN1 = 35; // left side sensor
const int NPN2 = 37; // right side sensor
bool Lclamps = true; // a state diagram to know if the left clamp is on or off
bool Rclamps = true; // a state diagram to know if the right clamps is on or off

// suction cup pins
const int pressureInput = 9; //select the analog input pin for the pressure transducer
const int solonoid1 = 31; //suction cup push
const int solonoid2 = 29; // suction cup pull
const int NPNsuc = 33; // npn sensor for the suction cup
const int valve = 23; // valve for the vacuum  
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
float pressureValue = 0; //variable to store the value coming from the pressure transducer
bool CheckSolonoid = false; // a state diagram to know if the suction cup are on or off
bool vaccuum = false; // vaccuum state variable
bool psi = false; // checks if the target psi is reached
//int pressure = 0;

// lead screw pins
const int motorStep = 6; // motor direction
const int motorDir = 7; // motor step
int RPM =1000; // 500-600 - 1000 is ideal
int Step = 0;
int dir = -0;
bool CW = false;
const int SMS1 = 47; // position 1 proximity sensor
const int SMS2 = 49; // position 2 proximity sensor
int posA = 0; // check if position one is on
int posB = 0; // check if position two is on
int maxSteps = 25985; // the required steps before the motor stops
int midway = 0;
int halt = 0;
int wait1 = 0, wait2 = 0;
Stepper motor(800, motorStep, motorDir);

// heater pins
const int SSR = 11; // solid state relay
const int csH = 12; // heating plate cs
//const int csSub = 16; // substrate cs
const uint8_t  SPI_MISO         =   MISO; ///< Master-In, Slave-Out PIN for SPI
const uint8_t  SPI_SYSTEM_CLOCK =    SCK; ///< System Clock PIN for SPI
float htemprature = 0.0;
//float subtemprature = 0.0;
MAX31855_Class hMAX31855; ///< Create an instance of MAX31855 for heating plate
//MAX31855_Class subMAX31855; ///< Create an instance of MAX31855 for substrate
int steadyHeat = 44; // analog value for the pwm for keeping the temprature at a steady rate
int hErr = 1;
int subErr = 1;
int32_t hAmbientTemperature;
int32_t hProbeTemperature;
uint8_t hFaultCode;
int highHeat = 1;
  
/*int32_t subAmbientTemperature;
int32_t subProbeTemperature;
uint8_t subFaultCode;*/

void setup() { // input for sensors, output for motors and control units
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
  pinMode(PES,INPUT); 
  // suction cup
  pinMode(solonoid1, OUTPUT); // suction cups
  pinMode(solonoid2, OUTPUT);// put your setup code here, to run once:
  pinMode(NPNsuc, INPUT);
  pinMode(pressureInput, INPUT);
  pinMode(valve, OUTPUT);
  
  // lead screw
  pinMode(motorStep, OUTPUT);
  pinMode(motorDir, OUTPUT);
  pinMode(SMS1, INPUT);
  pinMode(SMS2, INPUT);
  motor.setSpeed(RPM);
   
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
 /* while (!subMAX31855.begin(csSub))
  {
    Serial.println(F("Unable to start csSub MAX31855. Waiting 3 seconds."));
    delay(3000);
  }*/ // of loop until device is located

  // setup phase
  // turn on solonoid for suction cup
  digitalWrite(solonoid1, HIGH);
  // wait for the fixture to go up
  Serial.println("waiting for fixture to go up");
  CheckSolonoid = digitalRead(NPNsuc);  
  while (CheckSolonoid == true)
  {
    CheckSolonoid = digitalRead(NPNsuc);
    delay(5);
  }
  //turn on solonoid for clamps
  digitalWrite(clampSLpush, HIGH);
  Serial.println("waiting for clamps to go up");
    while (Lclamps == true)
    {
      digitalWrite(clampSLpush, HIGH);
      Lclamps = digitalRead(NPN1);
      delay(100);
    }
    while (Rclamps == true)
    {
      digitalWrite(clampSLpush, HIGH);
      Rclamps = digitalRead(NPN2);
      delay(5);
    } 

  // lead screw setup
  posA = digitalRead(SMS1); // read sensor 1
  posB = digitalRead(SMS2); // read sensor 2
  if (posA == 0 || posB == 1)
  {
    halt = 1;
    dir = -1600;
    Serial.println("moving the fixture back to the original position (position B)");
    while (halt == 1)
    {
      posA = digitalRead(SMS1); // read sensor 1
      posB = digitalRead(SMS2); // read sensor 2
      if (posB == 0 && posA == 0)
      {
        midway = 1;
      }
      if (midway == 1)
      {
        if (posA == 1)
        {
          dir = 0;
          motor.step(dir);
          //Readinput = 't';
          wait2++;
          if (wait2 == 15)
          {
            wait2 = 0;
            midway = 0;
            halt = 0;
          }
        }
      }
      motor.step(dir);
    }
  }

  // heater on
  digitalWrite(SSR, HIGH);
  hCheckTemperature();  
  Serial.println("waiting for heating plate to reach certain temperature");
  while (htemprature <= 60)
  {
    hCheckTemperature();
    delay(1000);
  }
  digitalWrite(SSR,LOW);
  
  // communicate to the vantage
  stage = 1;
  delay(1000);
}

void hCheckTemperature()
{
  hAmbientTemperature = hMAX31855.readAmbient(); // retrieve hMAX31855 die ambient temperature
  hProbeTemperature   = hMAX31855.readProbe();   // retrieve thermocouple probe temp
  hFaultCode          = hMAX31855.fault();       // retrieve any error codes
  if ( hFaultCode )                                     // Display error code if present
  {
    hErr++;
    if (hErr >= 4)
    {
      //protocol
    }
  }
  else
  {
    hErr = 1;
    htemprature = (float)hProbeTemperature/1000;     
  }
}
/*void subCheckTemperature()
{
  subAmbientTemperature = subMAX31855.readAmbient(); // retrieve subMAX31855 die ambient temperature
  subProbeTemperature   = subMAX31855.readProbe();   // retrieve thermocouple probe temp
  subFaultCode          = subMAX31855.fault();       // retrieve any error codes
  if ( subFaultCode )                                     // Display error code if present
  {
    subErr++;
    if (subErr >= 4)
    {
      //protocol
    }
  }
  else
  {
    subErr = 1;
    subtemprature = (float)subProbeTemperature/1000;
  }
}*/
void loop() {

  hCheckTemperature();
  //subCheckTemperature();
  posA = digitalRead(SMS1); // read sensor 1
  posB = digitalRead(SMS2); // read sensor 2
  
  /*switch(highHeat)
  {
    case 1:
    digitalWrite(SSR, HIGH);
    hCheckTemperature();  
    while (htemprature <= 60)
    {
      hCheckTemperature();
      delay(1000);
    }
    digitalWrite(SSR,LOW);
    break;
    case 0:
    digitalWrite(SSR, HIGH);
    hCheckTemperature();  
    while (htemprature <= 40)
    {
      hCheckTemperature();
      delay(1000);
    }
    digitalWrite(SSR,LOW);
    break;
  }*/
    digitalWrite(SSR, HIGH);
    hCheckTemperature();  
    while (htemprature <= 60)
    {
      hCheckTemperature();
      delay(1000);
    }
    digitalWrite(SSR,LOW);
switch (stage){
  case 1:
  // wait for substrate to be recieved
  recieved = digitalRead(PES);
  Serial.println("waiting for substrate");
  while (substrateReady < 10) // wait for the substrate to come from the EFEM
  {
    recieved = digitalRead(PES); // check if substrate been recieved
    if (recieved == false)
    {
      substrateReady++;
    }else
    {
      substrateReady = 0;
    }
    delay(1000);
  }
  substrateReady = 0;
  // open vaccum valve
  digitalWrite(valve,HIGH); 
  psi = digitalRead(pressureInput);
  Serial.println("waiting for vacuum to reach the proper PSI");
  while (psi == true) // get the vacuum to the proper PSI
  {
    psi = digitalRead(pressureInput);
    delay(100);
  }
  delay(1000);
  //lower fixture
  digitalWrite(solonoid1, LOW);
  delay(100);
  digitalWrite(solonoid2, HIGH);
  CheckSolonoid = digitalRead(NPNsuc);
  // wait for the solenoid to be down
  Serial.println("waiting for the fixture to go down");
  while (CheckSolonoid = false)
  {
    CheckSolonoid = digitalRead(NPNsuc);
    delay(100);
  }
  stage = 2;
  
  break;
  case 2:
  digitalWrite(clampSLpush, LOW);
  delay(100);
  digitalWrite(clampSLpull, HIGH);
  Lclamps = digitalRead(NPN1);
  Rclamps = digitalRead(NPN2);
  // wait for the sensors to confirm that the clamps are down
  Serial.println("waiting for the clamps to latch onto the fixture");
  while (Lclamps == false)
  {
    Lclamps = digitalRead(NPN1);
    delay(100);
  }
  while (Rclamps == false)
  {
    Rclamps = digitalRead(NPN2);
    delay(100);
  }
  // turn off the valve for the vacuum
  /*digitalWrite(valve,LOW); 
  subCheckTemperature();
  digitalWrite(SSR, HIGH);
  Serial.println("waiting for the substrate to get to 60C");
  while (subtemprature <= 60)
  {
    //digitalWrite(SSR, HIGH);
    subCheckTemperature();
    // communicate to vantage
    delay(1000);
    highHeat = 1;
  }
  digitalWrite(SSR, LOW);*/
  // add temprature protocol here
  // vantage protocol
  stage = 3;
  break;
  case 3:
  // move the substrate into the vantage
  halt = 1;
  dir = 1600;
  Serial.println("moving the fixture toward the vantage (position A)");
  while (halt == 1)
  {
    posA = digitalRead(SMS1); // read sensor 1
    posB = digitalRead(SMS2); // read sensor 2
    if (posB == 0 && posA == 0)
    {
      midway = 1;
    }
    if (midway == 1)
    {
      if (posB == 1)
      {
        dir = 0;
        motor.step(dir);
        //Readinput = 't';
        wait1++;
        if (wait1 == 15)
        {
          wait1 = 0;
          midway = 0;
          halt = 0;
        }
      }
    }
    motor.step(dir);
  }
  // check temprature
  /*digitalWrite(SSR, HIGH);
  subCheckTemperature();
  Serial.println("cooling the substrate and the heating pad to 60C");
  while (subtemprature <= 60)
  {
    // implement temprature control here
    subCheckTemperature();
    highHeat = 0;
  }*/
  digitalWrite(SSR, LOW);
  if (posB == true); //&& (subtemprature > 59.5 && subtemprature < 60.5))
  {
    //communicate with UI
    stage = 4;
  }
  
  break;
  case 4:
  // move the substrate back to it's original position
  halt = 1;
  dir = -1600;
  Serial.println("moving the fixture back to the original position (position B)");
  while (halt == 1)
  {
    posA = digitalRead(SMS1); // read sensor 1
    posB = digitalRead(SMS2); // read sensor 2
    if (posB == 0 && posA == 0)
    {
      midway = 1;
    }
    if (midway == 1)
    {
      if (posA == 1)
      {
        dir = 0;
        motor.step(dir);
        //Readinput = 't';
        wait2++;
        if (wait2 == 15)
        {
          wait2 = 0;
          midway = 0;
          halt = 0;
        }
      }
    }
    motor.step(dir);
  }
  if (posA == true); //&& (subtemprature > 59.5 && subtemprature < 60.5))
  {
    // communicate with UI
    stage = 5;
  }
  
  break;
  case 5:
  // turn on the valve
  digitalWrite(valve,HIGH); 
  psi = digitalRead(pressureInput);
  Serial.println("waiting for vacuum to reach the proper PSI");
  while (psi == false) // get the vacuum to the proper PSI
  {
    psi = digitalRead(pressureInput);
    delay(100);
  }
  delay(1000);
  // release clamps
  digitalWrite(clampSLpull, LOW);
  delay(100);
  digitalWrite(clampSLpush, HIGH);
  Lclamps = digitalRead(NPN1);
  Rclamps = digitalRead(NPN2);
  // wait for the sensors to confirm that the clamps are up
  Serial.println("waiting for the clamps to release");
  while (Lclamps == true)
  {
    Lclamps = digitalRead(NPN1);
    delay(100);
  }
  while (Rclamps == true)
  {
    Rclamps = digitalRead(NPN2);
    delay(100);
  }
  delay(1000);
  // raise fixture
  digitalWrite(solonoid2, LOW);
  delay(100);
  digitalWrite(solonoid1, HIGH);
  // wait for the solenoid to be up
  Serial.println("waiting for fixture to go up");
  CheckSolonoid = digitalRead(NPNsuc);
  while (CheckSolonoid == true)
  {
    CheckSolonoid = digitalRead(NPNsuc);
    delay(1000);
  }
  delay(1000);
  // close vaccuum 
  digitalWrite(valve,LOW); 
  psi = digitalRead(pressureInput);
  Serial.println("waiting for vacuum to turn off");
  while (psi == false) // wait for the vacuum to turn off
  {
    psi = digitalRead(pressureInput);
    delay(100);
  }
  recieved = digitalRead(PES);
  Serial.println("waiting for substrate to be picked up by the EFM");
  while (recieved == false) // wait for the substrate to be taken out
  {
    recieved = digitalRead(PES); // check if substrate been taken out
    delay(100);
  }
  delay(5000);
  Serial.println("Process is complete");
  stage = 1;
  break;
}
delay(1000);

}
