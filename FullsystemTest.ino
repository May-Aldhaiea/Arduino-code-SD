#include "Wire.h" //allows communication over i2c devices
#include "Keyboard.h"

// clamps pins
const int leftSLpush = 5; // left side clamp push
const int leftSLpull = 6; // left side clamp pull
const int rightSLpush = 9; // right side clamp push
const int rightSLpull = 10: // right side clamp pull
const int NPN1 = 1; // left side sensor
const int NPN2 = 2; // right side sensor

// suction cup pins
const int pressureInput = 11; //select the analog input pin for the pressure transducer
const int solonoid1 = 12; //suction cup push
const int solonoid2 = 13; // suction cup pull
const int NPNsuc = 7; // npn sensor for the suction cup
const int Vsolenoid = 26; // vacuum solenoid 
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
float pressureValue = 0; //variable to store the value coming from the pressure transducer
bool CheckSolonoid = false;
int inputTime = 0;

// lead screw pins
const int motorCW = 3; // clockwise motor
const int motorCCW = 4; // counter clockwise motor
const int SMS1 = 22; // position 1 proximity sensor
const int SMS2 = 24; // position 2 proximity sensor
int pos1 = 0; // check if position one is on
int pos2 = 0; // check if position two is on
const int RPM = 15; // 6% power

// heater pins
const int SSR = 8; // solid state relay
const int csH = 14; // heating plate cs
const int csSub = 16; // substrate cs
const int csAir = 18; // vantage air temp cs 

void setup() {
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
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
  pinMode(leftSLpush, OUTPUT);
  pinMode(leftSLpull, OUTPUT);
  pinMode(rightSLpush, OUTPUT);
  pinMode(rightSLpull, OUTPUT);
  pinMode(NPN1,INPUT);
  pinMode(NPN2,INPUT);

  // heater
  pinMode(SSR, OUTPUT);
  pinMode(csH, INPUT);
  pinMode(csSub, INPUT);
  pinMode(csAir, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}
