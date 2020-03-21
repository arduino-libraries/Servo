/*
 Controlling a servo position using serial input
 by Hamza Khalid https://github.com/hmzakhalid
*/

#include <Servo.h>

Servo myservo;  // create Servo object to control a servo motor

long pos = 0;    // initial position of the shaft (max is 180)

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  Serial.begin(9600);  //opens serial port, sets data rate to 9600 bps
  Serial.println("Enter Position from 1 to 180"); 
}

void loop() {
  if(Serial.available() > 0){        // checks if there is any input on the serial monitor
    pos = Serial.parseInt();         // Reading the input from the serial monitor and storing it in'pos'
    myservo.write(pos);              // tell servo to go to that position 
  }
}
