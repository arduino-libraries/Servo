/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo;  // create Servo object to control a servo
// up to six Servo objects can be created on CI13XX boards

#if defined(ARDUINO_ARCH_CI13XX)
#if defined(CI_CHIP_CI1302) || defined(CI_CHIP_CI1303)
const int servoPin = PA5;  // PWM3; avoids PC4 amplifier control
#else
const int servoPin = PB3;  // PWM4 on CI1306
#endif
#else
const int servoPin = 9;
#endif

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(servoPin);  // attaches the servo to the Servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}
