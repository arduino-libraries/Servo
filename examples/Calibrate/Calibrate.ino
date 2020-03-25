/* Calibrate
  This code allows you to calibrate your servo motor using the serial monitor
  Enter the degree in the serial monitor
  by Divyansh Khandelwal <github.com/noob-master147>
  This example code is in the public domain.

  modified 24th March 2020
  by Divyansh Khandelwal
*/

#include <Servo.h>
Servo myservo;     // create servo object to control a servo
int input;         // variable to read the value from the Serial Monitor
int servoPin = 11; // the Arduino pin the servo is attached to


void setup() {
  myservo.attach(servoPin); // attaches the servo on servoPin to the servo object
  Serial.begin(9600);
  Serial.println("Enter The Angle from 0 to 180");
}

void loop() {
  if (Serial.available() > 0) {
    input = Serial.parseInt();        // reads the value from Serial
    delay(25);
    while (Serial.available()) {      // remove the line endings
      Serial.read();
    }
    myservo.write(input);             // sets the servo position
    delay (25);
    Serial.print("Servo set to ");
    Serial.print(input);
    Serial.println(" degree");

  }
}
