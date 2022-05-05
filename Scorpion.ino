#include "Scorpion.h"
#include <HCSR04.h>

HCSR04 sonarA(22, 23); //Front Sonor - initialisation class HCSR04 (trig pin , echo pin)
HCSR04 sonarB(12, 21); //Right Sonor - initialisation class HCSR04 (trig pin , echo pin)
HCSR04 sonarC(2, 15); //Left Sonor - initialisation class HCSR04 (trig pin , echo pin)

//Using class "Motor" {methods = Forward, Backward, Stop, Speed, Status}
Motor motorA(27, 26, 14);  // Right Motor - (in1, in2, en)
Motor motorB(4, 5, 13);  // Left Motor - (in1, in2, en)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //motorA.Forward();
}

void loop() {
  ActivateSonar();
  delay(60);
}

void ActivateSonar(){
  int S1 = sonarA.dist();
  Serial.print("Sonor 1: ");
  Serial.println(S1);
  int S2 = sonarB.dist();
  Serial.print("Sonor 2: ");
  Serial.println(S2);
  int S3 = sonarC.dist();
  Serial.print("Sonor 3: ");
  Serial.println(S3);
  
  PutSonorData(S1, S2, S3);
}


