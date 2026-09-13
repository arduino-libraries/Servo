/*
  Using a servo as a car Wiper, activated by switch.

  The circuit:
  - switch is attached from pin 2 to ground
  - Servo's signal pin is attached to Pin 9

  modified on 21 March 2020
  by Durgesh Pachghare
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int button_pin = 2;  // digital pin used to connect the button input
int val;    // variable to read the value from the analog pin
int startpos = 10;      //angle from which the servo wiper starts
int endpos = 145;       //angle till the servo wiper rotates

void setup()
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(button_pin, INPUT_PULLUP);   //declares the pin as digital input
  myservo.write(startpos);      // Initialize the servo wiper to start angle
  pinMode(LED_BUILTIN, OUTPUT); // to show the state of servo wiper
}

void loop()
{
  // The button_pin is declared as INPUT_PULLUP which means switch logic is inverted.
  // It gives LOW when the switch is pressed and HIGH when it is open
  if (digitalRead(button_pin) == LOW)  // Start the servo if the button is pressed and complete the
                                       // entire rotation regardless of button state later
  {
    digitalWrite(LED_BUILTIN, HIGH);   // status that wiper servo is activated
    for (int pos = startpos; pos <= endpos; pos += 1)  // goes from starting angle to end angle in steps of 1 degree
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits for the servo to reach the position
    }
    for (int pos = endpos; pos >= startpos; pos -= 1) // goes from end angle to start angle again
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits for the servo to reach the position
    }
  }
  digitalWrite(LED_BUILTIN, LOW);  //status that wiper servo has stopped
}
