/* PushRotate
  by K.Abhijeet
  This example code rotates the servo motor clockwise or anti-clockwise as per button pressed.

  Circuit:-
    Arduino Uno
    Micro Servo Motor connected at pin 9
    push button1 between pin 6 and VCC with 220 Ohm pull-down resistor(rotate anti-clockwise)
    push button2 between pin 7 and VCC with 220 Ohm pull-down resistor(rotate clockwise)

*/

#include <Servo.h>

Servo Smotor;                                       // Servo motor object
int pos = 0;
int angle = 5;                                      //angle to rotate
void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT);                                //Input button1
  pinMode(7, INPUT);                                //Input button2
  Smotor.attach(9);                                 //Servo Motor connected at pin 9
  Smotor.write(90);
}

void loop() {

  if (digitalRead(6) == HIGH)
  {
    pos = pos + angle;
    Smotor.write(pos);                                   // rotates servo motor anti-clockwise
  }

  if (digitalRead(7) == HIGH)
  {
    pos = pos - angle;
    Smotor.write(pos);                                  //rotates servo motor clockwise
  }
  delay(100);

  Serial.print("Current Motor position is:");
  Serial.println(pos);                                  //prints motor angle
}
