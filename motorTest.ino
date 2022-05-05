#include <Stepper.h>
#include <Keyboard.h>
const int baudRate = 115200;
const int motorStep = 6; // motor direction
const int motorDir = 7; // motor step
int RPM =600; // 500-600
int Step = 0;
int dir = -0;
int Readinput;
bool CW = false;
const int SMS1 = 47; // position 1 proximity sensor
const int SMS2 = 49; // position 2 proximity sensor
int posB = 0; // check if position one is on
int posA = 0; // check if position two is on
int maxSteps = 25985; // the required steps before the motor stops
int midway = 0;
int halt = 1;
int wait1 = 0, wait2 = 0;
Stepper motor(800, motorStep, motorDir);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  pinMode(motorStep, OUTPUT);
  pinMode(motorDir, OUTPUT);
  pinMode(SMS1, INPUT);
  pinMode(SMS2, INPUT);
  motor.setSpeed(RPM);
  digitalWrite(motorDir, HIGH); // clockwise going away from the motor
  Serial.println("program running");
  //digitalWrite(motorDir,LOW); // counter clockwise going to the motor
}

void loop() {
  // put your main code here, to run repeatedly:
    posB = digitalRead(SMS1); // read sensor 1
    posA = digitalRead(SMS2); // read sensor 2
  if (posB == 0)
  {
    Serial.print("sensor B is off");
  }else if (posB == 1)
  {
    Serial.print("sensor B is on");
  }
  if (posA == 0)
  {
    Serial.println("     sensor A is off");
  }else if (posA == 1)
  {
    Serial.println("     sensor A is on");
  }
  if (Serial.available() > 0)
  {
    Readinput = Serial.read();
  }
  switch(Readinput)
  {
    case ' ':
    dir = -1600;
    break;
    case 'd':
    dir = 1600;
    break;
    case 's':
    dir = 0;
    break;
    case 'a':
    Serial.println(Step); 
    Step = 0;
    delay(5000);
    break;
    /*case 'r':
    if (halt == 0)
    {
      halt = 1;
    }else
    {
      halt = 0;
    }
    break;*/
  }
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
        while (halt == 0)
        {
          if (Serial.available() > 0)
          {
            Readinput = Serial.read();
          }
          switch(Readinput)
          {
            case 'q':
            halt = 1;
            break;
          }
        }
        wait1 = 0;
        midway = 0;
      }
    }
    if (posA == 1)
    {
      dir = 0;
      motor.step(dir);
      //Readinput = 't';
      wait2++;
      if (wait2 == 15)
      {
        while (halt == 0)
        {
          if (Serial.available() > 0)
          {
            Readinput = Serial.read();
          }
          switch(Readinput)
          {
            case 'q':
            halt = 1;
            break;
          }
        }
        wait2 = 0;
        midway = 0;
      }
    }
  }
  /*if (halt == 0)
  {
    if (posB ==  1)
    {
      dir = 0;
      motor.step(dir);
      //Readinput = 't';
      wait++;
      if (wait == 10)
      {
        while (halt == 0)
        {
          if (Serial.available() > 0)
          {
            Readinput = Serial.read();
          }
          switch(Readinput)
          {
            case 'q':
            halt = 1;
            break;
          }
        }
        wait = 0;
      }
    }
  }*/
  if (posB == 0)
  {
    halt = 0;
  }
  if (posA == 0)
  {
    halt = 0;
  }
  /*if (Step >= maxSteps)
  {
    Serial.println("steps reached");
    dir = 0;
  }*/
  motor.step(dir);
  if (dir != 0)
  {
    Step = Step +1;
  }
  delayMicroseconds(1);
}
