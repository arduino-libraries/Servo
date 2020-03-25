#include <Servo.h>
Servo myservo;     // create servo object to control a servo
int input;         // variable to read the value from the SerialMonitor 
int servoPin = 11; // set the Pin on Arduino, servo is attached


void setup() {
  myservo.attach(servoPin); // attaches the servo on servoPin to the servo object
  Serial.begin(9600);
}

void loop() {
if (Serial.available() > 0) {
    input = Serial.parseInt();        // reads the value from Serial
    myservo.write(input);             // sets the servo position 
    Serial.print("Servo set to ");
    Serial.print(input);
    Serial.println(" degree");
    delay (1000);
  }
}
