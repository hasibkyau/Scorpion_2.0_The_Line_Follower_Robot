#include "Scorpion.h"

//Using class "Motor" {methods = Forward, Backward, Stop, Speed, Status}
Motor motorA(27, 26, 14);  //(in1, in2, en)
Motor motorB(4, 5, 13);  //(in1, in2, en)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  
  Serial.println("After modification :");
  motorA.Forward();
  delay(2000);

  motorB.Speed(255);
  delay(2000);
}

void loop() {

}
