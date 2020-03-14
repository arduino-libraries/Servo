/*
  Servo Demo

  As Long as the button is pushed down the servo will move 
  in a counter-clockwise direction in a stepsize of one 
  degree till it reaches 180 degrees and then returns to 
  start from the zero position automatically.

  The circuit:
 * Servo GND pin to ground
 * Servo power pin to 5V
 * Servo signal pin to digital pin 9
 * Button:
 * Terminal 1b to ground
 * Terminal 2b to digital pin 2
 
 - A graphical representation of the circuit can be found in cotrolledMotion.png
 
  created 12 Mar 2020
  
  by Mohamed Lotfy
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int servoPos;  // variable to store the servo position
int buttonState = 0;  // variable to track the button state
int buttonPin = 2;  // button attached to input pin 2 of the board


void setup() {
  pinMode(buttonPin,HIGH);  // set the button pin in the board to be an input pin
  digitalWrite(buttonPin,HIGH);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
}

void loop() {
  buttonState = digitalRead(buttonPin);  // store the button state
  myservo.write(servoPos);  // set the servo position with the intended position
  if(buttonState == LOW) {  // increment the servo position angle if the button is pressed
    servoPos = (servoPos+1) % 180;  // set the servo position to zero if it exceeds 180 degrees
    delay(50);  // damp the button press effect 
  }  
}
