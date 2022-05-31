#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

class Motor {
  private:
    int in1;
    int in2;
    int enable1Pin;
    int pwmChannel;
    int dutyCycle = 0;
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
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    };

    void Backward() {
      //Serial.println("Backward");
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    };

    void Release() {
      //Serial.println("Stop");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    };

    void Status() {
      Serial.println("Pin used : ");
      Serial.println(in1);
      Serial.println(in2);
      Serial.println(enable1Pin);
    }
    void Speed(int dutyCycle ) {
      ledcWrite(pwmChannel, dutyCycle );
    }
};


#endif
