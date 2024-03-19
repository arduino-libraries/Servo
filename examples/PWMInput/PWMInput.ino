/*
  PWM input example for servo motor.
  by Bhargav Patel <https://github.com/Engineer1999/>
 
  Circuit description 
 +-----------+--------+
 |Servo      | Arduino|
 |-----------+--------|
 |Ground     |   GND  |
 |Power      |   5V   |
 |Signal     |   D10  |
 +-----------+--------+

 Mapping of pulse duration(width) to angle
 +-----------------------------+-----------+
 |Pulse duration(micro second) |   Angle   |
 +-----------------------------+-----------+
 |         500                 |    0      |
 |         1010                |    45     |
 |         1480                |    90     |
 |         2400                |   180     |
 +-----------------------------+-----------+          
*/       

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int angle;     // variable to read the value of the angle

void setup(){
  Serial.begin(9600);              // initializing serial communication with 9600 baud rate
  myservo.attach(10);              // attaches the servo on pin 10 to the servo object
}
void loop(){
  myservo.writeMicroseconds(500);  // setting PWM to 500
  angle = myservo.read();           // read the value of the current angle of servo horn
  Serial.print("Angle at PWM value 500: ");
  Serial.println(angle);            // print angle of servo horn on serial monitor
  delay(2000);                      // delay of 1 second
  
  myservo.writeMicroseconds(1010);  // setting PWM to 1010
  angle = myservo.read();
  Serial.print("Angle at PWM value 1010: ");
  Serial.println(angle);
  delay(1000);

  myservo.writeMicroseconds(1480);  // setting PWM to 1480  
  angle = myservo.read();
  Serial.print("Angle at PWM value 1480: ");
  Serial.println(angle);
  delay(1000);

  myservo.writeMicroseconds(2400);  // setting PWM to 2400
  angle = myservo.read();
  Serial.print("Angle at PWM value 2400: ");
  Serial.println(angle);
  delay(1000);
}
