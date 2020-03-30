#include <Servo.h>
Servo servo1;
String input ="";
void setup()
{
  Serial.begin(9600);
  servo1.attach(9);
  servo1.write(0);
  Serial.println("Enter angle between 0 to 180");
}
void loop()
{
  //servo1.write(0);
  while(Serial.available()>0)
  {
    int inchar = Serial.read();
    if(isDigit(inchar))
    {
      input+=(char)inchar;
    }
    if(inchar=='\n'){
      servo1.write(input.toInt());
      delay(5000);
      servo1.write(0);
      input="";
    }
    }
}
