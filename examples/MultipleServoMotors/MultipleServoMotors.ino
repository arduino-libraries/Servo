/*
  Example of attach() and detach()
  by Bhargav Patel <https://github.com/Engineer1999/>
  
              Circuit description 
 +-----------+--------+  +-----------+--------+                              
 |Servo_1    | Arduino|  |Servo_2    | Arduino|            
 |-----------+--------|  |-----------+--------|
 |Ground     |   GND  |  |Ground     |   GND  |
 |Power      |   5V   |  |Power      |   5V   |
 |Signal     |   D10  |  |Signal     |   D11  |
 +-----------+--------+  +-----------+--------+
*/
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int angle;      // variable to read the angle value of servo horn

void setup() {
  Serial.begin(9600);      // initializing serial communication with 9600 baud rate
  myservo.attach(10);      // attaches the servo on pin 10 to the servo object
  if (myservo.attached()) { // check whether servo motor is attached or not
    Serial.println("Servo motor attached to pin 10");
    myservo.write(0);      // setting angle of servo horn at 0 degree
    delay(1000);           // delay of 1 second
    myservo.write(90);     // setting angle of servo horn at 90 degree
    delay(1000);
    myservo.write(180);    // setting angle of servo horn at 180 degree
    delay(1000);
  } else {
    Serial.println("Servo motor attachment failed");
  }
  myservo.attach(11);       // attaches the servo on pin 11 to the servo object
  if (myservo.attached()) { // check whether servo motor is attached or not
    Serial.println("Servo motor attached to pin 11");
    myservo.write(0);
    delay(1000);
    myservo.write(90);
    delay(1000);
    myservo.write(180);
    delay(1000);
  } else {
    Serial.println("Servo motor attachment failed");
  }
  myservo.detach();         // detach the servo motor
  if (myservo.attached()) { // check whether servo motor is attached or not
    Serial.println("Servo motor is attached");
  } else {
    Serial.println("Servo motor was detached");
  }
}

void loop() {
}
