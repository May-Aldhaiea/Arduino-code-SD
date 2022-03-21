const int baudRate = 9600;
const int motorStep = 3; // motor direction
const int motorDir = 4; // motor step
int RPM = 15; // 6% power
int Step = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  pinMode(motorStep, OUTPUT);
  pinMode(motorDir, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Step  == 50){
    
    digitalWrite(motorDir, HIGH);
    delay(250);
    digitalWrite(motorDir,LOW);
    Step = 0;
  }
  analogWrite(motorStep, RPM);
  Step++;
  delay(3515);
}
