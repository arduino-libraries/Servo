/*
Controling multiple servo motors using attach() and detach()
by Bhargav Patel <https://github.com/Engineer1999/>

*/

#include <Servo.h> 

Servo myservo;  // create servo object to control a servo

int angle;  // variable to read the angle value of servo motor

void setup() 
{ 

  
  Serial.begin(9600);      // initializing serial communication with 9600 baud rate
  
  myservo.attach(10);      // attaches the servo on pin 10 to the servo object
  if (myservo.attached()){ // cheack whether servo motor is attached or not
  Serial.println("Servo motor attached to pin 10");
  
  myservo.write(0);      // setting angle of servo motor at 0 degree
  delay(1000);           // delay of 1 second
  myservo.write(90);     // setting angle of servo motor at 90 degree
  delay(1000);  
  myservo.write(180);    // setting angle of servo motor at 90 degree
  delay(1000);
  } else{
    Serial.println("Servo motor attachement failed");
  }

  
  myservo.attach(12);      // attaches the servo on pin 12 to the servo object
  if (myservo.attached()){ // cheack whether servo motor is attached or not
  Serial.println("Servo motor attached to pin 12");
  
  myservo.write(0);
  delay(1000);
  myservo.write(90);
  delay(1000);  
  myservo.write(180);
  delay(1000);
  } else{
    Serial.println("Servo motor attachement failed");
  }
  
  myservo.detach();        // detach the servo motor
  if (myservo.attached()){ // cheack whether servo motor is attached or not
    Serial.println("Servo motor is still attached");
  } else{
    Serial.println("Servo motor is still detached");
  }
  
} 

void loop() {
} 
