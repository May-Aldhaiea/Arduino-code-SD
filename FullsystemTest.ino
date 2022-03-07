// variables
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
int stage = 1; // stage value should go from 1 to 5

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
float temprature = 0.0;

void setup() { // input for sensors, output for motors and control units
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
  pinMode(clampSLpush, OUTPUT);
  pinMode(clampSLpull, OUTPUT);
  pinMode(NPN1,INPUT);
  pinMode(NPN2,INPUT);

  // heater
  pinMode(SSR, OUTPUT);
  pinMode(csH, INPUT);
  pinMode(csSub, INPUT);
  pinMode(csAir, INPUT);

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
switch (stage){
  case 1:
  
  break;
  case 2:
  digitalWrite(clampSLpush, LOW);
  delay(100);
  digitalWrite(clampSLpull, HIGH);
  
  
  break;
  case 3:
  analogWrite(motorCW, RPM);
  while (pos2 == 0)
  {
    pos2 = digitalRead(SMS2);
  }
  digitalWrite(motorCW, LOW);
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
