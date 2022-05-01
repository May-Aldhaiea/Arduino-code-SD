#include "Wire.h" //allows communication over i2c devices
#include "Keyboard.h"

const int pressureInput = A0; //select the analog input pin for the pressure transducer
int solonoid1 = 25;
int solonoid2 = 27;
int solonoid3 = 31;
int solonoid4 = 29;
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
bool CheckSolonoid = false;
int inputTime = 0;
float pressureValue = 0; //variable to store the value coming from the pressure transducer
int valve = 23;
void setup() //setup routine, runs once when system turned on or reset
{
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
  pinMode(solonoid1, OUTPUT);
  pinMode(solonoid2, OUTPUT);
  pinMode(solonoid3, OUTPUT);
  pinMode(solonoid4, OUTPUT);
  digitalWrite(solonoid1, HIGH);
  pinMode(valve, OUTPUT);
  //digitalWrite(solonoid3,LOW);
}

void loop() //loop routine runs over and over again forever
{
  if(inputTime >= 8) //timer to switch which solonoid is active every 2 seconds
  {
      //digitalWrite(valve,HIGH);
       if (CheckSolonoid == false) // if this statement execute then push the solonoid
       {
         digitalWrite(solonoid2, LOW);
         delay(100);
         digitalWrite(solonoid1, HIGH);
         CheckSolonoid = true;
       }else if (CheckSolonoid == true) // if this statement execute then pull the solonoid
       {
         digitalWrite(solonoid1, LOW);
         delay(100);
         digitalWrite(solonoid2, HIGH);
         CheckSolonoid = false;
       }
    inputTime = 0;
  }
  inputTime = inputTime + 1;
  delay(sensorreadDelay); //delay in milliseconds between read values
}
