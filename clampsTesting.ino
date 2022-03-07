const int clampSLpush = 5; // clamp push
const int clampSLpull = 6; // clamp pull
const int NPN1 = 1; // left side sensor
const int NPN2 = 2; // right side sensor
int inputTime = 0;
int clamps = 1;
void setup() {
  pinMode(clampSLpush, OUTPUT);
  pinMode(clampSLpull, OUTPUT);
  digitalWrite(clampSLpush, HIGH);

}

void loop() {
  if (inputTime = 15)
  {
    if (clamps == 1)
    {
      digitalWrite(clampSLpush, LOW);
      delay(100);
      digitalWrite(clampSLpull, HIGH);
      clamps = 0;
    }else if (clamps == 0)
    {
      digitalWrite(clampSLpull, LOW);
      delay(100);
      digitalWrite(clampSLpush, HIGH);
      clamps = 1;
    }
    inputTime = 0;
  }
  inputTime = inputTime + 1;
  delay(1000); 

}
