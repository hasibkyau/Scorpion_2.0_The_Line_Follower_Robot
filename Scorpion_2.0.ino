#include "Scorpion.h"
#include <HCSR04.h>

int DutyCycle = 200;
int FrontWall = 20, RightWall = 20, LeftWall = 20; //Declaring Sonar sensor variable
int IRA = 19, IRB = 18, IRC = 5, IRD = 17, IRE = 16; //IR variable for declaring GPIO Pin
int A = 0, B = 0, C = 0, D = 0, E = 0, AIR; //IR variable for store value

bool obstacle = false;

HCSR04 sonarA(22, 23); //Front Sonor - initialisation class HCSR04 (trig pin , echo pin)
HCSR04 sonarB(2, 15); //Right Sonor - initialisation class HCSR04 (trig pin , echo pin)
HCSR04 sonarC(21, 4); //Left Sonor - initialisation class HCSR04 (trig pin , echo pin)

//Using class "Motor" {methods = Forward, Backward, Stop, Speed, Status}
Motor MotorR(27, 14, 26, 0);  // Right Motor - (in1, in2, en, pwm channel)
Motor MotorL(33, 25, 32, 1);  // Left Motor - (inputpin1, inputpin2, enablepin, pwmChannel[0-18])

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(IRA, INPUT);
  pinMode(IRB, INPUT);
  pinMode(IRC, INPUT);
  pinMode(IRD, INPUT);
  pinMode(IRE, INPUT);

  MotorR.Speed(DutyCycle);
  MotorL.Speed(DutyCycle);
  MotorR.Release();
  MotorL.Release();
  delay(1000);
  MotorR.Forward();
  MotorL.Forward();
}

void loop() {
  //ReadSonar(); // reading sonar data
  ReadIR(); // reading IR data
  AIR = A + B + C + D + E; // sum of all IR sensor


  if (FrontWall >= 25){
    if (AIR == 4)
    {
      if (A == 0) {
        _90dLeft();
      }
      else if (B == 0) {
        MedLeft();
      }
      else if (C == 0) {
        Straight();
      }
      else if (D == 0) {
        MedRight();
      }
      else if (E == 0) {
        _90dRight();
      }
    }
    else if (AIR == 3)
    {
      if (B == 0) {
        (A == 0) ? SharpLeft() : LowLeft();
      }
      else if (D == 0) {
        (C == 0) ? LowRight() : SharpRight();
      }
    }
    else if (AIR == 2 || AIR == 1) {
      (A == 1) ? _90dRight() : _90dLeft();
    }
    else if (AIR == 0)
    {
      DefaultTurn();
    }
    else if(AIR == 5)
    {
      Straight();
      delay(500);
      MotorL.Speed(0);MotorR.Speed(0);
      ReadIR(); ReadSonar();
      AIR = A + B + C + D + E;
      if(AIR =! 5){Straight();} // if found track. It was a blank track.
      else if(AIR == 5) // if no track
      {
        if(LeftWall <= 50 || RightWall <= 50){PassThroughWalls();} // if no track & found walls
        else{_180dturn();} //if no track and no Side Walls. The track ends here.
      }
    }
    
  }
  else if(FrontWall <= 5){
    AvoidWall();
  }
}

void Straight() {
  MotorR.Speed(255);
  MotorL.Speed(255);
}
void Stop() {
  MotorR.Speed(0);
  MotorL.Speed(0);
}

void LowLeft() {
  MotorL.Speed(230);
  MotorR.Speed(255);
}
void MedLeft() {
  MotorL.Speed(200);
  MotorR.Speed(255);
}
void SharpLeft() {
  MotorL.Speed(0);
  MotorR.Speed(255);
}

void LowRight() {
  MotorR.Speed(230);
  MotorL.Speed(255);
}
void MedRight() {
  MotorR.Speed(200);
  MotorL.Speed(255);
}

void SharpRight() {
  MotorR.Speed(0);
  MotorL.Speed(255);
}

void _90dLeft() {
  MotorR.Speed(255); MotorL.Speed(0);
  do {
    ReadIR();
    AIR = A + B + C + D + E;
  }
  while (!(AIR == 4 && C == 0));
}
void _90dRight() {
  MotorL.Speed(255); MotorR.Speed(0);
  do {
    ReadIR();
    AIR = A + B + C + D + E;
  }
  while (!(AIR == 4 && C == 0));
}
void _180dturn() {
  MotorR.Release(); MotorL.Release();
  MotorR.Speed(255); MotorL.Speed(255);
  delay(68);
  MotorL.Forward(); MotorR.Backward();
  do {
    ReadIR();
    AIR = A + B + C + D + E;
  }
  while (!(AIR == 4 && C == 0));
  MotorR.Release(); MotorL.Release();
  delay(68);
  MotorL.Forward(); MotorR.Forward();
}

void DefaultTurn(){
   true ? _90dRight() : _90dLeft();
}

void PassThroughWalls(){
  
}


// Reading all Sonar sensor and passing data to another function for processing
void ReadSonar() {
  FrontWall = sonarA.dist();
  RightWall = sonarB.dist();
  LeftWall = sonarC.dist();

  Serial.print("FrontWall=");
  Serial.print(FrontWall);

  Serial.print(" :RightWall= ");
  Serial.print(RightWall);

  Serial.print(" :LeftWall=");
  Serial.println(LeftWall);
}

// Read all IR sensor and passing data to another function for processing
void ReadIR() {
  A = digitalRead(IRA); // IR Sensor output pin connected to D1
  B = digitalRead(IRB); // IR Sensor output pin connected to D1
  C = digitalRead(IRC); // IR Sensor output pin connected to D1
  D = digitalRead(IRD); // IR Sensor output pin connected to D1
  E = digitalRead(IRE); // IR Sensor output pin connected to D1

  Serial.print(":A=");
  Serial.print(A); //left
  Serial.print(":B=");
  Serial.print(B);
  Serial.print(":C=");
  Serial.print(C); //right
  Serial.print(":D=");
  Serial.print(D);
  Serial.print(":E=");
  Serial.println(E);
}

void AvoidWall() {

}


