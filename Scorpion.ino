#include "Scorpion.h"
#include <HCSR04.h>

//Declaring Sonar sensor variable
int S1, S2, S3;
//Declaring digital pin for IR sensor
int IRA = 32, IRB = 33, IRC = 25, IRD = 34, IRE = 19; //IRD = Right side IR
//Declaring variable for IR
int valA = 0, valB = 0, valC = 0, valD = 0, valE = 0;

HCSR04 sonarA(22, 23); //Front Sonor - initialisation class HCSR04 (trig pin , echo pin)
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
}

void loop() {
  //ActivateSonar(); // reading sonar data
  ActivateIR(); // reading IR data
  //FollowLine(motorA, motorB); // making decision
    //motorA.Forward();
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
void ActivateIR(){
  valA = digitalRead(IRA); // IR Sensor output pin connected to D1
  valB = digitalRead(IRB); // IR Sensor output pin connected to D1
  valC = digitalRead(IRC); // IR Sensor output pin connected to D1
  valD = digitalRead(IRD); // IR Sensor output pin connected to D1
  valE = digitalRead(IRE); // IR Sensor output pin connected to D1
  Serial.print("valA :");
  Serial.println(valA); //right
  Serial.print("valB :");
  Serial.println(valB); //left
  Serial.print("valC :");
  Serial.println(valC);
  Serial.print("valD :");
  Serial.println(valD);
  Serial.print("valE :");
  Serial.println(valE);
  PutIRData(valA, valB, valC, valD, valE);// Sending data for processing
}


