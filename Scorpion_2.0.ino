// Line follower Robot : Scorpion 2.0
// By : Md. Hasibur Rahman, KYAU

#include "Scorpion.h"
#include <HCSR04.h>
//int wrt = 555; // wrt = whell rotation time. time for rotating two time in mls
int wrt = 500; // for super fast mood
int DutyCycle = 0, min_speed = 200, med_speed = 205, high_speed = 210, max_speed = 255;
int FrontWall = 20, RightWall = 100, LeftWall = 100, RoadWidth = 100, SideSpace = 20; //Declaring Sonar sensor variable
int IRA = 19, IRB = 18, IRC = 5, IRD = 17, IRE = 16, IRF = 21; //IR variable for declaring GPIO Pin
int A = 0, B = 0, C = 0, D = 0, E = 0, F = 1, AIR; //IR variable for store value
int dt = 1; // default turn (1 = right, 0   = left).

int TOUCH_PIN = 4, BLUE_LED = 22, BUZZER = 15;

HCSR04 SonarR(13, 12); //Right Sonor - initialisation class HCSR04 (trig pin , echo pin)
HCSR04 SonarL(2, 0); //Left Sonor - initialisation class HCSR04 (trig pin , echo pin)

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
  pinMode(IRF, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);
  Beep(3, 150);

  MotorR.Release();
  MotorL.Release();
  delay(100);
  MotorR.Forward();
  MotorL.Forward();
}

//*** Default turn
void DefaultTurn() {
  (dt == 1) ? _90dRight() : _90dLeft();
}



void loop() {
  /**
   *get touch
  TouchSpeed(){
  if(SuparFast == true){wrt = 500;}
  else{wrt = 555;}
  TouchDefaultTurn{toogle default turn};
  ***/

  ReadSonar(); // reading sonar data
  ReadIR(); // reading IR data

  (F == 1) ? FollowTrack() : AvoidObstacle();
}

//*** Straight Forward
void Straight() {
  MotorR.Speed(max_speed);
  MotorL.Speed(max_speed);
}

//*** Car speed 0 with with forward gear
void Brake() {
  MotorR.Speed(0);
  MotorL.Speed(0);
}

//*** Car stop and neutral
void Neutral() {
  MotorR.Release();
  MotorL.Release();
}

//*** Smooth Left Turn
void SmoothLeft() {
  MotorL.Speed(high_speed);
  MotorR.Speed(max_speed);
}

//*** Medium Left Turn
void MedLeft() {
  MotorL.Speed(med_speed);
  MotorR.Speed(max_speed);
}

//*** Sharp Left Turn
void SharpLeft() {
  MotorL.Speed(min_speed);
  MotorR.Speed(max_speed);
}

void HardLeft() {

  MotorL.Speed(0);
  MotorR.Speed(max_speed);
}


void HardRight() {
  MotorL.Speed(max_speed);
  MotorR.Speed(0);
}

//*** Smooth Right Turn
void SmoothRight() {
  MotorR.Speed(high_speed);
  MotorL.Speed(max_speed);
}

//*** Medium Right Turn
void MedRight() {
  MotorR.Speed(med_speed);
  MotorL.Speed(max_speed);
}

//*** Sharp Right Turn
void SharpRight() {
  MotorR.Speed(min_speed);
  MotorL.Speed(max_speed);
}

//*** 90d left turn
void _90dLeft() {
  Straight();
  delay(wrt);
  Neutral(); delay(10);
  MotorR.Forward(); MotorL.Backward();
  MotorL.Speed(max_speed); MotorR.Speed(max_speed);
  do {
    ReadIR();
  }
  while (!(AIR == 4 && C == 0));
  Neutral(); delay(10);
  MotorL.Forward(); MotorR.Forward();
}

//*** 90d Right Turn
void _90dRight() {
  Straight();
  delay(wrt);
  Neutral(); delay(10);
  MotorL.Forward(); MotorR.Backward();
  MotorL.Speed(max_speed); MotorR.Speed(max_speed);
  //MotorL.Speed(max_speed); MotorR.Speed(0);
  do {
    ReadIR();
  }
  while (!(AIR == 4 && C == 0));
  Neutral(); delay(10);
  MotorL.Forward(); MotorR.Forward();
}

//*** 180d turn on place
void _180dturn() {
  //Serial.println("Taking U turn"); delay(2000);
  Neutral(); // Both motor stop with neutral gear
  delay(68);
  MotorL.Forward(); MotorR.Backward();// Rotate on place
  MotorR.Speed(max_speed); MotorL.Speed(max_speed);
  do {
    ReadIR();
    AIR = A + B + C + D + E;
  }
  while (!(AIR == 4 && C == 0)); // finish 180d ?
  Brake(); // Both motor stop with neutral gear
  Neutral();
  delay(68);
  MotorL.Forward(); MotorR.Forward();
}

