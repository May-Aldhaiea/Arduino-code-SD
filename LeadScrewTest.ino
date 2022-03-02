const int baudRate = 9600;
const int motorCW = 3; // clockwise motor
const int motorCCW = 4; // counter clockwise motor
const int SMS1 = 14; // position 1 proximity sensor
const int SMS2 = 15; // position 2 proximity sensor
int pos1 = 0; // check if position one is on
int pos2 = 0; // check if position two is on
// const int RPM = 15; // 6% power


void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  pinMode(motorCW, OUTPUT);
  pinMode(motorCCW, OUTPUT);
  pinMode(SMS1, INPUT);
  pinMode(SMS2, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  pos1 = digitalRead(SMS1); // read sensor 1
  pos2 = digitalRead(SMS2); // read sensor 2
  if (pos1 == 1) // if fixture is in position 1 then turn on the lead screw to go to position 2
  {
    digitalWrite(motorCCW, LOW);
    delay(100);
    analogWrite(motorCW, RPM);
  }
  if (pos2 == 1) // if fixture is in position 2 then turn on the lead screw to go to position 1
  {
    digitalWrite(motorCW, LOW);
    delay(100);
    analogWrite(motorCCW, RPM);
  }
  delay(1000);
}
