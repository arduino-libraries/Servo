/* Read
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 18 Mar 2020
 by S Jeeva
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int currPos;    // variable to read the current position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); // Begin the Serial at 9600 baud rate
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    currPos = myservo.read();        // Read the angle of position
    Serial.println(currPos);         // Print the angle of position in serial monitor
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    currPos = myservo.read();        // Read the angle of position
    Serial.println(currPos);         // Print the angle of position in serial monitor
  }
}

