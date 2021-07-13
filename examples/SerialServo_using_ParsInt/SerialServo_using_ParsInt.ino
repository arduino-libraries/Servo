#include <Servo.h>
Servo servo1;
void setup()
{
  Serial.begin(9600);
  servo1.attach(9);
  servo1.write(0);
  Serial.println("Enter angle between 0 to 180");
}
void loop()
{
  if(Serial.available()>0)
  {
    int angle=Serial.parseInt();
    servo1.write(angle);
  }
}
