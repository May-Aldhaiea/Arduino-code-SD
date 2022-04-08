#include "Wire.h" //allows communication over i2c devices
#include "Keyboard.h"
// general
const int PES = 28; // photoelectric sensor pin
bool recieved = false; // checks if the substrate has been recieved 


// clamps pins
const int clampSLpush = 5; // clamp push
const int clampSLpull = 6; // clamp pull
const int NPN1 = 1; // left side sensor
const int NPN2 = 2; // right side sensor
bool Lclamps = true; // a state diagram to know if the left clamp is on or off
bool Rclamps = true; // a state diagram to know if the right clamps is on or off

// suction cup
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int pressureInput = 11; //select the analog input pin for the pressure transducer
const int solonoid1 = 12; //suction cup push
const int solonoid2 = 13; // suction cup pull
const int NPNsuc = 7; // npn sensor for the suction cup
const int valve = 26; // valve for the vacuum  
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
float pressureValue = 0; //variable to store the value coming from the pressure transducer
bool CheckSolonoid = false; // a state diagram to know if the suction cup are on or off
bool vaccuum = false; // vaccuum state variable
bool psi = false; // checks if the target psi is reached
int stage = 0;
void setup() //setup routine, runs once when system turned on or reset
{
  pinMode(PES,INPUT);
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
  // suction cup
  pinMode(solonoid1, OUTPUT); // suction cups
  pinMode(solonoid2, OUTPUT);// put your setup code here, to run once:
  pinMode(NPNsuc, INPUT);
  pinMode(pressureInput, INPUT);
  pinMode(valve, OUTPUT);
  

  // valve
  pinMode(clampSLpush, OUTPUT);
  pinMode(clampSLpull, OUTPUT);
  pinMode(NPN1,INPUT);
  pinMode(NPN2,INPUT);
  
  digitalWrite(solonoid1, HIGH);
  CheckSolonoid = digitalRead(NPNsuc);
  while (CheckSolonoid == true)
  {
    CheckSolonoid = digitalRead(NPNsuc);
    delay(5);
  }
  stage = 1;
}

void loop() //loop routine runs over and over again forever
{
  switch(stage)
  {
    case 1:
    recieved = digitalRead(PES);
    while (recieved == false) // wait for the substrate to come from the EFEM
    {
      recieved = digitalRead(PES); // check if substrate been recieved
      delay(1000);
    }
      // open vaccum valve
    digitalWrite(valve,HIGH); 
    while (psi == false) // get the vacuum to the proper PSI
    {
      psi = digitalRead(pressureInput);
      delay(5);
    }
    delay(1000);
    //lower fixture
    digitalWrite(solonoid1, LOW);
    delay(100);
    digitalWrite(solonoid2, HIGH);
    CheckSolonoid = digitalRead(NPNsuc);
    // wait for the solenoid to be down
    while (CheckSolonoid = false)
    {
      CheckSolonoid = digitalRead(NPNsuc);
      delay(5);
    }
    stage = 2;
    break;
    case 2: 
    // lower clamps
    digitalWrite(clampSLpush, LOW);
    delay(100);
    digitalWrite(clampSLpull, HIGH);
    Lclamps = digitalRead(NPN1);
    Rclamps = digitalRead(NPN2);
    // wait for the sensors to confirm that the clamps are down
    while (Lclamps == false)
    {
      Lclamps = digitalRead(NPN1);
      delay(5);
    }
    while (Rclamps == false)
    {
      Rclamps = digitalRead(NPN2);
      delay(5);
    }   
    delay(1000);
    // close vacuum
    digitalWrite(valve,LOW); 
    psi = digitalRead(pressureInput);
    while (psi == true) // wait for the pressure to be gone
    {
      psi = digitalRead(pressureInput);
      delay(5);
    }
    delay(5000);
    // open vaccum valve
    digitalWrite(valve,HIGH); 
    while (psi == false) // get the vacuum to the proper PSI
    {
      psi = digitalRead(pressureInput);
      delay(5);
    }
    delay(1000);
    // raise clamps
    digitalWrite(clampSLpull, LOW);
    delay(100);
    digitalWrite(clampSLpush, HIGH);
    Lclamps = digitalRead(NPN1);
    Rclamps = digitalRead(NPN2);
    // wait for the sensors to confirm that the clamps are up
    while (Lclamps == true)
    {
      Lclamps = digitalRead(NPN1);
      delay(5);
    }
    while (Rclamps == true)
    {
      Rclamps = digitalRead(NPN2);
      delay(5);
    }
    delay(500);
    //raise  fixture
    digitalWrite(solonoid2, LOW);
    delay(100);
    digitalWrite(solonoid1, HIGH);
    CheckSolonoid = digitalRead(NPNsuc);
    // wait for the solenoid to be up
    while (CheckSolonoid = true)
    {
      CheckSolonoid = digitalRead(NPNsuc);
      delay(5);
    }
    delay(1000);
    // close vacuum
    digitalWrite(valve,LOW); 
    psi = digitalRead(pressureInput);
    while (psi == true) // wait for the pressure to be gone
    {
      psi = digitalRead(pressureInput);
      delay(5);
    }
    delay(5000);
    stage = 1;
    break;
  }

  delay(1000); //delay in milliseconds between read values
}
