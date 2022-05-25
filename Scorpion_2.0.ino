#include "Scorpion.h"
#include <HCSR04.h>

//Declaring Sonar sensor variable
int S1, S2, S3, SA = 230, SB = 255;
//Declaring digital pin for IR sensor
int IRA = 32, IRB = 33, IRC = 19, IRD = 22, IRE = 23; //IRD = Right side IR
//Declaring variable for IR
int valA = 0, valB = 0, valC = 0, valD = 0, valE = 0;
bool obstacle = false;

HCSR04 sonarA(17, 18); //Front Sonor - initialisation class HCSR04 (trig pin , echo pin)
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
  ActivateSonar(); // reading sonar data
  ActivateIR(); // reading IR data
  
  //FollowObject();
  //FollowLine();
  MakeDecision();
  delay(60);
}
// Reading all Sonar sensor and passing data to another function for processing
void ActivateSonar() {
  S1 = sonarA.dist();
  S2 = sonarB.dist();
  S3 = sonarC.dist();

  Serial.print("S1=");
  Serial.print(S1);

  Serial.print(" :S2= ");
  Serial.print(S2);

  Serial.print(" :S3=");
  Serial.println(S3);

  //PutSonorData(S1, S2, S3); // sending data for processing
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

void MakeDecision(){
  if (S1 <= 5) {
    SkipObject();
  }
  else if (valA == 0 && valB == 0 && valC == 0 && valD == 0 && valE == 0) {
    if (S2 <= 30 && S2 <= 30) {
      //follow object
      FollowObject();
    }
    else {
      //follow line
      motorA.Speed(240); motorA.Speed(255);
      motorA.Forward(); motorB.Forward();
    }
  }
  else{
    FollowLine();
  }
}

void FollowLine() {
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
      motorB.Speed(255);
      //motorA.Speed(245);
      motorA.Stop();// Motor Forward(Speed);
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
      //motorB.Speed(255);
      motorA.Speed(245);
      motorA.Forward();// Motor Forward(Speed);
      motorB.Stop();
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

void SkipObject() {
  motorA.Speed(240); motorB.Speed(255);

  //turn right
  delay(500);
  motorB.Forward();
  delay(1400);

  //go Forward for 2 sec
  motorA.Forward();
  motorB.Forward();
  delay(500);

  //turn left
  motorB.Stop();
  motorA.Forward();
  delay(900);

  //go Forward straigt for 2 sec
  motorA.Forward();
  motorB.Forward();
  delay(1000);

  //turn left
  motorB.Stop();
  motorA.Forward();
  delay(1200);

  //forward
  motorA.Forward();
  motorB.Forward();
  delay(500);

  //right
  motorA.Stop();
  motorB.Forward();
  delay(1400);
  motorB.Stop();

  //  while (valA == 0 && valB == 0 & valC && 1 && valD == 1 && valE == 1) {
  //    motorA.Stop();
  //    motorB.Stop();
  //    delay(500);
  //    motorA.Forward();
  //    motorB.Forward();
  //  }
  obstacle = false;
  delay(500);
}

void FollowObject() {
  int totalDist = (S2 + S3) - 4;
  int Dist = (totalDist / 2) - 2;
  motorA.Speed(240); motorB.Speed(255);
  
  if (S2 <= Dist) {
    motorA.Forward();
    motorB.Stop();
  }
  else if (S3 <= Dist) {
    motorA.Stop();
    motorB.Forward();
  }
  else {
    motorA.Forward();
    motorB.Forward();
  }
}

