#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

class Motor {
  private:
    int in1;
    int in2;
    int enable1Pin;
    int pwmChannel;
    int dutyCycle = 255;
    const int freq = 30000;
    const int resolution = 8;
    
  public:
    Motor(int in1, int in2, int enable1Pin, int pwm) {
      this -> in1 = in1;
      this -> in2 = in2;
      this -> enable1Pin = enable1Pin;
      this -> pwmChannel = pwm;
      init();
    };

    void init() {
      // sets the pins as outputs:
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(enable1Pin, OUTPUT);

      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);

      // configure LED PWM functionalitites
      ledcSetup(pwmChannel, freq, resolution);

      // attach the channel to the GPIO to be controlled
      ledcAttachPin(enable1Pin, pwmChannel);
      
      ledcWrite(pwmChannel, dutyCycle);
    }

    void Forward() {
      Serial.println("Forward");
      //Speed(spd);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    };

    void Backward() {
      Serial.println("Backward");
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    };

    void Stop() {
      Serial.println("Stop");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    };

    void Status(){
      Serial.println("Pin used : ");
      Serial.println(in1);
      Serial.println(in2);
      Serial.println(enable1Pin);
    }
    void Speed(int spd){
      this -> dutyCycle = spd;
      ledcWrite(pwmChannel, dutyCycle);
      Serial.println("Current Speed : ");
      Serial.println(dutyCycle);
      }
};


//All Sensor Data declaration
int Sonor1, Sonor2, Sonor3, IR1, IR2, IR3, IR4, IR5;
//Getting sonor data
void PutSonorData(int S1, int S2, int S3){
  Sonor1 = S1; Sonor2 = S2; Sonor3 = S3; 
}
//Getting IR data
void PutIRData(int IRA,  int IRB, int IRC, int IRD, int IRE){
    IR1 = IRA; IR2 = IRB; IR3 = IRC; IR4 = IRD; IRE = IR5;
}

void FollowLine(Motor A, Motor B){
  if(IR1 == 1 && IR2 == 1){
    A.Speed(240); B.Speed(250);
    A.Forward();// Motor Forward(Speed);
    B.Forward();
    Serial.println("Forward");
  }
  else if(IR1 == 1 && IR2 == 0){
    A.Speed(200); B.Speed(255);
        A.Forward();// Motor Forward(Speed);
        B.Forward();
  }
    else if(IR1 == 0 && IR2 == 1){
    A.Speed(240); B.Speed(200);
        A.Forward();// Motor Forward(Speed);
    B.Forward();
  }
  else{
    A.Stop();
    B.Stop();
    Serial.println("Stop");
    }
   
}
#endif
