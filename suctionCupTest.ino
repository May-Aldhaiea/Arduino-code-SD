#include "Wire.h" //allows communication over i2c devices
#include "Keyboard.h"

const int pressureInput = A0; //select the analog input pin for the pressure transducer
int solonoid1 = 12;
int solonoid2 = 13;
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
bool CheckSolonoid = false;
int inputTime = 0;
float pressureValue = 0; //variable to store the value coming from the pressure transducer

void setup() //setup routine, runs once when system turned on or reset
{
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
  pinMode(solonoid1, OUTPUT);
  pinMode(solonoid2, OUTPUT);
  digitalWrite(solonoid1, HIGH);
}

void loop() //loop routine runs over and over again forever
{
  if(inputTime >= 20)
  {
    if (Serial.available() > 0) 
    {
  
      // read incoming serial data:
  
      char inChar = Serial.read();
      if (inChar == '\n')
      {
        if (CheckSolonoid == false)
        {
          digitalWrite(solonoid2, LOW);
          delay(100);
          digitalWrite(solonoid1, HIGH);
          CheckSolonoid = true;
        }else if (CheckSolonoid == true)
        {
          digitalWrite(solonoid1, LOW);
          delay(100);
          digitalWrite(solonoid2, HIGH);
          CheckSolonoid = false;
        }
      }
  
    }
    inputTime = 0;
  }
  pressureValue = analogRead(pressureInput); //reads value from input pin and assigns to variable
  pressureValue = ((pressureValue-pressureZero)*pressuretransducermaxPSI)/(pressureMax-pressureZero); //conversion equation to convert analog reading to psi
  Serial.print(pressureValue, 1); //prints value from previous line to serial
  Serial.println("psi"); //prints label to serial
  Serial.print("Pressure:"); //prints label
  Serial.print(pressureValue, 1); //prints pressure value to lcd screen, 1 digit on float
  Serial.print("psi"); //prints label after value
  Serial.print("   "); //to clear the display after large values or negatives
  inputTime = inputTime + 1;
  delay(sensorreadDelay); //delay in milliseconds between read values
}
