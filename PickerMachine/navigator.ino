#include <Servo.h>
Servo servoUpDown;  
const int enPin=8;
const int stepXPin = 2; //X.STEP
const int dirXPin = 5; // X.DIR
const int stepYPin = 3; //Y.STEP
const int dirYPin = 6; // Y.DIR
const int pinUpDown = 11;   

int stepPin=stepYPin;
int dirPin=dirYPin;
int x_cur=0;
int y_cur=0;
const int stepsPerRev=200;
int pulseWidthMicros = 70; 	// microseconds
int millisBtwnSteps = 70;
void setup() {
 	Serial.begin(9600);
 	pinMode(enPin, OUTPUT);
 	digitalWrite(enPin, LOW);
 	pinMode(stepPin, OUTPUT);
 	pinMode(dirPin, OUTPUT);
  servoUpDown.attach(11);
}
void loop() {
  for(int i=0;i<3;i++)
  {
    int x_read = Serial.parseInt();
    int y_read = Serial.parseInt();
    int x=0;
    int y=0;
  //calculate the relative distance from the previous point
  //the picker won't move if there is no input, whose value is by default 0 in the arduino uno case
 	  if(x_read!=0||y_read!=0)
    {
      x=x_read-x_cur;
      y=y_read-y_cur;
      x_cur=x_read;
      y_cur=y_read;
    }
    moveX(x);
    moveY(y);
    dip(i);//1-取枪头、点样
  }
  int x_read = Serial.parseInt();
  int y_read = Serial.parseInt();
  int x=0;
  int y=0;
  //calculate the relative distance from the previous point
  //the picker won't move if there is no input, whose value is by default 0 in the arduino uno case
 	if(x_read!=0||y_read!=0)
  {
    x=x_read-x_cur;
    y=y_read-y_cur;
    x_cur=x_read;
    y_cur=y_read;
  }
  moveX(x);
  moveY(y);
  // Serial.println(x_cur);
  // Serial.println(y_cur);
  dip(3);//2-退枪头
  Serial.println("round complete");
  delay(2000);
}

void moveY(int y)
{
  if(y>0)
  {
    digitalWrite(dirPin, HIGH); 
    digitalWrite(dirXPin, LOW);
  }
  if(y<0)
  {
    digitalWrite(dirPin, LOW); 
    digitalWrite(dirXPin, HIGH);
  }
 	for (int i = 0; i < abs(y)*stepsPerRev; i++) {
 			digitalWrite(stepPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
      digitalWrite(stepXPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepXPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
 	delay(1000);
}

void moveX(int x)
{
  if(x>0)
  {
    digitalWrite(dirPin, HIGH); 
    digitalWrite(dirXPin, HIGH);
  }
  if(x<0)
  {
    digitalWrite(dirPin, LOW); 
    digitalWrite(dirXPin, LOW);
  }
 	for (int i = 0; i < abs(x)*stepsPerRev; i++) {
 			digitalWrite(stepPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
      digitalWrite(stepXPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepXPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
 	delay(1000);
}
//caution: the code bellow has not been tested yet
void dip(int mode)
{
  if(mode==0)
  {
    servoUpDown.write(60);
    delay(1000);
    servoUpDown.write(90);
    delay(1000);
    servoUpDown.write(120);
    delay(1000); 
    servoUpDown.write(90);
    delay(1000);
  }
  if(mode==1)
  {
    servoUpDown.write(60);
    delay(400);
    servoUpDown.write(90);
    delay(1000);
    servoUpDown.write(120);
    delay(600); 
    servoUpDown.write(90);
    delay(1000);
  }
  if(mode==2)
  {
    servoUpDown.write(60);
    delay(400);
    servoUpDown.write(90);
    delay(1000);
    servoUpDown.write(120);
    delay(600); 
    servoUpDown.write(90);
    delay(1000);
  }
  else if(mode==3)
  {
    servoUpDown.write(120);
    delay(700);
    servoUpDown.write(90);
    delay(1000);
    servoUpDown.write(60);
    delay(600); 
    servoUpDown.write(90);
    delay(1000);
  }
}
