/*
 Controlling a servo position using the Serial Monitor
 by Mohamed Sameh

 This example code is in the public domain.
*/

#include <Servo.h>

Servo myservo;  // create Servo object to control a servo

void setup() {
  Serial.begin(9600); // initialize serial communication
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object

  Serial.println("Servo is ready.");
  Serial.println("Please enter an angle between 0 and 180:");
}

void loop() {
  // if there's any serial available, read it:
  if (Serial.available() > 0) {
    char inChar = Serial.peek(); // look at the next character in the serial buffer

    // if the character is a digit, parse the integer
    if (isDigit(inChar)) {
      int angle = Serial.parseInt();

      // constrain the angle to the valid range (0-180)
      if (angle >= 0 && angle <= 180) {
        myservo.write(angle);              // tell servo to go to position
        Serial.print("Moving to ");
        Serial.print(angle);
        Serial.println(" degrees.");
        delay(15);                         // waits for the servo to reach the position
      } else {
        Serial.println("Invalid input. Please enter a value between 0 and 180.");
      }
    } else {
      // if it's not a digit, discard it (e.g., newline characters or spaces)
      Serial.read();
    }
  }
}
