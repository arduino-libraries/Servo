/*
 Controlling a servo position using serial input
 by Hamza Khalid https://github.com/hmzakhalid

*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo


int pos = 0;    // initial Position of the Shaft (max is 180)

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.println("Enter Position from 1 to 180"); 
}


void loop() {
  pos = Serial.read();               // Reading the input from the serial monitor and storing it in'pos'
  if(pos > 0){                       // checks if there is any input
    myservo.write(pos);              // tell servo to go to that position 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
