#include "Scorpion.h"
#include <HCSR04.h>

//Declaring Sonar sensor variable
int S1, S2, S3, SA = 230, SB = 255;
//Declaring digital pin for IR sensor
int IRA = 32, IRB = 33, IRC = 19, IRD = 22, IRE = 23; //IRD = Right side IR
//Declaring variable for IR
int valA = 0, valB = 0, valC = 0, valD = 0, valE = 0;

HCSR04 sonarA(24, 18); //Front Sonor - initialisation class HCSR04 (trig pin , echo pin)
HCSR04 sonarB(12, 21); //Right Sonor - initialisation class HCSR04 (trig pin , echo pin)
HCSR04 sonarC(2, 15); //Left Sonor - initialisation class HCSR04 (trig pin , echo pin)

//Using class "Motor" {methods = Forward, Backward, Stop, Speed, Status}
Motor motorA(27, 26, 14, 0);  // Right Motor - (inputpin1, inputpin2, enablepin, pwmChannel[0-18])
Motor motorB(4, 5, 13, 1);  // Left Motor - (in1, in2, en)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(IRA, INPUT);
  pinMode(IRB, INPUT);
  pinMode(IRC, INPUT);
  pinMode(IRD, INPUT);
  pinMode(IRE, INPUT);
  motorA.Speed(230);
  motorB.Speed(245);
}

void loop() {
  //ActivateSonar(); // reading sonar data
  ActivateIR(); // reading IR data
  CarMove();
  //FollowLine(motorA, motorB); // making decision
  //motorB.Speed(SB);
  //motorA.Speed(SA);
  //motorA.Forward();// Motor Forward(Speed);
  //motorB.Forward();

  delay(60);
}
// Reading all Sonar sensor and passing data to another function for processing
void ActivateSonar() {
  S1 = sonarA.dist();
  Serial.print("Sonor 1: ");
  Serial.println(S1);
  S2 = sonarB.dist();
  Serial.print("Sonor 2: ");
  Serial.println(S2);
  S3 = sonarC.dist();
  Serial.print("Sonor 3: ");
  Serial.println(S3);

  PutSonorData(S1, S2, S3); // sending data for processing
}

// Read all IR sensor and passing data to another function for processing
void ActivateIR() {
  valA = digitalRead(IRA); // IR Sensor output pin connected to D1
  valB = digitalRead(IRB); // IR Sensor output pin connected to D1
  valC = digitalRead(IRC); // IR Sensor output pin connected to D1
  valD = digitalRead(IRD); // IR Sensor output pin connected to D1
  valE = digitalRead(IRE); // IR Sensor output pin connected to D1
  Serial.print("IR:C=");
  Serial.print(valC); //right
  Serial.print(":A=");
  Serial.print(valA); //left
  Serial.print(":B=");
  Serial.print(valB);
  Serial.print(":D=");
  Serial.print(valD);
  Serial.print(":E=");
  Serial.println(valE);
  //PutIRData(valA, valB, valC, valD, valE);// Sending data for processing
}

void CarMove() {
  //when car on the straight line
  if (valC == 0 && valA == 1 && valB == 0) {
    motorB.Speed(245); motorA.Speed(230);
    motorA.Forward();// Motor Forward(Speed);
    motorB.Forward();
    //Serial.println("Forward");
  }

  //for smooth right turn
  else if (valC == 0 && valA == 1 && valB == 1) {
    //if 90 degree right turn
    if (valE == 0 && valD == 1) {
      motorB.Speed(255); motorA.Speed(245);
      motorA.Backward();// Motor Forward(Speed);
      motorB.Forward();
    }
    else {
      motorB.Speed(255); motorA.Speed(200);
      motorA.Forward();// Motor Forward(Speed);
      motorB.Forward();
    }
  }

  //for smooth left turn
  else if (valC == 1 && valA == 1 && valB == 0) {
    //if 90 degree left turn
    if (valE == 1 && valD == 0) {
      motorB.Speed(255); motorA.Speed(245);
      motorA.Forward();// Motor Forward(Speed);
      motorB.Backward();
    }

    else {
      motorB.Speed(200); motorA.Speed(255);
      motorA.Forward();// Motor Forward(Speed);
      motorB.Forward();
    }
  }

  // curvy right turn
  else if (valC == 0 && valA == 0 && valB == 1) {
    motorB.Speed(245);
    motorB.Forward();
    motorA.Stop();// Motor Forward(Speed);

  }

  //  curvy left turn
  else if (valC == 1 && valA == 0 && valB == 0) {
    motorA.Speed(230);
    motorA.Forward();// Motor Forward(Speed);
    motorB.Stop();

  }
  //when no sensor is detected
  else if (valC == 0 && valA == 0 && valB == 0 && valD == 0 && valE == 0) {
    //motorA.Speed(0); motorB.Speed(0);
    motorA.Stop();
    motorB.Stop();
    //Serial.println("Stop");
  }

  //when only left most sensor is detected
  else if (valC == 0 && valA == 0 && valB == 0 && valD == 0 && valE == 1) {
    motorA.Speed(245);
    motorA.Forward();
    motorB.Stop();
    //Serial.println("Stop");
  }

  //when only right most sensor is detected
  else if (valC == 0 && valA == 0 && valB == 0 && valD == 1 && valE == 0) {
    motorB.Speed(255);
    motorB.Forward();
    motorA.Stop();
    //Serial.println("Stop");
  }
}


