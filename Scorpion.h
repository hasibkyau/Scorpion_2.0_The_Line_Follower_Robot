#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>


class Motor {
  private:
    int in1;
    int in2;
    int enable1Pin;
    const int freq = 30000;
    const int pwmChannel = 0;
    const int resolution = 8;
    
  public:
    int dutyCycle = 200;
    Motor(int in1, int in2, int enable1Pin) {
      this -> in1 = in1;
      this -> in2 = in2;
      this -> enable1Pin = enable1Pin;
      init();
    };

    void init() {
      // sets the pins as outputs:
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(enable1Pin, OUTPUT);

      // configure LED PWM functionalitites
      ledcSetup(pwmChannel, freq, resolution);

      // attach the channel to the GPIO to be controlled
      ledcAttachPin(enable1Pin, pwmChannel);
      
      ledcWrite(pwmChannel, dutyCycle);
    }

    void Forward() {
      Serial.println("Forward");
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


//Data
int Sonor1, Sonor2, Sonor3, IR1, IR2, IR3, IR4, IR5;
void PutSonorData(int S1, int S2, int S3){
  Sonor1 = S1; Sonor2 = S2; Sonor3 = S3; 
}
#endif