//*** Move straight forward in the walls by middle
void PassThroughWalls() {
  ReadSonar();
  RoadWidth = LeftWall + RightWall; // total side gap
  SideSpace = RoadWidth / 2 - 2; // average side gap for each side

  if (RoadWidth <= 120) { //when the sensor can count distance. Go by the middle of path
    if (LeftWall <= SideSpace) { // car is not in middle of the.
      MedRight();
    }
    else if (RightWall <= SideSpace) { // car is not in middle of the walls
      MedLeft();
    }
    else { // car is now in middle of the walls
      Straight();
    }
  }
}

//***Avoid obstacle if found
//for default turn == left
void AvoidObstacle() {
  int max_spd, min_spd;
  if (dt == 1){
    max_spd = 0;
    min_spd = 255;
  }
  else {
    max_spd = 255;
    min_spd = 0;
  }
  Brake();
  Serial.println("Obstacle found!"); delay(500);
  MotorL.Speed(min_spd); MotorR.Speed(max_spd);// Right turn
  delay(wrt * 3);
  MotorR.Speed(min_spd); MotorL.Speed(max_spd);// Left turn
  delay(wrt * 3);
  MotorR.Speed(255); MotorL.Speed(255);// Straight forward
  delay(wrt * 3);
  MotorR.Speed(min_spd); MotorL.Speed(max_spd);// Right turn
  delay(wrt * 3);
  do {
    MotorR.Speed(255); MotorL.Speed(255);// Straight
    ReadIR();
  }
  while (AIR == 5);
  //Brake();
  Serial.println("Obstacle skiped!"); delay(500);
}

int Beep(int n, int dly) {
  int i = 0;
  while (i < n) {
    digitalWrite(BUZZER, HIGH);
    delay(dly);
    digitalWrite(BUZZER, LOW);
    delay(dly);
    i++;
  }
}

//Follow track
void FollowTrack() {

  if (AIR == 4)//On track
  {
    //(A == 0) ? _90dLeft() : (B == 0) ? MedLeft() : (C == 0 ) ? Straight() : ( D == 0 ) ? MedRight() : _90dRight();
    (A == 0) ? HardLeft() : (B == 0) ? MedLeft() : (C == 0 ) ? Straight() : ( D == 0 ) ? MedRight() : HardRight();
  }
  else if (AIR == 3) //
  {
    if (B == 0) {
      //(A == 0) ? SharpLeft() : SmoothLeft();//smooth left for C = 0 and B = 0
      (C == 0) ? SmoothLeft() : SharpLeft();
    }
    else if (D == 0) {
      (C == 0) ? SmoothRight() : SharpRight();
    }

  }
  else if (AIR == 2 || AIR == 1) {
    int temp = A;
    delay(wrt / 8); ReadIR(); //go 1 cm and read the track
    (AIR == 0) ? DefaultTurn() : ((temp == 1) ? _90dRight() : _90dLeft());
  }
  else if (AIR == 0)//multiple line
  {
    DefaultTurn();
  }
  else if (AIR == 5)// White space
  {
    //Serial.println("white space!");delay(500);
    Straight(); //go 14cm forward
    delay(wrt);
    Brake(); // Speed 0 with forward gear

    ReadIR();
    //ReadSonar();

    if (AIR < 5) {
      //Serial.println("Track Found"); delay(500);
      Straight(); // if found track. It was a blank track.
    }
    else if (AIR == 5) // if no track
    {
      //Serial.println("No Track Found! Wall checking"); delay(500);
      if (LeftWall <= 50 && RightWall <= 50) // if no track & found walls
      {
        Serial.println("Wall found!"); delay(500);
        // follow walls until the track is founded
        while (AIR == 5){
          ReadIR();
          PassThroughWalls();
        }
        Serial.println("Released from wall!"); delay(500);
      }
      else if (LeftWall >= 100 && RightWall >= 100) {
        //Serial.println("No track and no wall found!"); delay(1000);
        _180dturn(); //if no track and no Side Walls. The track ends here.
      }
    }
  }
}


//*** Reading all Sonar sensor
void ReadSonar() {
  RightWall = SonarR.dist();
  LeftWall = SonarL.dist();
  Serial.print(" :RightWall= ");
  Serial.print(RightWall);
  Serial.print(" :LeftWall=");
  Serial.println(LeftWall);
}

//*** Read all IR sensor
void ReadIR() {
  A = digitalRead(IRA); // 0 = black, 1 = white
  B = digitalRead(IRB); // 0 = black, 1 = white
  C = digitalRead(IRC); // 0 = black, 1 = white
  D = digitalRead(IRD); // 0 = black, 1 = white
  E = digitalRead(IRE); // 0 = black, 1 = white
  F = digitalRead(IRF); // 0 = black, 1 = white
  AIR = A + B + C + D + E;

  Serial.println(" ");
  Serial.print(":A=");
  Serial.print(A);
  Serial.print(":B=");
  Serial.print(B);
  Serial.print(":C=");
  Serial.print(C);
  Serial.print(":D=");
  Serial.print(D);
  Serial.print(":E=");
  Serial.print(E);
  Serial.print(":F=");
  Serial.print(F);
  Serial.print(":AIR=");
  Serial.print(AIR);
}





