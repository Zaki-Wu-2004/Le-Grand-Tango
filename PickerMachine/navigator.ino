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
int x_read=0;
int y_read=0;
int x=0;
int y=0;
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
  Serial.println("Arduino is ready"); // Print a message when ready
}
void loop() {
  if (Serial.available() > 0) { // Check if data is available to read
    String data = Serial.readString(); // Read the incoming data
    Serial.println("Received: " + data); // Print the received data
    
    // Parse coordinate data
    int index = 0;
    int i=0;
    while ((index = data.indexOf(';')) != -1) {
      String coordinate = data.substring(0, index);
      data = data.substring(index + 1);
      Serial.println("moving towards: (" + coordinate + ")"); // Process single coordinate
      Serial.println(i);
      int commaIndex = coordinate.indexOf(',');
      if (commaIndex != -1) {
        String xStr = coordinate.substring(0, commaIndex);
        String yStr = coordinate.substring(commaIndex + 1);
        
        x_read = xStr.toInt(); // Convert x coordinate to integer
        y_read = yStr.toInt(); // Convert y coordinate to integer
        
      } else {
        Serial.println("Invalid coordinate format");
      }
      x=x_read-x_cur;
      y=y_read-y_cur;
      x_cur=x_read;
      y_cur=y_read;
      moveX(x);
      moveY(y);
      dip(i);
      i=i+1;
    }
    i=0;
    if (data.length() > 0) { // Process the last coordinate
      Serial.println("Coordinate: " + data);
      String coordinate = data;
      int commaIndex = coordinate.indexOf(',');
      if (commaIndex != -1) {
        String xStr = coordinate.substring(0, commaIndex);
        String yStr = coordinate.substring(commaIndex + 1);
        
        x_read = xStr.toInt(); // Convert x coordinate to integer
        y_read = yStr.toInt(); // Convert y coordinate to integer
        
      } else {
        Serial.println("Invalid coordinate format");
      }
      x=x_read-x_cur;
      y=y_read-y_cur;
      x_cur=x_read;
      y_cur=y_read;
      moveX(x);
      moveY(y);
      dip(3);
    }
    Serial.println("round complete");
    delay(2000);
  }
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
    delay(300);
    servoUpDown.write(90);
    delay(1000);
    servoUpDown.write(120);
    delay(500); 
    servoUpDown.write(90);
    delay(1000);
  }
  if(mode==2)
  {
    servoUpDown.write(60);
    delay(300);
    servoUpDown.write(90);
    delay(1000);
    servoUpDown.write(120);
    delay(500); 
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
