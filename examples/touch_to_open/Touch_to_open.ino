#include <Servo.h>         //including the servo library

Servo touch_servo;       // initialize a sevo object to use it
int sensor_pin =2;      // defining a pin for the touch sensor  

void setup() {
  pinMode(2, INPUT);
  touch_servo.attach(9);       // defining the pin of the servo motor(pin9)
  Serial.begin(9600);         // defining the Baud Rate to use the serial Monitor
}

void loop() {
  
  if (digitalRead(2) == HIGH)      // checking if the sensor is touched 
  {
  touch_servo.write(90);         // moving the servo 90 degree 
  Serial.println("Sensor is touched | servo is opened");
  }
  

}
