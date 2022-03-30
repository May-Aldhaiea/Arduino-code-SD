const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int pressureInput = 11; //select the analog input pin for the pressure transducer
const int Vac1 = 26; // vacuum solenoid 
const int Vac2 = 28; // vacuum solenoid 
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
float pressureValue = 0; //variable to store the value coming from the pressure transducer
bool CheckSolonoid = false; // a state diagram to know if the suction cup are on or off
bool vaccuum = false; // vaccuum state variable
bool psi = false; // checks if the target psi is reached
void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
  pinMode(pressureInput, INPUT);
  pinMode(Vac1, OUTPUT);
  pinMode(Vac2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Vac1,HIGH); 
  delay(100);
  digitalWrite(Vac2,HIGH);
  // Serial.println("low signal recieved");
  /*while (psi == true) // get the vacuum to the proper PSI
  {
    psi = digitalRead(pressureInput);
    delay(5);
  }*/
  psi = digitalRead(pressureInput);
  if (psi == true)
  {
   Serial.println("high signal recieved"); 
  }else if (psi == false)
  {
    Serial.println("low signal recieved");
  }
  psi = false;
  delay(2000);
  // turn off both vaccums
  digitalWrite(Vac1,LOW); 
  delay(100);
  digitalWrite(Vac2,LOW);
  delay(2000);
}
