#include <Servo.h>
Servo servo1;     // 2 Servos Initialized
Servo servo2;
int x_Key = A1;     //Horizontal pin for Joystick
int y_Key = A0;     //Vertical pin for Joystick
int x_Pos;
int y_Pos;
int servo1_Pin = 8;   //Signal Pins for Servo
int servo2_Pin = 9;
int initial_Position1 = 90;    //Initially Servos are moved to 90
int initial_Position2 = 90;

void setup() {
  Serial.begin (9600) ;
  servo1.attach (servo1_Pin) ;
  servo2.attach (servo2_Pin) ;
  servo1.write (initial_Position1);
  servo2.write (initial_Position2);
  pinMode (x_Key, INPUT) ;
  pinMode (y_Key, INPUT) ;
}

void loop() {
  x_Pos = analogRead(x_Key) ;      //read the values for the horizontal and the vertical position from the joystick module
  y_Pos = analogRead(y_Key) ;

  if (x_Pos < 300) {      //if the value for the horizontal position is less than 300, then the first servo will move towards the right
    if (initial_Position1 >= 10)
    { initial_Position1 = initial_Position1 - 20;
      servo1.write(initial_Position1);
      delay(100);
    }
  }
  if (x_Pos > 700) {      //If the value for the horizontal position is greater than 700, then the servo will move towards the left
    if (initial_Position1 <= 180)
    {
      initial_Position1 = initial_Position1 + 20;
      servo1.write(initial_Position1);
      delay (100);
    }
  }

  if (y_Pos < 300) {   // if the value is less than 300, then the second servo will move towards the left
    if (initial_Position2 >= 10) {
      initial_Position2 = initial_Position2 - 20;
      servo2.write (initial_Position2);
      delay (100);
    }
  }
  if (y_Pos > 700) {   //if the value is greater than 700, then the second servo will move towards the right.
    if (initial_Position2 <= 180)
    {
      initial_Position2 = initial_Position2 + 20;
      servo2.write(initial_Position2);
      delay(100);
    }
  }
}
